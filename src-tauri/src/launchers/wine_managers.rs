use rusqlite::Connection;
use tauri::api::path;

use crate::launchers::GameObject;
use crate::operations::custom_fs::d_f_exists;

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

    let launcher_location: String = path::home_dir()
        .unwrap()
        .into_os_string()
        .into_string()
        .unwrap()
        + ".local/share/lutris";

    match d_f_exists(&launcher_location).await {
        Ok(_) => {
            let db_path: String = path::data_dir()
                .unwrap()
                .into_os_string()
                .into_string()
                .unwrap()
                + "/lutris/pga.db";

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

            let connection = match Connection::open(&db_path) {
                Ok(conn) => conn,
                Err(_) => return None,
            };
            let mut statement = connection.prepare("SELECT * FROM games").unwrap();
            let game_iter = statement.query_map([], |row| {
                Ok(Game {
                    display_name: row.get(1)?,
                    game_id: row.get(3).unwrap_or_default(),
                    launch_id: row.get(0).unwrap_or_default(),
                    executable: row.get(4).unwrap_or_default(),
                    location: row.get(9).unwrap_or_default(),
                    size: row.get(14).unwrap_or_default(),
                })
            });

            for game in game_iter.unwrap() {
                let game_unwrapped = game.unwrap();
                all_lutris_games.push(GameObject::new(
                    game_unwrapped.executable,
                    game_unwrapped.location,
                    game_unwrapped.display_name,
                    game_unwrapped.game_id,
                    game_unwrapped.launch_id,
                    game_unwrapped.size,
                    "".to_string(),
                    "Lutris".to_string(),
                    vec![],
                ))
            }

            return Some(all_lutris_games);
        }
        Err(_) => None,
    }
}

// TO-DO! Gather bottles games just like lutris (bottles is a wine manager)
// async fn get_bottles_games() -> Option<Vec<GameObject>> {

// }
