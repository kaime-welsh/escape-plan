use bevy::prelude::*;
use escape_plan::GameState;

pub mod input;

fn main() {
    let mut app = App::new();

    app.add_plugins(DefaultPlugins);

    app.init_state::<GameState>();

    app.add_plugins(input::InputPlugin);

    app.run();
}
