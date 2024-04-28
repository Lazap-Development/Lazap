use std::{
    fs::File,
    io::{copy, Cursor},
};

use reqwest::header::{ACCEPT, CONTENT_TYPE};
use serde::Deserialize;

use crate::{
    operations::{custom_fs::d_f_exists, misc::sha256},
    CONFIG_DIR,
};

pub async fn get_banner(
    display_name: &str,
    game_id: &str,
    launcher_name: &str,
    url: &str,
) -> String {
    match launcher_name {
        "Steam" => {
            return fetch_banner(
                format!(
                    "https://cdn.akamai.steamstatic.com/steam/apps/{}/library_600x900.jpg",
                    game_id
                ),
                display_name,
            )
            .await;
        }
        "RiotGames" => {
            return fetch_banner(format!("https://valorant-config.fr/wp-content/uploads/2020/05/7d604cf06abf5866f5f3a2fbd0deacf9-200x300.png"), display_name).await;
        }
        "Minecraft" => {
            return fetch_banner(format!("https://i.imgur.com/PJFx3U2.jpg"), display_name).await;
        }
        "FiveM" => {
            return fetch_banner(
                format!("https://logos-world.net/wp-content/uploads/2021/03/FiveM-Symbol.png"),
                display_name,
            )
            .await;
        }
        "Lunar" => {
            return fetch_banner(
                format!(
                    "https://pbs.twimg.com/profile_images/1608698913476812801/uLTLhANK_400x400.jpg"
                ),
                display_name,
            )
            .await;
        }
        "Lutris" => {
            if display_name.replace(" ", "_") == "Epic_Games_Store" {
                return fetch_banner(
                    format!("https://pcper.com/wp-content/uploads/2021/02/epic-games-store.png"),
                    display_name,
                )
                .await;
            }
            if display_name.replace(" ", "_") == "Rockstar_Games_Launcher" {
                return fetch_banner(format!("https://cdn.player.one/sites/player.one/files/2019/08/26/rockstar-games.png"), display_name).await;
            }
            return String::new();
        }
        // "XboxGames" => {
        //     return fetch_banner(url.to_string(), display_name).await;
        // },
        "Osu" => {
            return fetch_banner(format!("https://cdn2.steamgriddb.com/file/sgdb-cdn/grid/a5d7420f9fdc41087377b4d58c5fe94b.png"), display_name).await;
        }
        "EpicGames" => {
            return rawg_fetch_banner(display_name).await;
        }
        "Uplay" => {
            return rawg_fetch_banner(display_name).await;
        }
        "RockstarGames" => {
            return fetch_banner(format!("https://media-rockstargames-com.akamaized.net/rockstargames-newsite/img/global/games/fob/640/{}.jpg", game_id), display_name).await;
        }
        "GOG" => {
            return fetch_banner(url.to_string(), display_name).await;
        }

        _ => return String::new(),
    }
}

async fn fetch_banner(url: String, display_name: &str) -> String {
    let banners_dir = format!("{}/cache/games/banners", CONFIG_DIR.lock().unwrap());

    let dispsha256 = sha256(display_name.replace(" ", "_")).await.unwrap();
    let file_path: String = format!("{}/{}.png", banners_dir, dispsha256);
    if d_f_exists(&file_path).await.unwrap() {
        return file_path;
    }

    let response = reqwest::get(url).await.unwrap();
    let mut file = File::create(&file_path).unwrap();

    let mut content = Cursor::new(response.bytes().await.unwrap());
    copy(&mut content, &mut file).unwrap();

    return file_path;
}

#[derive(Deserialize, Debug)]
struct RAWGApi {
    results: Vec<GameWithinRawg>,
}

#[derive(Deserialize, Debug)]
struct GameWithinRawg {
    background_image: String,
}

async fn rawg_fetch_banner(display_name: &str) -> String {
    let banners_dir = format!("{}/cache/games/banners", CONFIG_DIR.lock().unwrap());

    let dispsha256 = sha256(display_name.replace(" ", "_")).await.unwrap();
    let file_path: String = format!("{}/{}.png", banners_dir, dispsha256);
    if d_f_exists(&file_path).await.unwrap() {
        return file_path;
    }

    let client = reqwest::Client::new();
    let response = client
        .get(format!("https://api.rawg.io/api/games?key=f8854c401fed44b89f4e1e4faa56ccc8&search={}&search_exact&search_precise&stores=11", display_name.replace(" ", "-").replace("_", "-")))
        .header(CONTENT_TYPE, "application/json")
        .header(ACCEPT, "application/json")
        .send()
        .await
        .unwrap();

    match response.status() {
        reqwest::StatusCode::OK => {
            let background_image_url =
                match response.json::<RAWGApi>().await.unwrap().results.get(0) {
                    Some(result) => result.background_image.clone(),
                    None => return String::new(),
                };

            let banner_response = match reqwest::get(&background_image_url).await {
                Ok(response) => response,
                Err(err) => {
                    eprintln!("Error fetching background image: {}", err);
                    return String::new();
                }
            };
            let mut file = File::create(&file_path).unwrap();

            let mut content = Cursor::new(banner_response.bytes().await.unwrap());
            copy(&mut content, &mut file).unwrap();

            return file_path;
        }
        _ => {
            println!("Something unexpected happened... Trace -> init_reqwest");
            return String::new();
        }
    }
}
