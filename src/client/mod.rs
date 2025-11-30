// client.rs
use crate::{GameState, shared::Player};
use avian3d::prelude::*;
use bevy::prelude::*;

pub struct ClientPlugin;

impl Plugin for ClientPlugin {
    fn build(&self, app: &mut App) {
        app.add_systems(Startup, spawn_camera);
        app.add_systems(Update, move_player.run_if(in_state(GameState::InGame)));

        app.add_systems(
            PostUpdate,
            camera_follow
                .run_if(in_state(GameState::InGame))
                .after(PhysicsSystems::Writeback),
        );
    }
}

#[derive(Component)]
struct GameCamera;

fn spawn_camera(mut commands: Commands) {
    debug!("Spawning camera.");
    commands.spawn((
        Name::new("GameCamera"),
        GameCamera,
        Camera3d::default(),
        Transform::from_xyz(0., 30., 10.).looking_at(Vec3::ZERO, Vec3::Y),
    ));
}

fn move_player(
    mut velocity: Single<&mut LinearVelocity, With<Player>>,
    input: Res<ButtonInput<KeyCode>>,
) {
    let speed = 10.0;
    let mut direction = Vec3::ZERO;

    if input.pressed(KeyCode::KeyW) {
        direction.z -= 1.0;
    }
    if input.pressed(KeyCode::KeyS) {
        direction.z += 1.0;
    }
    if input.pressed(KeyCode::KeyA) {
        direction.x -= 1.0;
    }
    if input.pressed(KeyCode::KeyD) {
        direction.x += 1.0;
    }

    if direction.length_squared() > 0. {
        direction = direction.normalize();

        velocity.x = direction.x * speed;
        velocity.z = direction.z * speed;
    } else {
        velocity.x = 0.;
        velocity.z = 0.;
    }
}

fn camera_follow(
    player_transform: Single<&Transform, (With<Player>, Without<GameCamera>)>,
    mut camera_transform: Single<&mut Transform, With<GameCamera>>,
) {
    let offset = Vec3::new(0., 30., 10.);
    camera_transform.translation = player_transform.translation + offset;
}
