use std::fs;
use std::path::Path;

use crate::Error;

#[tauri::command]
pub fn read_file(file_path: String) -> Result<String, Error> {
    println!("{}", file_path);
    Ok(fs::read_to_string(file_path).unwrap())
}

#[tauri::command]
pub async fn write_file(file_path: String, file_content: String) {
    fs::write(file_path, file_content).expect("Unable to write file.");
}

#[tauri::command]
pub async fn write_binary_file(file_path: String, file_content: Vec<u8>) {
    fs::write(file_path, file_content).expect("Unable to write file.");
}

#[tauri::command]
pub async fn d_f_exists(path: &str) -> Result<bool, Error> {
    Ok(Path::new(&path).exists())
}

#[tauri::command]
pub async fn rename_file(from: String, to: String) {
    fs::rename(from, to).expect("Unable to rename file.");
}

#[tauri::command]
pub async fn remove_file(file_path: String) {
    fs::remove_file(file_path).expect("Unable to remove file.");
}

#[tauri::command]
pub async fn read_dir(dir_path: &str) -> Result<Vec<String>, Error> {
    let mut file_list = Vec::new();
    for entry in fs::read_dir(dir_path).unwrap() {
        let entry = entry.unwrap();
        let path = entry.path();

        file_list.push(
            Path::new(&path.display().to_string())
                .file_name()
                .unwrap()
                .to_os_string()
                .into_string()
                .unwrap(),
        );
    }
    Ok(file_list)
}
