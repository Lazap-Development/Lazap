pub mod epic_games;
pub mod lutris;
pub mod minecraft;
pub mod riot_games;
pub mod rockstar_games;
pub mod steam;
pub mod uplay;
pub mod xbox_games;

use serde::{Deserialize, Serialize};

#[derive(Debug, Serialize, Deserialize)]
pub struct GameObject {
    executable: String,
    location: String,
    pub display_name: String,
    game_id: String,
    launch_id: i32,
    size: i64,
    launch_command: String,
    launcher_name: String,
    args: Vec<String>,
}

impl GameObject {
    pub fn new(
        executable: String,
        location: String,
        display_name: String,
        game_id: String,
        launch_id: i32,
        size: i64,
        launch_command: String,
        launcher_name: String,
        args: Vec<String>,
    ) -> Self {
        Self {
            executable,
            location,
            display_name,
            game_id,
            launch_id,
            size,
            launch_command,
            launcher_name,
            args,
        }
    }
}

#[tauri::command]
pub async fn fetch_installed_games() -> Vec<GameObject> {
    let mut installed_games: Vec<GameObject> = Vec::new();
    installed_games.extend(steam::get_installed_games().await);
    installed_games.extend(minecraft::get_installed_games().await);
    installed_games
}
