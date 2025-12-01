use bevy::prelude::*;
use bevy_replicon::prelude::*;
use bevy_replicon_renet::renet::ServerEvent;
use serde::{Deserialize, Serialize};

pub struct ServerPlugin;
impl Plugin for ServerPlugin {
    fn build(&self, app: &mut bevy::app::App) {
        app.add_server_message::<WelcomeMessage>(Channel::Ordered);
        app.add_systems(Update, server_message_handler);
    }
}

#[derive(Message, Deserialize, Serialize)]
pub struct WelcomeMessage(pub String);

fn server_message_handler(mut server_messages: MessageReader<ServerEvent>) {
    for message in server_messages.read() {
        match message {
            ServerEvent::ClientConnected { client_id } => {
                info!("New client {} connected!", *client_id);
            }
            ServerEvent::ClientDisconnected { client_id, reason } => {
                info!("Client {} disconnected. Reason: {}", *client_id, *reason);
            }
        }
    }
}
