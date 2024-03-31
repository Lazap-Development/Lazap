use crate::{launchers::GameObject, modules::banners};

use crate::operations::custom_fs::d_f_exists;

#[cfg(target_os = "windows")]
use std::{os::windows::process::CommandExt, process::Command, str};

#[cfg(target_os = "windows")]
use crate::launchers::LINE_ENDING;

#[cfg(any(target_os = "linux"))]
use tauri::api::path;

#[cfg(any(target_os = "linux"))]
use crate::launchers::is_installed;

pub async fn get_installed_games() -> Vec<GameObject> {
    let mut all_games: Vec<GameObject> = Vec::new();

    if let Some(got_something) = get_minecraft_launcher().await {
        all_games.push(got_something);
    }

    return all_games;
}

async fn get_minecraft_launcher() -> Option<GameObject> {
    #[cfg(target_os = "windows")]
    {
        let output = Command::new("cmd")
            .args(&[
                "/C",
                "Reg",
                "query",
                "HKEY_CLASSES_ROOT\\Applications\\MinecraftLauncher.exe",
            ])
            .creation_flags(0x08000000)
            .output()
            .ok()?;

        if output.stderr.is_empty() {
            let stdout_str = str::from_utf8(&output.stdout).ok()?;
            let lines: Vec<&str> = stdout_str.lines().collect();
            let location_executable: Vec<&str> =
                lines[1].split("REG_SZ").map(|s| s.trim()).collect();

            if location_executable.len() > 1 {
                let location = location_executable[1].split('"').nth(1)?.to_string();
                let args = location_executable[1]
                    .split('"')
                    .nth(2)?
                    .split_whitespace()
                    .map(|s| s.to_string())
                    .collect();
                let executable = location.split("\\").last()?.to_string();

                if !d_f_exists(&location).await.unwrap() {
                    return None;
                }

                return Some(GameObject::new(
                    banners::get_banner("Minecraft Launcher", "Minecraft", "Minecraft").await,
                    executable,
                    location,
                    "Minecraft Launcher".to_string(),
                    "Minecraft".to_string(),
                    "0".to_string(),
                    0,
                    "".to_string(),
                    "Minecraft".to_string(),
                    args,
                ));
            } else {
                return None;
            }
        } else {
            let is_installed_output = Command::new("cmd")
                .args(&[
                    "/C",
                    "Reg",
                    "query",
                    "HKEY_CURRENT_USER\\Software\\Classes\\Local Settings\\Software\\Microsoft\\Windows\\CurrentVersion\\AppModel\\Repository\\Packages",
                    "/s",
                    "/f",
                    "Microsoft.4297127D64EC6*"
                ])
                .creation_flags(0x08000000)
                .output()
                .ok()?
                .stdout;

            let is_installed_str = str::from_utf8(&is_installed_output).ok()?;

            if is_installed_str.contains("0 match") {
                return None;
            }

            let minecraft_reg_output = Command::new("cmd")
                .args(&[
                    "/C",
                    "Reg",
                    "query",
                    is_installed_str.split(&LINE_ENDING).nth(1).unwrap(),
                ])
                .creation_flags(0x08000000)
                .output()
                .ok()?
                .stdout;

            let location = str::from_utf8(&minecraft_reg_output)
                .ok()?
                .split(&LINE_ENDING)
                .find(|x| x.trim().starts_with("PackageRootFolder"))
                .map(|x| x.split("    ").nth(3).unwrap_or("").trim().to_string())?;

            return Some(GameObject::new(
                banners::get_banner("Minecraft Launcher", "Minecraft", "Minecraft").await,
                "Minecraft.exe".to_string(),
                location,
                "Minecraft Launcher".to_string(),
                "Minecraft".to_string(),
                "0".to_string(),
                0,
                "".to_string(),
                "Minecraft".to_string(),
                vec![],
            ));
        }
    }

    #[cfg(any(target_os = "linux"))]
    {
        if !is_installed("minecraft-launcher", "com.mojang.Minecraft") {
            return None;
        }

        let home_dir = path::home_dir()
            .unwrap()
            .into_os_string()
            .into_string()
            .unwrap();

        if !d_f_exists(&format!("{}/.minecraft", home_dir))
            .await
            .unwrap()
        {
            return None;
        }

        let location = "/usr/bin/minecraft-launcher";
        let executable = "minecraft-launcher";

        return Some(GameObject::new(
            banners::get_banner("Minecraft Launcher", "Minecraft", "Minecraft").await,
            executable.to_string(),
            location.to_string(),
            "Minecraft Launcher".to_string(),
            "Minecraft".to_string(),
            "0".to_string(),
            0,
            "".to_string(),
            "Minecraft".to_string(),
            vec![],
        ));
    }
}
