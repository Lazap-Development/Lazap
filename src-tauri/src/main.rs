#![windows_subsystem = "windows"]

use html_parser::Dom;
use sysinfo::{CpuExt, System, SystemExt};
use tauri::Manager;
use tauri::{CustomMenuItem, SystemTray, SystemTrayEvent, SystemTrayMenu, SystemTrayMenuItem};

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
        .system_tray(tray)
        .plugin(tauri_plugin_fs_extra::FsExtra::default())
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
            sysusername,
            get_sys_info
        ])
        .setup(|app| {
            let window = app.get_window(&"main").unwrap();
            window_shadows::set_shadow(&window, true).expect("Unsupported platform!");
            Ok(())
        })
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
        .plugin(tauri_plugin_sql::TauriSql::default())
        .plugin(tauri_plugin_fs_extra::FsExtra::default())
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
            sysusername,
            get_sys_info
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
            sysusername,
            get_sys_info
        ])
        .setup(|app| {
            let window = app.get_window(&"main").unwrap();
            window_shadows::set_shadow(&window, true).expect("Unsupported platform!");
            Ok(())
        })
        .run(tauri::generate_context!())
        .expect("error while running lazap");
}

#[tauri::command]
async fn parse(value: &str) -> Result<String, Error> {
    Ok(Dom::parse(value)?.to_json_pretty()?)
}

#[tauri::command]
async fn sysusername() -> Result<String, Error> {
    Ok(whoami::username())
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
async fn get_sys_info(
    is_memory: bool,
    is_cpu: bool,
    is_systemname: bool,
    is_systemkernel: bool,
    is_systemhost: bool,
) -> Result<String, Error> {
    let mut sys = System::new_all();
    sys.refresh_all();

    if is_memory == true {
        // Rate = 1048576 Byte is 1MiB
        let rate: i64 = 1048576;

        let data_used_mem: i64 = sys.used_memory().try_into().unwrap();
        let data_all_mem: i64 = sys.total_memory().try_into().unwrap();
        let converted_used_mem = data_used_mem / rate;
        let converted_all_mem = data_all_mem / rate;
        return Ok(converted_used_mem.to_string()
            + " MiB"
            + " / "
            + &converted_all_mem.to_string()
            + " MiB");
    }

    if is_cpu == true {
        for cpu in sys.cpus() {
            return Ok(cpu.brand().to_string());
        }
    }

    if is_systemname == true {
        return Ok(sys.name().unwrap().to_string());
    }

    if is_systemkernel == true {
        return Ok(sys.kernel_version().unwrap().to_string());
    }

    if is_systemhost == true {
        return Ok(sys.host_name().unwrap().to_string());
    }

    Ok("".to_string())
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
