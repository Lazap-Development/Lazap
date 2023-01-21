#![windows_subsystem = "windows"]

use declarative_discord_rich_presence::activity::Activity;
use declarative_discord_rich_presence::activity::Assets;
use declarative_discord_rich_presence::activity::Timestamps;
use declarative_discord_rich_presence::DeclarativeDiscordIpcClient;
use html_parser::Dom;
use std::fs;
use std::io::Write;
use std::path::Path;
use sysinfo::{CpuExt, System, SystemExt};
use tauri::{
    CustomMenuItem, Manager, State, SystemTray, SystemTrayEvent, SystemTrayMenu, SystemTrayMenuItem,
};
const DISCORD_RPC_CLIENT_ID: &str = "1058022807373627462";

fn init_storage() -> Result<(), std::io::Error> {
    let base_config_path = tauri::api::path::app_config_dir(&tauri::Config::default())
        .unwrap()
        .display()
        .to_string()
        + "com.lazap.config";
    let base_config_cache_path = tauri::api::path::app_config_dir(&tauri::Config::default())
        .unwrap()
        .display()
        .to_string()
        + "com.lazap.config/cache";
    let base_config_cache_games_path = tauri::api::path::app_config_dir(&tauri::Config::default())
        .unwrap()
        .display()
        .to_string()
        + "com.lazap.config/cache/games";
    let base_config_cache_games_banners_path =
        tauri::api::path::app_config_dir(&tauri::Config::default())
            .unwrap()
            .display()
            .to_string()
            + "com.lazap.config/cache/user";
    let base_config_cache_user_path = tauri::api::path::app_config_dir(&tauri::Config::default())
        .unwrap()
        .display()
        .to_string()
        + "com.lazap.config/cache/games/banners";
    let base_config_ld_file = tauri::api::path::app_config_dir(&tauri::Config::default())
        .unwrap()
        .display()
        .to_string()
        + "com.lazap.config/LauncherData.json";
    let base_config_cache_user_data_file =
        tauri::api::path::app_config_dir(&tauri::Config::default())
            .unwrap()
            .display()
            .to_string()
            + "com.lazap.config/cache/user/data.json";
    let base_config_cache_game_data_file =
        tauri::api::path::app_config_dir(&tauri::Config::default())
            .unwrap()
            .display()
            .to_string()
            + "com.lazap.config/cache/games/data.json";

    if !Path::new(&base_config_path).exists() {
        fs::create_dir_all(base_config_path).expect("Failed to create dir.");
    }
    if !Path::new(&base_config_cache_path).exists() {
        fs::create_dir_all(base_config_cache_path).expect("Failed to create dir.");
    }
    if !Path::new(&base_config_cache_games_path).exists() {
        fs::create_dir_all(base_config_cache_games_path).expect("Failed to create dir.");
    }
    if !Path::new(&base_config_cache_games_banners_path).exists() {
        fs::create_dir_all(base_config_cache_games_banners_path).expect("Failed to create dir.");
    }
    if !Path::new(&base_config_cache_user_path).exists() {
        fs::create_dir_all(base_config_cache_user_path).expect("Failed to create dir.");
    }
    if !Path::new(&base_config_ld_file).exists() {
        let mut file = fs::File::create(base_config_ld_file)?;
        writeln!(file, "{{ \"enable_rpc\": true, \"launch_on_startup\": false, \"skip_login\": false, \"tray_min_launch\": true, \"tray_min_quit\": false, \"check_for_updates\": false, \"accent_color\": \"#7934FA\" }}")?;
    }
    if !Path::new(&base_config_cache_user_data_file).exists() {
        let mut file = fs::File::create(base_config_cache_user_data_file)?;
        writeln!(file, "{{  \"username\": \"{}\" }}", whoami::username())?;
    }
    if !Path::new(&base_config_cache_game_data_file).exists() {
        let mut file = fs::File::create(base_config_cache_game_data_file)?;
        writeln!(file, "[]")?;
    }

    Ok(())
}

