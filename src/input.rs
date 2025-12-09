use bevy::prelude::*;
use escape_plan::GameState;
use serde::{Deserialize, Serialize};

pub struct InputPlugin;
impl Plugin for InputPlugin {
    fn build(&self, app: &mut App) {
        info!("Adding ClientInputPlugin");
        // FIXME: Make sure to change this to a ClientMessage once replicon is integrated!!!
        app.add_message::<InputMessage>();
        app.add_systems(
            Update,
            (client_gather_input)
                .chain()
                .run_if(in_state(GameState::InGame)), // .run_if(in_state(ClientState::Connected)),
        );
    }
}

#[derive(Message, Serialize, Deserialize)]
pub enum InputMessage {
    Move(Vec2),
}

pub fn client_gather_input(
    mut exit_writer: MessageWriter<AppExit>,
    mut input_writer: MessageWriter<InputMessage>,
    keyboard: Res<ButtonInput<KeyCode>>,
) {
    let mut direction = Vec2::new(0., 0.);

    if keyboard.pressed(KeyCode::KeyW) {
        direction.y = 1.;
    } else if keyboard.pressed(KeyCode::KeyS) {
        direction.y = -1.;
    }

    if keyboard.pressed(KeyCode::KeyA) {
        direction.x = -1.;
    } else if keyboard.pressed(KeyCode::KeyD) {
        direction.x = 1.;
    }

    #[cfg(debug_assertions)]
    if keyboard.pressed(KeyCode::Escape) {
        exit_writer.write(AppExit::Success);
    }

    input_writer.write(InputMessage::Move(direction.normalize()));
}
