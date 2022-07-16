#![windows_subsystem = "windows"]

use tauri::Manager;
use tauri::{CustomMenuItem, SystemTrayMenu, SystemTrayMenuItem, SystemTray, SystemTrayEvent};

fn main() {
    /* 
     * Tray code
    */
    let quit = CustomMenuItem::new("quit".to_string(), "Quit");
    let hide = CustomMenuItem::new("hide".to_string(), "Hide");
    let show = CustomMenuItem::new("show".to_string(), "Show");
    let tray_menu = SystemTrayMenu::new()
    .add_item(quit)
    .add_native_item(SystemTrayMenuItem::Separator)
    .add_item(hide)
    .add_item(show);
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
                  println!("system tray received a left click");
                }
                SystemTrayEvent::RightClick {
                  position: _,
                  size: _,
                  ..
                } => {
                  println!("system tray received a right click");
                }
                SystemTrayEvent::DoubleClick {
                  position: _,
                  size: _,
                  ..
                } => {
                  println!("system tray received a double click");
                }
                SystemTrayEvent::MenuItemClick { id, .. } => {
                  match id.as_str() {
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
                  }
                }
                _ => {}
              })
            .invoke_handler(tauri::generate_handler![run_game])
            .setup(|app| {
                let window = app.get_window(&"main").unwrap();
                window_shadows::set_shadow(&window, true).expect("Unsupported platform!");
                Ok(())
            })
            .run(tauri::generate_context!())
            .expect("error while running lazap");
    } else if cfg!(unix) {
        tauri::Builder::default()
            .system_tray(tray)
            .on_system_tray_event(|app, event| match event {
                SystemTrayEvent::LeftClick {
                  position: _,
                  size: _,
                  ..
                } => {
                  println!("system tray received a left click");
                }
                SystemTrayEvent::RightClick {
                  position: _,
                  size: _,
                  ..
                } => {
                  println!("system tray received a right click");
                }
                SystemTrayEvent::DoubleClick {
                  position: _,
                  size: _,
                  ..
                } => {
                  println!("system tray received a double click");
                }
                SystemTrayEvent::MenuItemClick { id, .. } => {
                  match id.as_str() {
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
                  }
                }
                _ => {}
              })
            .invoke_handler(tauri::generate_handler![run_game])
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
                  println!("system tray received a left click");
                }
                SystemTrayEvent::RightClick {
                  position: _,
                  size: _,
                  ..
                } => {
                  println!("system tray received a right click");
                }
                SystemTrayEvent::DoubleClick {
                  position: _,
                  size: _,
                  ..
                } => {
                  println!("system tray received a double click");
                }
                SystemTrayEvent::MenuItemClick { id, .. } => {
                  match id.as_str() {
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
                  }
                }
                _ => {}
              })
            .invoke_handler(tauri::generate_handler![run_game])
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
