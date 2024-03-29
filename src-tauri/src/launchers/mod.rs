#[cfg(target_os = "windows")]
mod epic_games;
mod minecraft;
#[cfg(target_os = "windows")]
mod riot_games;
#[cfg(target_os = "windows")]
mod rockstar_games;
mod steam;
#[cfg(target_os = "windows")]
mod uplay;
#[cfg(target_os = "linux")]
mod wine_managers;
// #[cfg(target_os = "windows")]
// mod xbox_games;

use serde::{Deserialize, Serialize};

#[cfg(target_os = "windows")]
const LINE_ENDING: &'static str = "\r\n";

#[derive(Debug, Serialize, Deserialize)]
pub struct GameObject {
    banner_path: String,
    executable: String,
    location: String,
    display_name: String,
    game_id: String,
    launch_id: String,
    size: i64,
    launch_command: String,
    launcher_name: String,
    args: Vec<String>,
}

impl GameObject {
    pub fn new(
        banner_path: String,
        executable: String,
        location: String,
        display_name: String,
        game_id: String,
        launch_id: String,
        size: i64,
        launch_command: String,
        launcher_name: String,
        args: Vec<String>,
    ) -> Self {
        Self {
            banner_path,
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

#[cfg(target_os = "linux")]
fn is_installed(native_name: &str, flatpak_name: &str) -> bool {
    let flatpak = String::from_utf8(std::process::Command::new("flatpak")
        .arg("list")
        .output()
        .expect("Failed to execute command").stdout).unwrap_or_default();

    let native = String::from_utf8(std::process::Command::new("which")
        .arg(native_name)
        .output()
        .expect("Failed to execute command").stdout).unwrap_or_default();

    if native.starts_with("/") {
        return true;
    } else {
        if flatpak.contains(flatpak_name) {
            return true;
        }
    }

    return false;
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
    #[cfg(target_os = "windows")]
    installed_games.extend(uplay::get_installed_games().await);
    // #[cfg(target_os = "windows")]
    // installed_games.extend(xbox_games::get_installed_games().await);
    installed_games
}
