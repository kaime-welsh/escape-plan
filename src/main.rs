use bevy::prelude::*;
use bevy_egui::EguiPlugin;
use bevy_inspector_egui::quick::WorldInspectorPlugin;
use bevy_replicon::prelude::*;
use bevy_replicon_renet::{
    RenetChannelsExt, RepliconRenetPlugins,
    netcode::{
        ClientAuthentication, NetcodeClientTransport, NetcodeServerTransport, ServerAuthentication,
        ServerConfig,
    },
    renet::{ConnectionConfig, RenetClient, RenetServer},
};
use escape_plan::{Cli, client, server};
use std::{
    net::{Ipv4Addr, SocketAddr, UdpSocket},
    time::SystemTime,
};

fn main() {
    let mut app = App::new();

    app.init_resource::<Cli>();

    app.add_plugins(DefaultPlugins);
    app.add_plugins(EguiPlugin::default());
    app.add_plugins(WorldInspectorPlugin::default());
    app.add_plugins((RepliconPlugins, RepliconRenetPlugins));

    app.init_state::<escape_plan::GameState>();

    app.add_plugins(server::ServerPlugin);
    app.add_plugins(client::ClientPlugin);

    app.add_systems(Startup, setup);

    app.run();
}

fn setup(mut commands: Commands, cli: Res<Cli>, channels: Res<RepliconChannels>) -> Result<()> {
    const PROTOCOL_ID: u64 = 0;
    commands.spawn(Camera2d);

    match *cli {
        Cli::SinglePlayer => {
            info!("starting single-player game");
        }
        Cli::Server { port } => {
            info!("starting server at port {port}");

            let server_channels_config = channels.server_configs();
            let client_channels_config = channels.client_configs();

            let server = RenetServer::new(ConnectionConfig {
                server_channels_config,
                client_channels_config,
                ..Default::default()
            });

            let current_time = SystemTime::now().duration_since(SystemTime::UNIX_EPOCH)?;
            let socket = UdpSocket::bind((Ipv4Addr::UNSPECIFIED, port))?;
            let server_config = ServerConfig {
                current_time,
                max_clients: 1,
                protocol_id: PROTOCOL_ID,
                authentication: ServerAuthentication::Unsecure,
                public_addresses: Default::default(),
            };
            let transport = NetcodeServerTransport::new(server_config, socket)?;

            commands.insert_resource(server);
            commands.insert_resource(transport);
        }
        Cli::Client { ip, port } => {
            info!("connecting to {ip}:{port}");

            // Backend initialization
            let server_channels_config = channels.server_configs();
            let client_channels_config = channels.client_configs();

            let client = RenetClient::new(ConnectionConfig {
                server_channels_config,
                client_channels_config,
                ..Default::default()
            });

            let current_time = SystemTime::now().duration_since(SystemTime::UNIX_EPOCH)?;
            let client_id = current_time.as_millis() as u64;
            let server_addr = SocketAddr::new(ip, port);
            let socket = UdpSocket::bind((Ipv4Addr::UNSPECIFIED, 0))?;
            let addr = socket.local_addr()?;
            let authentication = ClientAuthentication::Unsecure {
                client_id,
                protocol_id: PROTOCOL_ID,
                server_addr,
                user_data: None,
            };
            let transport = NetcodeClientTransport::new(current_time, authentication, socket)?;

            commands.insert_resource(client);
            commands.insert_resource(transport);

            commands.spawn(Text(format!("Client: {addr}")));
        }
    }

    Ok(())
}
