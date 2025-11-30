use bevy::prelude::*;
use bevy_egui::EguiPlugin;
use bevy_infinite_grid::*;
use bevy_inspector_egui::quick::WorldInspectorPlugin;

fn main() {
    let mut app = App::new();

    app.add_plugins(DefaultPlugins);
    app.add_plugins(InfiniteGridPlugin);
    app.add_plugins(EguiPlugin::default());
    app.add_plugins(WorldInspectorPlugin::default());

    app.run();
}

fn setup_grid(mut commands: Commands) {
    commands.spawn((
        DirectionalLight {
            ..Default::default()
        },
        Transform::from_xyz(0., 1000., 0.).looking_at(Vec3::ZERO, Vec3::Y),
    ));
    commands.spawn(InfiniteGridBundle::default());
}
