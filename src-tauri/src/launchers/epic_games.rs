use serde::Deserialize;

use crate::{
    launchers::GameObject,
    modules::banners,
    operations::custom_fs::{d_f_exists, read_dir, read_file},
};

#[derive(Deserialize, Debug)]
struct EpicGamesItem {
    #[serde(rename = "LaunchExecutable")]
    launch_executable: String,
    #[serde(rename = "InstallLocation")]
    install_location: String,
    #[serde(rename = "DisplayName")]
    display_name: String,
    #[serde(rename = "AppName")]
    app_name: String,
    #[serde(rename = "InstallSize")]
    install_size: i64,
    #[serde(rename = "LaunchCommand")]
    launch_command: String,
    #[serde(rename = "CatalogNamespace")]
    catalog_namespace: String,
    #[serde(rename = "CatalogItemId")]
    catalog_item_id: String,
}

pub async fn get_installed_games() -> Vec<GameObject> {
    let mut all_games: Vec<GameObject> = Vec::new();

    if d_f_exists("C:\\ProgramData\\Epic\\EpicGamesLauncher\\Data\\Manifests")
        .await
        .expect("Something went wrong")
    {
        let entries = read_dir("C:\\ProgramData\\Epic\\EpicGamesLauncher\\Data\\Manifests")
            .await
            .unwrap();

        for entry in entries {
            if entry.ends_with(".item") {
                let file = read_file(format!(
                    "{}{}",
                    "C:\\ProgramData\\Epic\\EpicGamesLauncher\\Data\\Manifests\\", entry
                ));
                let file_prased: EpicGamesItem = serde_json::from_str(&file.unwrap()).unwrap();
                let app_name_clone = file_prased.app_name.clone();
                all_games.push(GameObject::new(
                    banners::get_banner(&file_prased.display_name, "", "EpicGames").await,
                    file_prased.launch_executable,
                    file_prased.install_location,
                    file_prased.display_name,
                    file_prased.app_name,
                    format!(
                        "{}:{}:{}",
                        file_prased.catalog_namespace, file_prased.catalog_item_id, app_name_clone
                    ),
                    file_prased.install_size,
                    file_prased.launch_command,
                    "EpicGames".to_string(),
                    vec![],
                ))
            }
        }
    }

    return all_games;
}
