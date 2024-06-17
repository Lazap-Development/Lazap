use sha2::{Digest, Sha256};
use sysinfo::{CpuExt, System, SystemExt};
use tauri::Manager;

use crate::Error;

#[tauri::command]
pub async fn show_window(window: tauri::Window) {
    window.get_window("main").unwrap().show().unwrap();
}

#[tauri::command]
pub async fn sha256(content: String) -> Result<String, Error> {
    let mut hasher = Sha256::new();
    hasher.update(content);
    Ok(format!("{:x}", hasher.finalize()))
}

#[tauri::command]
pub async fn get_sys_info() -> Result<String, Error> {
    let mut sys = System::new_all();
    sys.refresh_all();

    // Rate = 1048576 Byte is 1MiB
    let rate: i64 = 1048576;

    // Rate = 1073741824 Byte is 1GB
    let rate2: u64 = 1000000000;

    use sysinfo::DiskExt;

    let data_used_mem: i64 = sys.used_memory().try_into().unwrap();
    let data_all_mem: i64 = sys.total_memory().try_into().unwrap();

    let mut data_used_disk: u64 = 0;
    let mut data_all_disk: u64 = 0;
    for disk in sys.disks() {
        data_used_disk += disk.total_space() - disk.available_space();
        data_all_disk += disk.total_space();
    }

    let converted_used_mem = data_used_mem / rate;
    let converted_all_mem = data_all_mem / rate;
    let converted_used_disk = data_used_disk / rate2;
    let converted_all_disk = data_all_disk / rate2;

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
        disk: String,
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
        disk: converted_used_disk.to_string()
            + " GB"
            + " / "
            + &converted_all_disk.to_string()
            + " GB",
    };

    Ok(format!(
        r#"{{"memory": "{}", "cpu": "{}", "system_name": "{}", "system_kernel": "{}", "system_host": "{}", "disk": "{}"}}"#,
        sys_data.memory,
        sys_data.cpu,
        sys_data.system_name,
        sys_data.system_kernel,
        sys_data.system_host,
        sys_data.disk
    ))
}
