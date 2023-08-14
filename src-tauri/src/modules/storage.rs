use crate::addons;

use rdev::{listen, simulate, Button, EventType, Key, SimulateError};
use serde_json::Value;
use std::fs;
use std::io::Write;
use std::path::Path;
use std::{thread, time};
use tauri::Manager;

fn create_dir_if_not_exists(path: &str) {
    if !Path::new(path).exists() {
        fs::create_dir_all(path).expect("Failed to create dir.");
    }
}

fn create_file_if_not_exists(file_path: &str, content: &str) -> Result<(), std::io::Error> {
    if !Path::new(file_path).exists() {
        let mut file = fs::File::create(file_path)?;
        writeln!(file, "{}", content)?;
    }
    Ok(())
}

pub fn init_storage() -> Result<(), std::io::Error> {
    let base_config_path = format!(
        "{}com.lazap.config",
        tauri::api::path::app_config_dir(&tauri::Config::default())
            .ok_or(std::io::Error::new(
                std::io::ErrorKind::Other,
                "Failed to retrieve app config dir"
            ))?
            .display()
    );
    let base_config_cache_path = format!("{}/cache", base_config_path);
    let base_config_cache_games_path = format!("{}/games", base_config_cache_path);
    let base_config_cache_user_path = format!("{}/user", base_config_cache_path);
    let base_config_cache_games_banners_path = format!("{}/banners", base_config_cache_games_path);
    let base_config_ld_file = format!("{}/LauncherData.json", base_config_path);
    let base_config_cache_user_data_file = format!("{}/data.json", base_config_cache_user_path);
    let base_config_cache_game_data_file = format!("{}/data.json", base_config_cache_games_path);

    create_dir_if_not_exists(&base_config_path);
    create_dir_if_not_exists(&base_config_cache_path);
    create_dir_if_not_exists(&base_config_cache_games_path);
    create_dir_if_not_exists(&base_config_cache_games_banners_path);
    create_dir_if_not_exists(&base_config_cache_user_path);

    let json_content = "{ 
        \"enable_rpc\": true, 
        \"enable_spotify\": false, 
        \"enable_overlay\": false,
        \"launch_on_startup\": false, 
        \"skip_login\": false, 
        \"tray_min_launch\": true, 
        \"tray_min_quit\": false, 
        \"check_for_updates\": true, 
        \"accentColor\": \"#7934FA\",
        \"frontColor\": \"#2a1051\",
        \"backgroundColor\": \"#140e24\",
        \"primaryColor\": \"#0c0b0e\"
    }";

    create_file_if_not_exists(&base_config_ld_file, json_content)?;

    create_file_if_not_exists(
        &base_config_cache_user_data_file,
        &format!("{{\"username\": \"{}\"}}", whoami::username()),
    )?;
    create_file_if_not_exists(&base_config_cache_game_data_file, "[]")?;

    let expected_config: Value = serde_json::from_str(json_content)?;
    let actual_config: Value =
        serde_json::from_str(&fs::read_to_string(base_config_ld_file.clone()).unwrap())?;

    let expected_keys = extract_keys(&expected_config);
    let actual_keys = extract_keys(&actual_config);

    if actual_keys != expected_keys {
        let missing_keys: Vec<&String> = expected_keys
            .iter()
            .filter(|key| !actual_keys.contains(*key))
            .collect();
        for key in &missing_keys {
            let mut config: Value =
                serde_json::from_str(&fs::read_to_string(base_config_ld_file.clone()).unwrap())?;
            if let Some(value) = expected_config[key].as_str() {
                if let Value::Object(ref mut obj) = config {
                    obj.insert(key.to_string(), value.into());
                }
                fs::write(base_config_ld_file.clone(), config.to_string())?;
            } else if let Some(value) = expected_config[key].as_bool() {
                if let Value::Object(ref mut obj) = config {
                    obj.insert(key.to_string(), value.into());
                }
                fs::write(base_config_ld_file.clone(), config.to_string())?;
            }
        }
    }

    Ok(())
}

fn extract_keys(json: &Value) -> Vec<String> {
    match json {
        Value::Object(obj) => obj.keys().map(|s| s.to_string()).collect(),
        _ => Vec::new(),
    }
}

