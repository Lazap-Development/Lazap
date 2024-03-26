use std::{os::windows::process::CommandExt, process::Command};

use crate::launchers::{GameObject, LINE_ENDING};
use crate::operations::custom_fs::{d_f_exists, read_dir};

pub async fn get_installed_games() -> Vec<GameObject> {
    let mut all_games: Vec<GameObject> = Vec::new();

    if let Some((uplay_path, res)) = get_uplay_location().await {
        if d_f_exists(&uplay_path).await.unwrap_or(false) {
            for y in read_dir(&uplay_path).await.unwrap() {
                let game_res_loc = res
                    .split(&LINE_ENDING.repeat(2))
                    .find(|x| {
                        let trimmed_x = x.trim().split(&LINE_ENDING).nth(2).unwrap_or_default();
                        trimmed_x.contains("InstallDir") && trimmed_x.contains(y.as_str())
                    })
                    .unwrap_or_default()
                    .split(&LINE_ENDING)
                    .filter(|z| z.trim().starts_with("InstallDir"))
                    .next()
                    .unwrap_or_default()
                    .split("REG_SZ")
                    .nth(1)
                    .unwrap_or_default()
                    .trim()
                    .to_string();

                if !game_res_loc.is_empty() {
                    all_games.push(GameObject::new(
                        "".to_string(),
                        game_res_loc,
                        y,
                        123.to_string(),
                        "".to_string(),
                        "".to_string(),
                        0,
                        "".to_string(),
                        "Uplay".to_string(),
                        vec![],
                    ));
                }
            }
        }
    }

    return all_games;
}

async fn get_uplay_location() -> Option<(String, String)> {
    #[cfg(all(target_os = "windows", target_arch = "x86_64"))]
    let output = Command::new("cmd")
        .args(&[
            "/C",
            "Reg",
            "query",
            "HKEY_LOCAL_MACHINE\\SOFTWARE\\WOW6432Node\\Ubisoft\\Launcher",
            "/s",
        ])
        .creation_flags(0x08000000)
        .output()
        .expect("failed to execute regedit query.");

    #[cfg(all(target_os = "windows", target_arch = "x86"))]
    let output = Command::new("cmd")
        .args(&[
            "/C",
            "Reg",
            "query",
            "HKEY_LOCAL_MACHINE\\SOFTWARE\\Ubisoft\\Launcher",
            "/s",
        ])
        .creation_flags(0x08000000)
        .output()
        .expect("failed to execute regedit query.");

    if output.stdout.is_empty() {
        return None;
    }

    let registry_res: String = String::from_utf8(
        String::from_utf8(output.stdout.clone())
            .unwrap()
            .split(&LINE_ENDING.repeat(2))
            .next()
            .unwrap()
            .into(),
    )
    .unwrap();
    let launcher_location = registry_res
        .split("REG_SZ")
        .nth(1)
        .unwrap_or_default()
        .split(&LINE_ENDING.repeat(2))
        .next()
        .unwrap()
        .split(&LINE_ENDING)
        .next()
        .unwrap()
        .trim()
        .to_string();
    Some((
        launcher_location + "/games",
        String::from_utf8(output.stdout).unwrap(),
    ))
}
