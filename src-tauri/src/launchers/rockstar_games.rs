use std::{os::windows::process::CommandExt, process::Command};

use serde::Deserialize;

use crate::{
    launchers::{GameObject, LINE_ENDING},
    modules::banners,
    operations::custom_fs::{d_f_exists, read_dir},
};

static ROCKSTAR_JSON: &str = r#"{
    "titles": [
        {
            "titleId": "gta5",
            "rosTitleId": 11,
            "friendlyName": "Grand Theft Auto V",
            "installFolderRegKey": "Rockstar Games\\Grand Theft Auto V",
            "installFolderRegValueName": "InstallFolder",
            "executable": "GTA5.exe",
            "folder": "GTA V",
            "aliases": ["GTAV"],
            "modWhitelist": [
                "GTAVLauncher.exe",
                "Rockstar-Games-Launcher.exe",
                "PlayGTAV.exe"
            ],
            "minRglVersion": "1.0.33.319",
            "minGameVersion": "1.0.2060.0",
            "bannerId": "V"
        },
        {
            "titleId": "rdr2",
            "rosTitleId": 13,
            "friendlyName": "Red Dead Redemption 2",
            "installFolderRegKey": "Rockstar Games\\Red Dead Redemption 2",
            "installFolderRegValueName": "InstallFolder",
            "executable": "RDR2.exe",
            "folder": "Red Dead Redemption 2",
            "bannerId": "reddeadredemption2"
        },
        {
            "titleId": "lanoire",
            "rosTitleId": 9,
            "friendlyName": "L.A. Noire",
            "installFolderRegKey": "Rockstar Games\\LANoireMTL",
            "installFolderRegValueName": "InstallFolder",
            "executable": "LANoire.exe",
            "folder": "L.A. Noire",
            "bannerId": "lanoire"
        },
        {
            "titleId": "mp3",
            "rosTitleId": 10,
            "friendlyName": "Max Payne 3",
            "installFolderRegKey": "Rockstar Games\\MP3MTL",
            "installFolderRegValueName": "InstallFolder",
            "executable": "MaxPayne3.exe",
            "folder": "Max Payne 3",
            "bannerId": "maxpayne3"
        },
        {
            "titleId": "lanoirevr",
            "rosTitleId": 24,
            "friendlyName": "L.A. Noire: The VR Case Files",
            "installFolderRegKey": "Rockstar Games\\LANoireVR",
            "installFolderRegValueName": "InstallFolder",
            "executable": "LANoireVR.exe",
            "folder": "L.A. Noire VR",
            "bannerId": "lanoire"
        },
        {
            "titleId": "gtasa",
            "rosTitleId": 18,
            "friendlyName": "Grand Theft Auto: San Andreas",
            "installFolderRegKey": "Rockstar Games\\GTA: San Andreas",
            "installFolderRegValueName": "InstallFolder",
            "executable": "gta_sa.exe",
            "folder": "GTA San Andreas",
            "bannerId": "sanandreas"
        },
        {
            "titleId": "gta3",
            "rosTitleId": 26,
            "friendlyName": "Grand Theft Auto III",
            "installFolderRegKey": "Rockstar Games\\Grand Theft Auto III",
            "installFolderRegValueName": "InstallFolder",
            "executable": "gta3.exe",
            "folder": "Grand Theft Auto III",
            "bannerId": "grandtheftauto3"
        },
        {
            "titleId": "gtavc",
            "rosTitleId": 27,
            "friendlyName": "Grand Theft Auto: Vice City",
            "installFolderRegKey": "Rockstar Games\\Grand Theft Auto: Vice City",
            "installFolderRegValueName": "InstallFolder",
            "executable": "gta-vc.exe",
            "folder": "GTA Vice City",
            "bannerId": "vicecity"
        },
        {
            "titleId": "bully",
            "rosTitleId": 23,
            "friendlyName": "Bully: Scholarship Edition",
            "installFolderRegKey": "Rockstar Games\\Bully",
            "installFolderRegValueName": "InstallFolder",
            "executable": "Bully.exe",
            "folder": "Bully",
            "bannerId": "bully"
        },
        {
            "titleId": "gta4",
            "rosTitleId": 1,
            "friendlyName": "Grand Theft Auto IV",
            "installFolderRegKey": "Rockstar Games\\Grand Theft Auto IV",
            "installFolderRegValueName": "InstallFolder",
            "executable": "GTAIV.exe",
            "folder": "GTA IV",
            "bannerId": "IV"
        },
        {
            "titleId": "gta3unreal",
            "rosTitleId": 28,
            "friendlyName": "GTA III - Definitive Edition",
            "installFolderRegKey": "Rockstar Games\\GTA III - DE",
            "installFolderRegValueName": "InstallFolder",
            "executable": "Gameface\\Binaries\\Win64\\LibertyCity.exe",
            "folder": "GTA III Definitive Edition",
            "minRglVersion": "1.0.53.576",
            "minGameVersion": "1.0.0.14377",
            "bannerId": "GTATrilogy"
        },
        {
            "titleId": "gtavcunreal",
            "rosTitleId": 29,
            "friendlyName": "GTA Vice City - Definitive Edition",
            "installFolderRegKey": "Rockstar Games\\GTA Vice City - DE",
            "installFolderRegValueName": "InstallFolder",
            "executable": "Gameface\\Binaries\\Win64\\ViceCity.exe",
            "folder": "GTA Vice City Definitive Edition",
            "minRglVersion": "1.0.53.576",
            "minGameVersion": "1.0.0.14377",
            "bannerId": "GTATrilogy"
        },
        {
            "titleId": "gtasaunreal",
            "rosTitleId": 30,
            "friendlyName": "GTA San Andreas - Definitive Edition",
            "installFolderRegKey": "Rockstar Games\\GTA San Andreas - DE",
            "installFolderRegValueName": "InstallFolder",
            "executable": "Gameface\\Binaries\\Win64\\SanAndreas.exe",
            "folder": "GTA San Andreas Definitive Edition",
            "minRglVersion": "1.0.53.576",
            "minGameVersion": "1.0.0.14377",
            "bannerId": "GTATrilogy"
        }
    ]
}"#;

