use crate::{
    launchers::{
        GameObject,
        LINE_ENDING,
    },
    operations::custom_fs::d_f_exists,
    modules::banners,
};
use serde::{Deserialize, Serialize};
use std::process::Command;
use tokio_rusqlite::Connection;

#[derive(Serialize, Deserialize)]
struct ImageData {
    #[serde(default)]
    background: String,
    icon: String,
    #[serde(rename = "logo2x")]
    logo: String,
}

pub async fn get_installed_games() -> Vec<GameObject> {
    let mut gameobjects = vec![];
    if !d_f_exists("C:\\ProgramData\\GOG.com\\Galaxy\\storage").await.expect("Something went wrong")
    {
        return vec![];
    }
    
    struct InstalledGame {
        product_id: i32,
        installation_path: String,
    }
    struct GameData {
        product_id: i32,
        title: String,
        banner: String,
    }

    let conn = Connection::open("C:\\ProgramData\\GOG.com\\Galaxy\\storage\\galaxy-2.0.db")
        .await
        .unwrap();
    let games = conn
        .call(|conn| {
            let mut stmt = conn.prepare("SELECT * FROM InstalledBaseProducts")?;
            let installed_games = stmt
                .query_map([], |row| {
                    Ok(InstalledGame {
                        product_id: row.get(0).unwrap_or_default(),
                        installation_path: row.get(3).unwrap_or_default(),
                    })
                })
                .unwrap()
                .collect::<std::result::Result<Vec<InstalledGame>, rusqlite::Error>>();
            println!("{}", installed_games.as_deref().unwrap()[0].product_id);
            Ok(installed_games)
        })
        .await
        .unwrap();
    
    let games_data = conn
    .call(|conn| {
        let mut stmt = conn.prepare("SELECT * FROM LimitedDetails")?;
        let data = stmt
        .query_map([], |row| {
            Ok(GameData {
                product_id: row.get(1).unwrap_or_default(),
                title: row.get(5).unwrap_or_default(),
                        banner: row.get(7).unwrap_or_default(),
                    })
                })
                .unwrap()
                .collect::<std::result::Result<Vec<GameData>, rusqlite::Error>>();
            Ok(data)
        })
        .await
        .unwrap()
        .unwrap();

    let cmd = Command::new("cmd")
        .args(&[
            "/C",
            "Reg",
            "Query",
            "HKEY_LOCAL_MACHINE\\SOFTWARE\\WOW6432Node\\GOG.com\\GalaxyClient",
            "/s",
        ])
        .output()
        .expect("failed to execute process.");
    if cmd.stdout.is_empty() {
        return vec![];
    }

    let launcher_location = String::from_utf8_lossy(&cmd.stdout)
        .to_string()
        .split(LINE_ENDING)
        .filter(|x| x.contains("REG_SZ") && x.contains("client"))
        .map(
            |x| x.split("REG_SZ").collect::<Vec<_>>()[1].trim()
        )
        .collect::<Vec<_>>()
        .into_iter()
        .rev()
        .collect::<Vec<_>>().join("\\");
    
    for game in games.unwrap() {
        let product_id = game.product_id;
        let game_data = games_data
            .iter()
            .find(|x| x.product_id == product_id)
            .unwrap();
        let img_data: ImageData = serde_json::from_str(&game_data.banner).unwrap();
        gameobjects.push(GameObject::new(
            banners::get_banner(&game_data.title, &(product_id.to_string()), "GOG", &img_data.icon).await, // Link to the banner not the actual path on system
            "".to_string(),
            game.installation_path.to_string(),
            game_data.title.to_string(),
            product_id.to_string(),
            format!("\"{}\"", launcher_location) + " /command=runGame" + &format!(" /gameId={}", game.product_id) + &format!(" /path=\"{}\"", game.installation_path),
            0,
            "".to_string(),
            "GOG".to_string(),
            vec![],
        ));
    }

    println!("{:?}", gameobjects[0]);

    return gameobjects;
}

// "C:\Program Files (x86)\GOG Galaxy\GalaxyClient.exe" /command=runGame /gameId=1810005965 /path="C:\Program Files (x86)\GOG Galaxy\Games\Roboplant Demo"