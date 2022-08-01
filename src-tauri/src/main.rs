#![windows_subsystem = "windows"]

use tauri::Manager;
use tauri::{CustomMenuItem, SystemTray, SystemTrayEvent, SystemTrayMenu, SystemTrayMenuItem};
use tauri_plugin_sql::TauriSql;

fn main() {
    let show = CustomMenuItem::new("show".to_string(), "Show Lazap");
    let quit = CustomMenuItem::new("quit".to_string(), "Quit Lazap");
    let tray_menu = SystemTrayMenu::new()
        .add_item(show)
        .add_native_item(SystemTrayMenuItem::Separator)
        .add_item(quit);
    let tray = SystemTray::new().with_menu(tray_menu);

    if cfg!(windows) {
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
            .invoke_handler(tauri::generate_handler![run_game, parse])
            .setup(|app| {
                let window = app.get_window(&"main").unwrap();
                window_shadows::set_shadow(&window, true).expect("Unsupported platform!");
                Ok(())
            })
            .run(tauri::generate_context!())
            .expect("error while running lazap");
    } else if cfg!(unix) {
        tauri::Builder::default()
            .plugin(TauriSql::default())
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
            .invoke_handler(tauri::generate_handler![run_game, parse])
            .run(tauri::generate_context!())
            .expect("error while running lazap");
    } else if cfg!(target_os = "macos") {
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
            .invoke_handler(tauri::generate_handler![run_game, parse])
            .run(tauri::generate_context!())
            .expect("error while running lazap");
    }
}

#[tauri::command]
async fn run_game(exec: String, args: String) {
    let child = std::process::Command::new(exec)
        .arg(args)
        .spawn()
        .expect("failed to run");
    let _output = child.wait_with_output().expect("failed to wait on child");
}

use html_parser::Dom;
#[tauri::command]
async fn parse(value: &str) -> Result<String, Error> {
    let lol = Dom::parse(value)?.to_json_pretty()?;
    Ok(lol)
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