#![cfg_attr(
    all(not(debug_assertions), target_os = "windows"),
    windows_subsystem = "windows"
)]

mod addons;
mod launchers;
mod modules;
mod operations;

use declarative_discord_rich_presence::DeclarativeDiscordIpcClient;
use std::sync::Mutex;
use tauri::{
    CustomMenuItem, Manager, SystemTray, SystemTrayEvent, SystemTrayMenu, SystemTrayMenuItem,
};

static CONFIG_DIR: Mutex<String> = Mutex::new(String::new());

#[derive(Clone, serde::Serialize)]
struct Payload {
    args: Vec<String>,
    cwd: String,
}

#[cfg(target_os = "windows")]
fn main() {
    let show = CustomMenuItem::new("show".to_string(), "Show Lazap");
    let quit = CustomMenuItem::new("quit".to_string(), "Quit Lazap");
    let tray_menu = SystemTrayMenu::new()
        .add_item(show)
        .add_native_item(SystemTrayMenuItem::Separator)
        .add_item(quit);
    let tray = SystemTray::new().with_menu(tray_menu);
    tauri::Builder::default()
        .on_window_event(move |event| match event.event() {
            tauri::WindowEvent::Destroyed => {
                std::process::exit(0);
            }
            _ => {}
        })
        .setup(|app| {
            *CONFIG_DIR.lock().unwrap() = app
                .path_resolver()
                .app_config_dir()
                .unwrap_or(std::path::PathBuf::new())
                .to_string_lossy()
                .to_string();

            modules::storage::init_storage().expect("Failed to init storage fn.");

            let window = app.get_window(&"main").unwrap();
            window_vibrancy::apply_acrylic(&window, Some((0, 0, 0, 25)))
                .expect("Unsupported platform! 'apply_acrylic' is only supported on Windows 10/11");

            window_shadows::set_shadow(&window, true).expect("Unsupported platform!");
            let client = DeclarativeDiscordIpcClient::new("932504287337148417");
            app.manage(client);
            modules::storage::launcherdata_threads(app.get_window("main").unwrap())
                .expect("Failed to init storage misc fn.");
            Ok(())
        })
        .plugin(tauri_plugin_single_instance::init(|app, argv, cwd| {
            println!("{}, {argv:?}, {cwd}", app.package_info().name);

            let window = app.get_window("main").unwrap();

            if !window.is_visible().unwrap() {
                window.show().unwrap()
            }
            app.emit_all("single-instance", Payload { args: argv, cwd })
                .unwrap();
        }))
        .plugin(tauri_plugin_autostart::init(
            tauri_plugin_autostart::MacosLauncher::LaunchAgent,
            Some(vec![]),
        ))
        .system_tray(tray)
        .on_system_tray_event(|app, event| match event {
            SystemTrayEvent::LeftClick {
                position: _,
                size: _,
                ..
            } => {
                let window = app.get_window("main").unwrap();
                window.show().unwrap();
            }
            SystemTrayEvent::MenuItemClick { id, .. } => match id.as_str() {
                "quit" => {
                    std::process::exit(0);
                }
                "hide" => {
                    let window = app.get_window("main").unwrap();
                    window.hide().unwrap();
                }
                "show" => {
                    let window = app.get_window("main").unwrap();
                    window.show().unwrap();
                }
                _ => {}
            },
            _ => {}
        })
        .invoke_handler(tauri::generate_handler![
            operations::misc::get_sys_info,
            operations::discord_rpc::set_rpc_activity,
            operations::discord_rpc::disable_rpc,
            operations::misc::show_window,
            operations::custom_fs::read_file,
            operations::custom_fs::write_file,
            operations::custom_fs::d_f_exists,
            operations::custom_fs::read_dir,
            operations::custom_fs::write_binary_file,
            operations::custom_fs::rename_file,
            operations::custom_fs::remove_file,
            operations::misc::sha256,
            // Storage Module
            modules::storage::launcherdata_threads_x,
            // Launchers
            launchers::fetch_installed_games,
            launchers::handle_launch,
            launchers::custom_games::add_custom_game,
            // Spotify Addon
            addons::spotify::spotify_login,
            addons::spotify::spotify_connect,
            addons::spotify::spotify_toggle_playback,
            addons::spotify::spotify_forward,
            addons::spotify::spotify_backward,
            addons::spotify::spotify_info,
            addons::spotify::spotify_remove_token,
        ])
        .run(tauri::generate_context!())
        .expect("error while running lazap");
}

