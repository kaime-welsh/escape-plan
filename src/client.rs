use bevy::prelude::*;

use crate::server::WelcomeMessage;

pub struct ClientPlugin;
impl Plugin for ClientPlugin {
    fn build(&self, app: &mut App) {
        info!("Added client plugin.");
    }
}
