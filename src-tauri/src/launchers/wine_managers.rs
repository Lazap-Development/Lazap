use tauri::api::path;
use tokio_rusqlite::Connection;

use crate::{
    launchers::{is_installed, GameObject},
    modules::banners,
    operations::custom_fs::d_f_exists,
};

pub async fn get_installed_games() -> Vec<GameObject> {
    let mut all_games: Vec<GameObject> = Vec::new();

    if let Some(got_something) = get_lutris_games().await {
        for game in got_something {
            all_games.push(game);
        }
    }

    return all_games;
}

async fn get_lutris_games() -> Option<Vec<GameObject>> {
    let mut all_lutris_games = Vec::new();

    if !is_installed("lutris", "net.lutris.Lutris") {
        return None;
    }

    let launcher_location: String = path::data_dir()
        .unwrap()
        .into_os_string()
        .into_string()
        .unwrap()
        + "/lutris";

    if !d_f_exists(&launcher_location).await.unwrap_or(false) {
        return None;
    }

    let db_path: String = format!("{}/pga.db", launcher_location);

    if !d_f_exists(&db_path).await.unwrap() {
        return None;
    }

    struct Game {
        display_name: String,
        game_id: String,
        launch_id: i32,
        executable: String,
        location: String,
        size: i64,
    }

    let conn = Connection::open(&db_path).await.unwrap();

    let game_iter = conn
        .call(|conn| {
            let mut stmt = conn.prepare("SELECT * FROM games")?;
            let people = stmt
                .query_map([], |row| {
                    Ok(Game {
                        display_name: row.get(1).unwrap_or_default(),
                        game_id: row.get(3).unwrap_or_default(),
                        launch_id: row.get(0).unwrap_or_default(),
                        executable: row.get(4).unwrap_or_default(),
                        location: row.get(9).unwrap_or_default(),
                        size: row.get(14).unwrap_or_default(),
                    })
                })
                .unwrap()
                .collect::<std::result::Result<Vec<Game>, rusqlite::Error>>();

            Ok(people)
        })
        .await
        .unwrap();

    for game in game_iter.unwrap() {
        let banner = banners::get_banner(&game.display_name, &game.game_id, "Lutris", "").await;
        all_lutris_games.push(GameObject::new(
            banner,
            game.executable,
            game.location,
            game.display_name,
            game.game_id,
            game.launch_id.to_string(),
            game.size,
            String::new(),
            "Lutris".to_string(),
            vec![],
        ))
    }
    return Some(all_lutris_games);
}

// TO-DO! Gather bottles games just like lutris (bottles is a wine manager)
// async fn get_bottles_games() -> Option<Vec<GameObject>> {

// }
