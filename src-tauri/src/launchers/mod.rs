#[cfg(target_os = "windows")]
pub mod epic_games;
pub mod minecraft;
#[cfg(target_os = "windows")]
pub mod riot_games;
#[cfg(target_os = "windows")]
pub mod rockstar_games;
pub mod steam;
#[cfg(target_os = "windows")]
pub mod uplay;
#[cfg(target_os = "linux")]
pub mod wine_managers;
#[cfg(target_os = "windows")]
pub mod xbox_games;

use serde::{Deserialize, Serialize};

#[derive(Debug, Serialize, Deserialize)]
pub struct GameObject {
    executable: String,
    location: String,
    pub display_name: String,
    game_id: String,
    launch_id: String,
    banner_id: String,
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
        launch_id: String,
        banner_id: String,
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
            banner_id,
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
    #[cfg(target_os = "linux")]
    installed_games.extend(wine_managers::get_installed_games().await);
    #[cfg(target_os = "windows")]
    installed_games.extend(epic_games::get_installed_games().await);
    #[cfg(target_os = "windows")]
    installed_games.extend(riot_games::get_installed_games().await);
    #[cfg(target_os = "windows")]
    installed_games.extend(rockstar_games::get_installed_games().await);
    installed_games
}
