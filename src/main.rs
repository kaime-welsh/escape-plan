use bevy::prelude::*;
use bevy_egui::EguiPlugin;
use bevy_infinite_grid::*;
use bevy_inspector_egui::quick::WorldInspectorPlugin;
use escape_plan::{client::ClientPlugin, server::ServerPlugin, shared::SharedPlugin, *};

fn main() {
    let mut app = App::new();

    app.add_plugins(DefaultPlugins);
    app.add_plugins(EguiPlugin::default());
    app.add_plugins(InfiniteGridPlugin);
    app.add_plugins(WorldInspectorPlugin::default());

    app.init_state::<GameState>();

    app.add_plugins(SharedPlugin);
    app.add_plugins(ClientPlugin);
    app.add_plugins(ServerPlugin);

    app.add_systems(OnEnter(GameState::InGame), setup_grid);

    app.run();
}

fn setup_grid(mut commands: Commands) {
    // commands.spawn((
    //     DirectionalLight {
    //         ..Default::default()
    //     },
    //     Transform::from_xyz(0., 1000., 0.).looking_at(Vec3::ZERO, Vec3::Y),
    // ));

    commands.spawn(InfiniteGridBundle::default());
}