#[cfg(target_os = "linux")]
fn main() {
    let show = CustomMenuItem::new("show".to_string(), "Show Lazap");
    let quit = CustomMenuItem::new("quit".to_string(), "Quit Lazap");
    let tray_menu = SystemTrayMenu::new()
        .add_item(show)
        .add_native_item(SystemTrayMenuItem::Separator)
        .add_item(quit);
    let tray = SystemTray::new().with_menu(tray_menu);
    tauri::Builder::default()
        .on_window_event(move |event| match event.event() {
            tauri::WindowEvent::Destroyed => {
                std::process::exit(0);
            }
            _ => {}
        })
        .setup(|app| {
            *CONFIG_DIR.lock().unwrap() = app
                .path_resolver()
                .app_config_dir()
                .unwrap_or(std::path::PathBuf::new())
                .to_string_lossy()
                .to_string();

            modules::storage::init_storage().expect("Failed to init storage fn.");

            let client = DeclarativeDiscordIpcClient::new("932504287337148417");
            app.manage(client);
            modules::storage::launcherdata_threads(app.get_window("main").unwrap())
                .expect("Failed to init storage misc fn.");
            Ok(())
        })
        .plugin(tauri_plugin_single_instance::init(|app, argv, cwd| {
            println!("{}, {argv:?}, {cwd}", app.package_info().name);

            let window = app.get_window("main").unwrap();

            if !window.is_visible().unwrap() {
                window.show().unwrap()
            }

            app.emit_all("single-instance", Payload { args: argv, cwd })
                .unwrap();
        }))
        .plugin(tauri_plugin_autostart::init(
            tauri_plugin_autostart::MacosLauncher::LaunchAgent,
            Some(vec![]),
        ))
        .system_tray(tray)
        .on_system_tray_event(|app, event| match event {
            SystemTrayEvent::LeftClick {
                position: _,
                size: _,
                ..
            } => {
                let window = app.get_window("main").unwrap();
                window.show().unwrap();
            }
            SystemTrayEvent::MenuItemClick { id, .. } => match id.as_str() {
                "quit" => {
                    std::process::exit(0);
                }
                "hide" => {
                    let window = app.get_window("main").unwrap();
                    window.hide().unwrap();
                }
                "show" => {
                    let window = app.get_window("main").unwrap();
                    window.show().unwrap();
                }
                _ => {}
            },
            _ => {}
        })
        .invoke_handler(tauri::generate_handler![
            operations::misc::get_sys_info,
            operations::discord_rpc::set_rpc_activity,
            operations::discord_rpc::disable_rpc,
            operations::misc::show_window,
            operations::custom_fs::read_file,
            operations::custom_fs::write_file,
            operations::custom_fs::d_f_exists,
            operations::custom_fs::read_dir,
            operations::custom_fs::write_binary_file,
            operations::custom_fs::rename_file,
            operations::custom_fs::remove_file,
            operations::misc::sha256,
            // Storage Module
            modules::storage::launcherdata_threads_x,
            // Launchers
            launchers::fetch_installed_games,
            launchers::handle_launch,
            launchers::custom_games::add_custom_game,
            // Spotify Addon
            addons::spotify::spotify_login,
            addons::spotify::spotify_connect,
            addons::spotify::spotify_toggle_playback,
            addons::spotify::spotify_forward,
            addons::spotify::spotify_backward,
            addons::spotify::spotify_info,
            addons::spotify::spotify_remove_token,
        ])
        .run(tauri::generate_context!())
        .expect("error while running lazap");
}

