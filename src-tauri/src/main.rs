use tauri::Manager;

fn main() {
    if cfg!(windows) {
        println!("\n OS: [\x1b[34;1mWINDOWS\x1b[0m]");

        tauri::Builder::default()
            .invoke_handler(tauri::generate_handler![run_game])
            .setup(|app| {
                let window = app.get_window(&"main").unwrap();
                window_shadows::set_shadow(&window, true).expect("Unsupported platform!");
                Ok(())
            })
            .run(tauri::generate_context!())
            .expect("error while running lazap");
    } else if cfg!(unix) {
        println!("\n OS: [\x1b[32;1mLINUX\x1b[0m]");

        tauri::Builder::default()
            .invoke_handler(tauri::generate_handler![run_game])
            .run(tauri::generate_context!())
            .expect("error while running lazap");
    } else if cfg!(target_os = "macos") {
        println!("\n OS: [\x1b[1;35mMacOS\x1b[0m]");

        tauri::Builder::default()
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
