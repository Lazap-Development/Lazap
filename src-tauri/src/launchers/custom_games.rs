use crate::{
    launchers::GameObject,
    operations::{
        custom_fs::{d_f_exists, read_file, rename_file, write_file},
        misc::sha256,
    },
    CONFIG_DIR,
};
pub async fn get_installed_games() -> Vec<GameObject> {
    let games_data = read_file(format!(
        "{}/cache/games/data.json",
        CONFIG_DIR.lock().unwrap()
    ))
    .unwrap();

    let games: Vec<GameObject> = serde_json::from_str(&games_data).unwrap();
    let custom_games = games
        .into_iter()
        .filter(|x| x.launcher_name == "CustomGame");
    return custom_games.collect::<Vec<GameObject>>();
}

#[tauri::command]
pub async fn add_custom_game(location: String, display_name: String) {
    let mut obj: GameObject = GameObject::new(
        "".to_string(),
        "".to_string(),
        "".to_string(),
        display_name,
        "CustomGame".to_string(),
        "".to_string(),
        0,
        "".to_string(),
        "CustomGame".to_string(),
        vec![],
    );
    obj.executable = location
        .split("\\")
        .collect::<Vec<_>>()
        .into_iter()
        .rev()
        .collect::<Vec<_>>()[0]
        .to_string();
    obj.location = location
        .split("\\")
        .collect::<Vec<_>>()
        .into_iter()
        .filter(|x| x.to_string() != obj.executable)
        .collect::<Vec<_>>()
        .join("\\");

    let newbannerpath: String = format!(
        "{}/cache/games/banners/newcustombanner.png",
        CONFIG_DIR.lock().unwrap()
    );
    if d_f_exists(&newbannerpath)
        .await
        .expect("Something went wrong")
    {
        let bannername = sha256(obj.display_name.replace(" ", "_")).await.unwrap();
        let _rename = rename_file(
            newbannerpath.clone(),
            format!(
                "{}/cache/games/banners/{}",
                CONFIG_DIR.lock().unwrap(),
                bannername.clone()
            ),
        )
        .await;
        obj.banner_path = format!(
            "{}/cache/games/banners/{}",
            CONFIG_DIR.lock().unwrap(),
            bannername
        );
    }
    let mut games_data: Vec<GameObject> = serde_json::from_str(
        &read_file(format!(
            "{}/cache/games/data.json",
            CONFIG_DIR.lock().unwrap()
        ))
        .unwrap(),
    )
    .unwrap();
    games_data.push(obj);

    write_file(
        format!("{}/cache/games/data.json", CONFIG_DIR.lock().unwrap()),
        serde_json::to_string(&games_data).unwrap(),
    )
    .await;
}
