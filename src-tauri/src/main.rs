#![windows_subsystem = "windows"]

use tauri::Manager;
use tauri::{CustomMenuItem, SystemTrayMenu, SystemTrayMenuItem, SystemTray, SystemTrayEvent};
use tauri_plugin_sql::TauriSql;
use discord_presence::{Client, Event};

fn main() {
    /* 
     * Tray code
    */
    // Get our main status message
    let state_message = "sheeesh";
    // Create the client
    let mut drpc = Client::new(997900605072887860);
    // Register event handlers with the corresponding methods
    drpc.on_event(Event::Ready, |_ctx| {
        println!("READY!");
    });
    // Start up the client connection, so that we can actually send and receive stuff
    drpc.start();
    // Set the activity
    drpc.set_activity(|act| act.state(state_message).assets(|ass| ass
      .large_image("lazap").large_text("xd")
    ));

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
            .plugin(TauriSql::default())
            .system_tray(tray)
            .on_system_tray_event(|app, event| match event {
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
            .plugin(TauriSql::default())
            .system_tray(tray)
            .on_system_tray_event(|app, event| match event {
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
            .plugin(TauriSql::default())
            .system_tray(tray)
            .on_system_tray_event(|app, event| match event {
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
