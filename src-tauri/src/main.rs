fn main() {
    tauri::Builder::default()
        .invoke_handler(tauri::generate_handler![run_game])
        .run(tauri::generate_context!())
        .expect("failed to run app");
}

#[tauri::command]
async fn run_game(exec: String, args: String) {
    let child = std::process::Command::new(exec)
        .arg(args)
        .spawn()
        .expect("failed to run");
    let _output = child.wait_with_output().expect("failed to wait on child");
}