#[cfg(target_os = "windows")]
fn main() {
    init_storage().expect("Failed to init storage fn.");
    let show = CustomMenuItem::new("show".to_string(), "Show Lazap");
    let quit = CustomMenuItem::new("quit".to_string(), "Quit Lazap");
    let tray_menu = SystemTrayMenu::new()
        .add_item(show)
        .add_native_item(SystemTrayMenuItem::Separator)
        .add_item(quit);
    let tray = SystemTray::new().with_menu(tray_menu);
    tauri::Builder::default()
        .setup(|app| {
            let window = app.get_window(&"main").unwrap();
            window_shadows::set_shadow(&window, true).expect("Unsupported platform!");
            let client = DeclarativeDiscordIpcClient::new(DISCORD_RPC_CLIENT_ID);
            app.manage(client);
            Ok(())
        })
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
            launch_game,
            parse,
            get_sys_info,
            set_activity,
            disable_rpc,
            show_window,
            read_file,
            write_file,
            d_f_exists,
            read_dir,
            write_binary_file
        ])
        .run(tauri::generate_context!())
        .expect("error while running lazap");
}

#[cfg(target_os = "linux")]
fn main() {
    init_storage().expect("Failed to init storage fn.");
    let show = CustomMenuItem::new("show".to_string(), "Show Lazap");
    let quit = CustomMenuItem::new("quit".to_string(), "Quit Lazap");
    let tray_menu = SystemTrayMenu::new()
        .add_item(show)
        .add_native_item(SystemTrayMenuItem::Separator)
        .add_item(quit);
    let tray = SystemTray::new().with_menu(tray_menu);
    tauri::Builder::default()
        .setup(|app| {
            let client = DeclarativeDiscordIpcClient::new(DISCORD_RPC_CLIENT_ID);
            app.manage(client);
            Ok(())
        })
        .plugin(tauri_plugin_sql::Builder::default().build())
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
            launch_game,
            parse,
            get_sys_info,
            set_activity,
            disable_rpc,
            show_window,
            read_file,
            write_file,
            d_f_exists,
            read_dir,
            write_binary_file
        ])
        .run(tauri::generate_context!())
        .expect("error while running lazap");
}

#[cfg(target_os = "macos")]
fn main() {
    init_storage().expect("Failed to init storage fn.");
    let show = CustomMenuItem::new("show".to_string(), "Show Lazap");
    let quit = CustomMenuItem::new("quit".to_string(), "Quit Lazap");
    let tray_menu = SystemTrayMenu::new()
        .add_item(show)
        .add_native_item(SystemTrayMenuItem::Separator)
        .add_item(quit);
    let tray = SystemTray::new().with_menu(tray_menu);
    tauri::Builder::default()
        .setup(|app| {
            let window = app.get_window(&"main").unwrap();
            window_shadows::set_shadow(&window, true).expect("Unsupported platform!");
            let client = DeclarativeDiscordIpcClient::new(DISCORD_RPC_CLIENT_ID);
            app.manage(client);
            Ok(())
        })
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
            launch_game,
            parse,
            get_sys_info,
            set_activity,
            disable_rpc,
            show_window,
            read_file,
            write_file,
            d_f_exists,
            read_dir,
            write_binary_file
        ])
        .run(tauri::generate_context!())
        .expect("error while running lazap");
}

#[tauri::command]
async fn parse(value: &str) -> Result<String, Error> {
    Ok(Dom::parse(value)?.to_json_pretty()?)
}

#[tauri::command]
async fn launch_game(exec: String, args: String) {
    #[cfg(target_os = "windows")]
    use std::os::windows::process::CommandExt;
    #[cfg(target_os = "windows")]
    let child = std::process::Command::new("cmd")
        .arg(exec)
        .creation_flags(0x00000008)
        .spawn()
        .expect("failed to run");
    #[cfg(target_os = "windows")]
    let _output = child.wait_with_output().expect("failed to wait on child");

    #[cfg(target_os = "linux")]
    let child = std::process::Command::new(exec)
        .arg(args)
        .spawn()
        .expect("failed to run");
    #[cfg(target_os = "linux")]
    let _output = child.wait_with_output().expect("failed to wait on child");
}