#[cfg(target_os = "macos")]
fn main() {
    let show = CustomMenuItem::new("show".to_string(), "Show Lazap");
    let quit = CustomMenuItem::new("quit".to_string(), "Quit Lazap");
    let tray_menu = SystemTrayMenu::new()
        .add_item(show)
        .add_native_item(SystemTrayMenuItem::Separator)
        .add_item(quit);
    let tray = SystemTray::new().with_menu(tray_menu);
    tauri::Builder::default()
        .on_window_event(move |event| match event.event() {
            tauri::WindowEvent::Destroyed => {
                std::process::exit(0);
            }
            _ => {}
        })
        .setup(|app| {
            *CONFIG_DIR.lock().unwrap() = app
                .path_resolver()
                .app_config_dir()
                .unwrap_or(std::path::PathBuf::new())
                .to_string_lossy()
                .to_string();

            modules::storage::init_storage().expect("Failed to init storage fn.");

            let window = app.get_window("main").unwrap();
            window_vibrancy::apply_vibrancy(
                &window,
                window_vibrancy::NSVisualEffectMaterial::HudWindow,
                None,
                Some(15.0),
            )
            .expect("Unsupported platform! 'apply_vibrancy' is only supported on macOS");

            window_shadows::set_shadow(&window, true).expect("Unsupported platform!");

            let client = DeclarativeDiscordIpcClient::new("932504287337148417");
            app.manage(client);
            modules::storage::launcherdata_threads(app.get_window("main").unwrap())
                .expect("Failed to init storage misc fn.");
            Ok(())
        })
        .plugin(tauri_plugin_single_instance::init(|app, argv, cwd| {
            println!("{}, {argv:?}, {cwd}", app.package_info().name);

            let window = app.get_window("main").unwrap();

            if !window.is_visible().unwrap() {
                window.show().unwrap()
            }

            app.emit_all("single-instance", Payload { args: argv, cwd })
                .unwrap();
        }))
        .plugin(tauri_plugin_autostart::init(
            tauri_plugin_autostart::MacosLauncher::LaunchAgent,
            Some(vec![]),
        ))
        .system_tray(tray)
        .on_system_tray_event(|app, event| match event {
            SystemTrayEvent::LeftClick {
                position: _,
                size: _,
                ..
            } => {
                let window = app.get_window("main").unwrap();
                window.show().unwrap();
            }
            SystemTrayEvent::MenuItemClick { id, .. } => match id.as_str() {
                "quit" => {
                    std::process::exit(0);
                }
                "hide" => {
                    let window = app.get_window("main").unwrap();
                    window.hide().unwrap();
                }
                "show" => {
                    let window = app.get_window("main").unwrap();
                    window.show().unwrap();
                }
                _ => {}
            },
            _ => {}
        })
        .invoke_handler(tauri::generate_handler![
            operations::misc::get_sys_info,
            operations::discord_rpc::set_rpc_activity,
            operations::discord_rpc::disable_rpc,
            operations::misc::show_window,
            operations::custom_fs::read_file,
            operations::custom_fs::write_file,
            operations::custom_fs::d_f_exists,
            operations::custom_fs::read_dir,
            operations::custom_fs::write_binary_file,
            operations::custom_fs::rename_file,
            operations::custom_fs::remove_file,
            operations::misc::sha256,
            // Storage Module
            modules::storage::launcherdata_threads_x,
            // Launchers
            launchers::fetch_installed_games,
            launchers::handle_launch,
            launchers::custom_games::add_custom_game,
            // Spotify Addon
            addons::spotify::spotify_login,
            addons::spotify::spotify_connect,
            addons::spotify::spotify_toggle_playback,
            addons::spotify::spotify_forward,
            addons::spotify::spotify_backward,
            addons::spotify::spotify_info,
            addons::spotify::spotify_remove_token,
        ])
        .run(tauri::generate_context!())
        .expect("error while running lazap");
}

#[derive(Debug, thiserror::Error)]
enum Error {
    #[error(transparent)]
    Io(#[from] std::io::Error),
}

impl serde::Serialize for Error {
    fn serialize<S>(&self, serializer: S) -> Result<S::Ok, S::Error>
    where
        S: serde::ser::Serializer,
    {
        serializer.serialize_str(self.to_string().as_ref())
    }
}
