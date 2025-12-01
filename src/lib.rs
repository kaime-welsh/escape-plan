use std::net::{IpAddr, Ipv4Addr};

use bevy::prelude::*;
use clap::Parser;

pub mod client;
pub mod server;

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

#[derive(Parser, PartialEq, Resource)]
pub enum Cli {
    SinglePlayer,
    Server {
        #[arg(short, long, default_value_t = 5000)]
        port: u16,
    },
    Client {
        #[arg(short, long, default_value_t = Ipv4Addr::LOCALHOST.into())]
        ip: IpAddr,

        #[arg(short, long, default_value_t = 5000)]
        port: u16,
    },
}

impl Default for Cli {
    fn default() -> Self {
        Self::parse()
    }
}