#[tauri::command]
fn set_activity(
    client: State<'_, DeclarativeDiscordIpcClient>,
    state: &str,
    details: &str,
    large_image: &str,
    large_text: &str,
    small_image: &str,
    small_text: &str,
    timestamp: i64,
) {
    if let Err(why) = client.set_activity(
        Activity::new()
            .state(state)
            .details(details)
            .assets(
                Assets::new()
                    .large_image(large_image)
                    .large_text(large_text)
                    .small_image(small_image)
                    .small_text(small_text),
            )
            .timestamps(Timestamps::new().start(timestamp)),
    ) {
        println!("failed to set presence: {}", why)
    }
}

#[tauri::command]
fn disable_rpc(client: State<'_, DeclarativeDiscordIpcClient>, enable: bool) {
    if enable {
        client.enable();
    } else {
        client.disable();
    }
}

#[tauri::command]
async fn show_window(window: tauri::Window) {
    window.get_window("main").unwrap().show().unwrap();
}

#[tauri::command]
async fn read_file(file_path: String) -> Result<String, Error>  {
    Ok(fs::read_to_string(file_path).unwrap())
}

#[tauri::command]
async fn write_file(file_path: String, file_content: String) {
    fs::write(file_path, file_content).expect("Unable to write file.");
}

#[tauri::command]
async fn write_binary_file(file_path: String, file_content: Vec::<u8>) {
    fs::write(file_path, file_content).expect("Unable to write file.");
}

#[tauri::command]
async fn d_f_exists(path: String) -> Result<bool, Error> {
    Ok(Path::new(&path).exists())
}

#[tauri::command]
async fn read_dir(dir_path: String) -> Vec<String> {
    let mut file_list = Vec::new();
    for entry in fs::read_dir(dir_path).unwrap() {
        let entry = entry.unwrap();
        let path = entry.path();
        if !path.is_dir() {
            file_list.push(Path::new(&path.display().to_string()).file_name().unwrap().to_os_string().into_string().unwrap());
        }
    }
    file_list
}

#[tauri::command]
async fn get_sys_info() -> Result<String, Error> {
    let mut sys = System::new_all();
    sys.refresh_all();

    // Rate = 1048576 Byte is 1MiB
    let rate: i64 = 1048576;

    let data_used_mem: i64 = sys.used_memory().try_into().unwrap();
    let data_all_mem: i64 = sys.total_memory().try_into().unwrap();
    let converted_used_mem = data_used_mem / rate;
    let converted_all_mem = data_all_mem / rate;

    let mut cpu_info = "";
    for cpu in sys.cpus() {
        cpu_info = cpu.brand();
    }

    struct SysStruct {
        memory: String,
        cpu: String,
        system_name: String,
        system_kernel: String,
        system_host: String,
    }
    let sys_data = SysStruct {
        memory: converted_used_mem.to_string()
            + " MiB"
            + " / "
            + &converted_all_mem.to_string()
            + " MiB",
        cpu: cpu_info.to_string(),
        system_name: sys.name().unwrap().to_string(),
        system_kernel: sys.kernel_version().unwrap().to_string(),
        system_host: sys.host_name().unwrap().to_string(),
    };

    Ok(format!(
        "{{\"memory\": \"{}\", \"cpu\": \"{}\", \"system_name\": \"{}\", \"system_kernel\": \"{}\", \"system_host\": \"{}\"}}",
        sys_data.memory,
        sys_data.cpu,
        sys_data.system_name,
        sys_data.system_kernel,
        sys_data.system_host,
    ))
}

#[derive(Debug, thiserror::Error)]
enum Error {
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