#[derive(Deserialize, Debug)]
struct RockstarGameData {
    titles: Vec<Titles>,
}

#[derive(Deserialize, Debug)]
struct Titles {
    #[serde(rename = "friendlyName")]
    friendly_name: String,
    #[serde(rename = "titleId")]
    title_id: String,
    #[serde(rename = "rosTitleId")]
    ros_title_id: i64,
    #[serde(rename = "bannerId")]
    banner_id: String,
    executable: String,
    #[serde(rename = "installFolderRegKey")]
    install_folder_reg_key: Option<String>,
    aliases: Option<Vec<String>>,
    #[serde(default)]
    #[serde(rename = "modWhitelist")]
    mod_whitelist: Vec<String>,
}

pub async fn get_installed_games() -> Vec<GameObject> {
    let mut all_games: Vec<GameObject> = Vec::new();

    let output = Command::new("cmd")
        .args(&[
            "/C",
            "Reg",
            "query",
            "HKEY_LOCAL_MACHINE\\SOFTWARE\\WOW6432Node\\Rockstar Games",
            "/s",
        ])
        .creation_flags(0x08000000)
        .output()
        .expect("failed to execute regedit query.");

    if output.stdout.is_empty() {
        return all_games;
    }

    for x in String::from_utf8_lossy(&output.stdout).split(&LINE_ENDING.repeat(2)) {
        let res: Vec<&str> = x.split(LINE_ENDING).filter(|x| x.len() > 1).collect();
        let name = match res.get(0) {
            Some(name) => {
                if let Some(split_name) = name
                    .split("HKEY_LOCAL_MACHINE\\SOFTWARE\\WOW6432Node\\Rockstar Games\\")
                    .nth(1)
                {
                    split_name
                } else {
                    continue;
                }
            }
            None => continue,
        };
        let base_paths = res.into_iter().filter(|x| x.trim().starts_with("Install"));
        let mut real_paths: Vec<&str> = Vec::new();
        for path in base_paths {
            if d_f_exists(path.split("REG_SZ").nth(1).unwrap().trim())
                .await
                .unwrap_or(false)
            {
                real_paths.push(path);
            }
        }
        if real_paths.is_empty() {
            continue;
        }

        let game_option = parse_game_object(
            &real_paths
                .get(0)
                .unwrap()
                .split("REG_SZ")
                .nth(1)
                .unwrap()
                .trim(),
            &name,
        )
        .await;

        if let Some(game_object) = game_option {
            all_games.push(game_object);
        }
    }

    return all_games;
}

async fn parse_game_object(path: &str, name: &str) -> Option<GameObject> {
    if !d_f_exists(path).await.unwrap_or(false) {
        return None;
    }
    let game_data: RockstarGameData = serde_json::from_str(ROCKSTAR_JSON).unwrap();
    let game_opt = game_data.titles.iter().find(|x| {
        name == x.friendly_name
            || x.aliases
                .as_ref()
                .map_or(false, |aliases| aliases.contains(&name.to_string()))
            || name
                == x.install_folder_reg_key
                    .as_ref()
                    .map_or("", |key| key.split("\\").nth(1).unwrap_or(""))
    });
    match game_opt {
        Some(game) => {
            let mut executable: String = String::new();
            if game.title_id == "gta5" {
                if let Ok(files) = read_dir(path).await {
                    if let Some(file) = files
                        .into_iter()
                        .find(|x| game.mod_whitelist.contains(&x.to_string()))
                    {
                        executable = file.to_string();
                    }
                }
            } else {
                executable = game.executable.clone();
            }

            return Some(GameObject::new(
                banners::get_banner(&game.friendly_name, &game.banner_id, "RockstarGames", "")
                    .await,
                executable.to_string(),
                path.to_string(),
                game.friendly_name.clone(),
                format!("{}-{}", game.title_id, game.ros_title_id),
                "0".to_string(),
                0,
                String::new(),
                "RockstarGames".to_string(),
                vec![],
            ));
        }
        None => return None,
    }
}
