use bevy::prelude::*;
use bevy_infinite_grid::InfiniteGridBundle;

pub mod client;
pub mod server;
pub mod shared;

#[derive(States, Default, Debug, Hash, Eq, PartialEq, Clone)]
pub enum GameState {
    MainMenu,
    #[default]
    InGame,
    GameOver,
}

#[derive(States, Default, Debug, Hash, Eq, PartialEq, Clone)]
pub enum PausedState {
    #[default]
    Running,
    Paused,
}
