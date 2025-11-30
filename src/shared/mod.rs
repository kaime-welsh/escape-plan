// shared.rs
use avian3d::prelude::*;
use bevy::prelude::*;

use crate::GameState;

pub struct SharedPlugin;
impl Plugin for SharedPlugin {
    fn build(&self, app: &mut App) {
        app.add_plugins(PhysicsPlugins::default());
        #[cfg(debug_assertions)] // Disable in release builds
        app.add_plugins(PhysicsDebugPlugin::default());

        app.add_systems(OnEnter(GameState::InGame), setup_level);
    }
}

#[derive(Component)]
pub struct Player;

fn setup_level(
    mut commands: Commands,
    mut materials: ResMut<Assets<StandardMaterial>>,
    mut meshes: ResMut<Assets<Mesh>>,
) {
    debug!("Spawning ground.");
    commands.spawn((
        Name::new("Ground"),
        Mesh3d(meshes.add(Plane3d::default().mesh().size(50., 50.))),
        MeshMaterial3d(materials.add(Color::srgb(0.3, 0.5, 0.3))),
        RigidBody::Static,
        Collider::cuboid(50., 1., 50.),
    ));

    debug!("Spawning Player.");
    commands.spawn((
        Name::new("Player"),
        Player,
        Mesh3d(meshes.add(Sphere::new(1.).mesh())),
        MeshMaterial3d(materials.add(Color::srgb(0.8, 0.2, 0.2))),
        Transform::from_xyz(0., 2., 0.),
        RigidBody::Dynamic,
        Collider::sphere(1.),
        LinearDamping(1.),
        AngularDamping(1.),
        LockedAxes::ROTATION_LOCKED,
    ));

    commands.spawn((
        PointLight {
            intensity: 1500.,
            shadows_enabled: true,
            ..default()
        },
        Transform::from_xyz(4.0, 8.0, 4.0),
    ));
}
