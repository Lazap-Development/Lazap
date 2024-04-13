use crate::{
    launchers::GameObject,
    operations::{
        custom_fs::{
            read_file,
            write_file,
            d_f_exists,
        },
        misc::sha256,
    }
};
use std::fs;
use tauri::api::path;

struct CustomGameInput {
    location: String,
    display_name: String,
}

pub async fn get_installed_games() -> Vec<GameObject> {
    #[cfg(target_os = "windows")]
    let games_data = read_file(path::config_dir().unwrap().into_os_string().into_string().unwrap() + "\\com.lazap.config\\cache\\games\\data.json").unwrap();
    #[cfg(target_os = "linux")]
    let games_data = read_file(path::config_dir().unwrap().into_os_string().into_string().unwrap() + "\\com.lazap.config\\cache\\games\\data.json").unwrap();
    #[cfg(target_os = "macos")]
    return vec![];
    let games: Vec<GameObject> = serde_json::from_str(&games_data).unwrap();
    let custom_games = games.into_iter().filter(|x| x.launcher_name == "CustomGame");
    return custom_games.collect::<Vec<GameObject>>()
}

#[tauri::command]
pub async fn add_game(data: CustomGameInput) {
    let mut obj: GameObject = GameObject::new(
        "".to_string(),
        "".to_string(),
        "".to_string(),
        data.display_name,
        "CustomGame".to_string(),
        "CustomGame".to_string(),
        0,
        "".to_string(),
        "".to_string(),
        vec![]
    );
    obj.executable = data.location.split("\\").collect::<Vec<_>>().into_iter().rev().collect::<Vec<_>>()[0].to_string();
    obj.location = data.location.split("\\").collect::<Vec<_>>().into_iter().filter(|x| x.to_string() != obj.executable).collect::<Vec<_>>().join("\\");

    let newbannerpath: String = path::config_dir().unwrap().into_os_string().into_string().unwrap();
    if (d_f_exists(&(newbannerpath.to_string() + "\\com.lazap.config\\cache\\games\\banners\\newcustombanner.png")).await.expect("Something went wrong")) {
        let bannername = sha256(obj.display_name.replace(" ", "_")).await.unwrap();
        fs::rename(newbannerpath.to_string(), bannername.to_string());
        obj.banner_path = newbannerpath + "\\com.lazap.config\\cache\\games\\banners\\" + &bannername;
    }
    let mut games_data: Vec<GameObject> = serde_json::from_str(&read_file(path::config_dir().unwrap().into_os_string().into_string().unwrap() + "\\com.lazap.config\\cache\\games\\data.json").unwrap()).unwrap();
    games_data.push(obj);
    write_file(path::config_dir().unwrap().into_os_string().into_string().unwrap() + "\\com.lazap.config\\cache\\games\\data.json", serde_json::to_string(&games_data).unwrap());
}