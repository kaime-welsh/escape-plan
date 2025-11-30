use bevy::prelude::*;

pub mod player;

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
