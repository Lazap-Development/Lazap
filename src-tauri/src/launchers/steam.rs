use crate::launchers::GameObject;
use crate::modules::banners;
use crate::operations::custom_fs::{d_f_exists, read_dir, read_file};
use serde::Deserialize;
use std::collections::HashMap;

#[cfg(target_os = "windows")]
use std::{os::windows::process::CommandExt, process::Command};

#[cfg(any(target_os = "linux", target_os = "macos"))]
use tauri::api::path;

const BLACKLIST_APPID: &[i32] = &[
    228980, 231350, 1493710, 1391110, 1070560, 1826330, 1113280, 1245040, 1420170, 1580130,
    1887720, 1628350, 2348590, 2180100,
];

#[derive(Deserialize)]
#[serde(transparent)]
struct LibraryFolders {
    folders: HashMap<String, Folder>,
}

#[derive(Deserialize)]

struct Folder {
    path: String,
}

#[cfg(target_os = "macos")]
#[derive(Deserialize, Debug)]
struct AppState {
    appid: String,
    name: String,
    #[serde(rename = "SizeOnDisk")]
    size_on_disk: String,
    installdir: String,
}

#[cfg(target_os = "windows")]
#[derive(Deserialize, Debug)]
struct AppState {
    appid: String,
    name: String,
    #[serde(rename = "SizeOnDisk")]
    size_on_disk: String,
    #[serde(rename = "LauncherPath")]
    launcherpath: String,
}

#[cfg(target_os = "linux")]
#[derive(Deserialize, Debug)]
struct AppState {
    appid: String,
    name: String,
    #[serde(rename = "SizeOnDisk")]
    size_on_disk: String,
}

pub async fn get_installed_games() -> Vec<GameObject> {
    let mut all_games: Vec<GameObject> = Vec::new();

    let paths: Vec<String> = get_steam_location().await;
    if paths.is_empty() {
        return all_games;
    }

    for path in paths {
        #[cfg(target_os = "windows")]
        let acf_base_path = format!("{}\\steamapps", path);

        #[cfg(target_os = "linux")]
        let acf_base_path = format!("{}/steamapps", path);

        #[cfg(target_os = "macos")]
        let acf_base_path = format!("{}/steamapps", path);

        if !d_f_exists(&acf_base_path).await.unwrap_or(false) {
            continue;
        }

        let base_path_content = read_dir(&acf_base_path).await.unwrap();

        let acf_files: Vec<String> = base_path_content
            .into_iter()
            .filter(|filename| filename.ends_with(".acf"))
            .collect();

        for acf_file in acf_files {
            let game_file: String = read_file(format!("{}/{}", acf_base_path, acf_file)).unwrap();
            let game_file_parsed: AppState = keyvalues_serde::from_str(&game_file).unwrap();

            if BLACKLIST_APPID
                .iter()
                .any(|&x| x == game_file_parsed.appid.parse::<i32>().unwrap())
            {
                continue;
            }

            let mut temp_all_games: Vec<GameObject> = Vec::new();

            #[cfg(target_os = "macos")]
            temp_all_games.push(GameObject::new(
                banners::get_banner(&game_file_parsed.name, &game_file_parsed.appid, "Steam", "")
                    .await,
                String::new(),
                game_file_parsed.installdir.to_string(),
                game_file_parsed.name,
                game_file_parsed.appid.parse().unwrap(),
                "0".to_string(),
                game_file_parsed.size_on_disk.parse().unwrap(),
                String::new(),
                "Steam".to_string(),
                vec![],
            ));

            #[cfg(target_os = "windows")]
            temp_all_games.push(GameObject::new(
                banners::get_banner(&game_file_parsed.name, &game_file_parsed.appid, "Steam", "")
                    .await,
                String::new(),
                game_file_parsed.launcherpath.to_string(),
                game_file_parsed.name,
                game_file_parsed.appid.parse().unwrap(),
                "0".to_string(),
                game_file_parsed.size_on_disk.parse().unwrap(),
                String::new(),
                "Steam".to_string(),
                vec![],
            ));

            #[cfg(target_os = "linux")]
            temp_all_games.push(GameObject::new(
                banners::get_banner(&game_file_parsed.name, &game_file_parsed.appid, "Steam", "")
                    .await,
                String::new(),
                String::new(),
                game_file_parsed.name,
                game_file_parsed.appid.parse().unwrap(),
                "0".to_string(),
                game_file_parsed.size_on_disk.parse().unwrap(),
                String::new(),
                "Steam".to_string(),
                vec![],
            ));

            for game in temp_all_games {
                if !all_games
                    .iter()
                    .any(|obj| obj.display_name == game.display_name)
                {
                    all_games.push(game);
                }
            }
        }
    }

    return all_games;
}

async fn get_steam_location() -> Vec<String> {
    let mut launcher_location: Vec<String> = Vec::new();
    #[cfg(all(target_os = "windows", target_arch = "x86_64"))]
    let output = Command::new("cmd")
        .args(&[
            "/C",
            "Reg",
            "Query",
            "HKEY_LOCAL_MACHINE\\SOFTWARE\\WOW6432Node\\Valve\\Steam",
            "/v",
            "InstallPath",
        ])
        .creation_flags(0x08000000)
        .output()
        .expect("failed to execute process.");

    #[cfg(all(target_os = "windows", target_arch = "x86"))]
    let output = Command::new("cmd")
        .args(&[
            "/C",
            "Reg",
            "Query",
            "HKEY_LOCAL_MACHINE\\SOFTWARE\\Valve\\Steam",
            "/v",
            "InstallPath",
        ])
        .creation_flags(0x08000000)
        .output()
        .expect("failed to execute process.");

    #[cfg(target_os = "windows")]
    if output.stdout.is_empty() {
        return launcher_location;
    }

    #[cfg(target_os = "windows")]
    let path = String::from_utf8_lossy(&output.stdout)
        .to_string()
        .split("REG_SZ")
        .collect::<Vec<_>>()[1]
        .split("\r\n\r\n")
        .collect::<Vec<_>>()[0]
        .trim()
        .to_string()
        + "\\steamapps\\libraryfolders.vdf";

    #[cfg(target_os = "linux")]
    let path: String = path::home_dir()
        .unwrap()
        .into_os_string()
        .into_string()
        .unwrap()
        + "/.steam/steam/steamapps/libraryfolders.vdf";

    #[cfg(target_os = "macos")]
    let path: String = path::home_dir()
        .unwrap()
        .into_os_string()
        .into_string()
        .unwrap()
        + "/Library/Application Support/Steam/steamapps/libraryfolders.vdf";

    if d_f_exists(&path).await.unwrap_or(false) {
        let vdf_file = read_file(path.to_string()).unwrap();
        if vdf_file.is_empty() {
            return launcher_location;
        };

        let parsed: LibraryFolders = keyvalues_serde::from_str(&vdf_file).unwrap();
        for (_, folder) in parsed.folders.iter() {
            launcher_location.push(folder.path.clone());
        }
    } else {
        return launcher_location;
    }

    launcher_location
}