pub fn launcherdata_threads(window: tauri::Window) -> Result<(), std::io::Error> {
    let base_config_path = format!(
        "{}com.lazap.config",
        tauri::api::path::app_config_dir(&tauri::Config::default())
            .ok_or(std::io::Error::new(
                std::io::ErrorKind::Other,
                "Failed to retrieve app config dir"
            ))?
            .display()
    );
    let base_config_ld_file = format!("{}/LauncherData.json", base_config_path);

    #[derive(serde::Deserialize)]
    struct LauncherData {
        enable_spotify: bool,
        enable_overlay: bool,
    }

    match fs::read_to_string(&base_config_ld_file) {
        Ok(contents) => {
            let contents_str: &str = contents.as_str();
            let json: LauncherData =
                serde_json::from_str(contents_str).expect("JSON was not well-formatted");

            if json.enable_spotify {
                thread::Builder::new()
                    .name("lazap_spotify".to_string())
                    .spawn(move || {
                        addons::spotify::main().expect("Failed to init spotify addon server.");
                    })
                    .expect("Failed to spawn thread.");
            }

            if json.enable_overlay {
                thread::Builder::new()
                    .name("keybind".to_string())
                    .spawn(move || {
                        if let Err(error) = listen(move |event| {
                            static mut CTRLPRESSED: bool = false;
                            static mut SHIFTPRESSED: bool = false;

                            match event.event_type {
                                EventType::KeyPress(key) => unsafe {
                                    thread::sleep(time::Duration::from_millis(70));
                                    match key {
                                        Key::ControlLeft => CTRLPRESSED = true,
                                        Key::ShiftLeft => SHIFTPRESSED = true,
                                        Key::KeyL => {
                                            if SHIFTPRESSED && CTRLPRESSED {
                                                let overlay_window =
                                                    window.get_window("overlay").unwrap();
                                                let main_window =
                                                    window.get_window("main").unwrap();
                                                if overlay_window.is_visible().unwrap() {
                                                    overlay_window.hide().unwrap();
                                                    let _ = main_window.show();

                                                    // Workaround for showing overlay on top of Fullscreen Games (x11, Wayland)
                                                    send(&EventType::KeyPress(Key::Alt));
                                                    send(&EventType::KeyPress(Key::Return));
                                                    send(&EventType::KeyRelease(Key::Alt));
                                                    send(&EventType::KeyRelease(Key::Return));
                                                    send(&EventType::ButtonPress(Button::Left));
                                                    send(&EventType::ButtonRelease(Button::Left));
                                                } else {
                                                    overlay_window.show().unwrap();
                                                    let _ = main_window.hide();

                                                    // Workaround for showing overlay on top of Fullscreen Games (x11, Wayland)
                                                    send(&EventType::KeyPress(Key::Alt));
                                                    send(&EventType::KeyPress(Key::Return));
                                                    send(&EventType::KeyRelease(Key::Alt));
                                                    send(&EventType::KeyRelease(Key::Return));
                                                    send(&EventType::ButtonPress(Button::Left));
                                                    send(&EventType::ButtonRelease(Button::Left));
                                                }
                                            }
                                        }
                                        _ => {}
                                    }
                                },
                                EventType::KeyRelease(key) => unsafe {
                                    thread::sleep(time::Duration::from_millis(70));
                                    match key {
                                        Key::ControlLeft => CTRLPRESSED = false,
                                        Key::ShiftLeft => SHIFTPRESSED = false,
                                        _ => {}
                                    }
                                },
                                _ => {}
                            }

                            fn send(event_type: &EventType) {
                                match simulate(event_type) {
                                    Ok(()) => (),
                                    Err(SimulateError) => {
                                        println!("We could not send {:?}", event_type);
                                    }
                                }
                                thread::sleep(time::Duration::from_millis(20));
                            }
                        }) {
                            println!("Error: {:?}", error)
                        }
                    })
                    .expect("Failed to spawn thread.");
            }
        }
        Err(e) => {
            eprintln!("Error reading the file: {}", e);
        }
    }
    Ok(())
}

#[tauri::command]
pub async fn launcherdata_threads_x(window: tauri::Window) -> Result<(), Error> {
    launcherdata_threads(window).expect("Failed to call launcherdata_threads.");
    Ok(())
}

#[derive(Debug, thiserror::Error)]
pub enum Error {
    #[error(transparent)]
    Io(#[from] html_parser::Error),
}

impl serde::Serialize for Error {
    fn serialize<S>(&self, serializer: S) -> Result<S::Ok, S::Error>
    where
        S: serde::ser::Serializer,
    {
        serializer.serialize_str(self.to_string().as_ref())
    }
}
