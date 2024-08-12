use actix_web::rt::net::TcpListener;
use actix_web::{get, web, App, HttpResponse, HttpServer, Responder};
use base64::{
    alphabet,
    engine::{self, general_purpose},
    Engine as _,
};
use lazy_static::lazy_static;
use rand::{self, Rng};
use reqwest::{Client, Url};
use serde::Deserialize;
use std::sync::{Arc, OnceLock};
use tauri::{Manager, Window};
use tokio::sync::Mutex;

use crate::Error;

static SPOTIFY_CLIENT_ID: OnceLock<String> = OnceLock::new();
static SPOTIFY_CLIENT_SECRET: OnceLock<String> = OnceLock::new();
static AVOID_SPAWN: OnceLock<bool> = OnceLock::new();

lazy_static! {
    static ref ACCESS_TOKEN: Arc<Mutex<Option<String>>> = Arc::new(Mutex::new(None));
    static ref EXTERNAL_WINDOW: Arc<Mutex<Option<Window>>> = Arc::new(Mutex::new(None));
}

const CUSTOM_ENGINE: engine::GeneralPurpose =
    engine::GeneralPurpose::new(&alphabet::URL_SAFE, general_purpose::NO_PAD);

#[actix_web::main]
pub async fn main() -> std::io::Result<()> {
    // SPOTIFY_CLIENT_ID = String::new();
    // SPOTIFY_CLIENT_SECRET = String::new();

    if SPOTIFY_CLIENT_ID.get().is_none() || SPOTIFY_CLIENT_SECRET.get().is_none() {
        let spotify_client_id = std::env::var("SPOTIFY_CLIENT_ID").ok();
        let spotify_client_secret = std::env::var("SPOTIFY_CLIENT_SECRET").ok();

        match spotify_client_id {
            Some(id) => {
                SPOTIFY_CLIENT_ID
                    .set(id)
                    .expect("SPOTIFY_CLIENT_ID is not set. HttpServer not started.");
            }
            None => {
                eprintln!("SPOTIFY_CLIENT_ID is not set. HttpServer not started.");
                AVOID_SPAWN
                    .set(true)
                    .expect("Failed to modify AVOID_SPAWN OnceLock.");
                return Ok(());
            }
        }

        match spotify_client_secret {
            Some(secret) => {
                SPOTIFY_CLIENT_SECRET
                    .set(secret)
                    .expect("SPOTIFY_CLIENT_SECRET is not set. HttpServer not started.");
            }
            None => {
                eprintln!("SPOTIFY_CLIENT_SECRET is not set. HttpServer not started.");
                AVOID_SPAWN
                    .set(true)
                    .expect("Failed to modify AVOID_SPAWN OnceLock.");
                return Ok(());
            }
        }
    }

    match TcpListener::bind("localhost:3000").await {
        Ok(_) => {}
        Err(_) => return Ok(()),
    }

    HttpServer::new(|| App::new().service(login).service(callback).service(token))
        .bind(("localhost", 3000))?
        .run()
        .await
}

#[get("/auth/login")]
async fn login() -> impl Responder {
    let scope = "streaming app-remote-control user-read-playback-state user-modify-playback-state user-read-currently-playing";
    let state = gen_rand_string(16);

    let mut auth_query_parameters = Url::parse("https://accounts.spotify.com/authorize/").unwrap();
    auth_query_parameters
        .query_pairs_mut()
        .append_pair("response_type", "code")
        .append_pair("client_id", SPOTIFY_CLIENT_ID.get().unwrap())
        .append_pair("scope", scope)
        .append_pair("redirect_uri", "http://localhost:3000/auth/callback")
        .append_pair("state", &state);

    HttpResponse::Found()
        .append_header(("Location", auth_query_parameters.to_string()))
        .finish()
}

#[derive(Deserialize)]
struct AuthCallbackQuery {
    code: String,
}

#[get("/auth/callback")]
async fn callback(query: web::Query<AuthCallbackQuery>) -> impl Responder {
    let code = &query.code;

    let client = Client::new();
    let _response = client
        .post("https://accounts.spotify.com/api/token")
        .header(
            reqwest::header::AUTHORIZATION,
            format!(
                "Basic {}",
                CUSTOM_ENGINE.encode(format!(
                    "{}:{}",
                    SPOTIFY_CLIENT_ID.get().unwrap(),
                    SPOTIFY_CLIENT_SECRET.get().unwrap()
                ))
            ),
        )
        .header(
            reqwest::header::CONTENT_TYPE,
            "application/x-www-form-urlencoded",
        )
        .form(&[
            ("code", code),
            (
                "redirect_uri",
                &"http://localhost:3000/auth/callback".to_string(),
            ),
            ("grant_type", &"authorization_code".to_string()),
        ])
        .send()
        .await;

    if let Ok(response) = _response {
        if response.status().is_success() {
            let body: serde_json::Value = response.json().await.unwrap();
            let access_token = body["access_token"].as_str().unwrap();

            *ACCESS_TOKEN.lock().await = Some(access_token.to_string());

            if let Some(ref ext_window) = *EXTERNAL_WINDOW.lock().await {
                let window = ext_window.get_window("external").unwrap();
                window.hide().unwrap();
            }

            return HttpResponse::Found().finish();
        } else {
            println!("{}", response.status())
        }
    }

    HttpResponse::InternalServerError().finish()
}

#[get("/auth/token")]
async fn token() -> impl Responder {
    if let Some(ref access_token) = *ACCESS_TOKEN.lock().await {
        HttpResponse::Ok().json(serde_json::json!({
            "access_token": access_token,
        }))
    } else {
        HttpResponse::InternalServerError().finish()
    }
}

fn gen_rand_string(length: usize) -> String {
    let possible = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    let mut rng = rand::thread_rng();
    let text: String = (0..length)
        .map(|_| {
            let index = rng.gen_range(0..possible.len());
            possible.chars().nth(index).unwrap()
        })
        .collect();

    text
}

#[tauri::command]
pub async fn spotify_login(window: tauri::Window) -> Result<(), Error> {
    if *AVOID_SPAWN.get().unwrap_or(&false) {
        return Ok(());
    }

    let ext_window = window.get_window("external").unwrap();
    ext_window
        .eval("window.location.replace('http://localhost:3000/auth/login')")
        .expect("Failed to set page to /auth/login");
    *EXTERNAL_WINDOW.lock().await = Some(window);

    Ok(())
}

#[tauri::command]
pub async fn spotify_connect() -> Result<(), Error> {
    if let Some(ref ext_window) = *EXTERNAL_WINDOW.lock().await {
        let window = ext_window.get_window("external").unwrap();
        window
            .eval("window.location.replace('http://localhost:3000/auth/login')")
            .expect("Failed to set page to /auth/login");
        window.show().unwrap();
    }
    Ok(())
}

#[tauri::command]
pub async fn spotify_toggle_playback() -> Result<bool, Error> {
    if let Some(ref access_token) = *ACCESS_TOKEN.lock().await {
        let mut is_playing_glob: bool = false;

        let client = Client::new();
        let currently_playing = client
            .get("https://api.spotify.com/v1/me/player/currently-playing")
            .header(
                reqwest::header::AUTHORIZATION,
                format!("Bearer {}", access_token),
            )
            .send()
            .await;

        #[derive(Deserialize)]
        struct PlayStatus {
            is_playing: bool,
        }

        #[derive(Deserialize)]
        struct DeviceStatus {
            devices: Vec<Device>,
        }

        #[derive(Deserialize)]
        struct Device {
            id: String,
        }

        if let Ok(currently_playing) = currently_playing {
            let play_status: PlayStatus = match currently_playing.json().await {
                Ok(status) => status,
                Err(_e) => {
                    let devices_status = client
                        .get("https://api.spotify.com/v1/me/player/devices")
                        .header(
                            reqwest::header::AUTHORIZATION,
                            format!("Bearer {}", access_token),
                        )
                        .send()
                        .await;

                    if let Ok(devices_status) = devices_status {
                        let device_status: DeviceStatus = match devices_status.json().await {
                            Ok(status) => status,
                            Err(_e) => return Ok(false),
                        };
                        if let Some(first_device) = device_status.devices.get(0) {
                            let _response = client
                                .put(format!(
                                    "https://api.spotify.com/v1/me/player/play?device_id={}",
                                    &first_device.id
                                ))
                                .header(
                                    reqwest::header::AUTHORIZATION,
                                    format!("Bearer {}", access_token),
                                )
                                .header(reqwest::header::CONTENT_LENGTH, 0)
                                .send()
                                .await;
                        }
                    }
                    return Ok(true);
                }
            };
            is_playing_glob = play_status.is_playing;
            if play_status.is_playing {
                let _response = client
                    .put("https://api.spotify.com/v1/me/player/pause")
                    .header(
                        reqwest::header::AUTHORIZATION,
                        format!("Bearer {}", access_token),
                    )
                    .header(reqwest::header::CONTENT_LENGTH, 0)
                    .send()
                    .await;
            } else {
                let _response = client
                    .put("https://api.spotify.com/v1/me/player/play")
                    .header(
                        reqwest::header::AUTHORIZATION,
                        format!("Bearer {}", access_token),
                    )
                    .header(reqwest::header::CONTENT_LENGTH, 0)
                    .send()
                    .await;
            }
        }

        Ok(is_playing_glob)
    } else {
        Ok(false)
    }
}

#[tauri::command]
pub async fn spotify_backward() -> Result<(), Error> {
    if let Some(ref access_token) = *ACCESS_TOKEN.lock().await {
        let client = Client::new();
        let _response = client
            .post("https://api.spotify.com/v1/me/player/previous")
            .header(
                reqwest::header::AUTHORIZATION,
                format!("Bearer {}", access_token),
            )
            .header(reqwest::header::CONTENT_LENGTH, 0)
            .send()
            .await;
        Ok(())
    } else {
        Ok(())
    }
}

#[tauri::command]
pub async fn spotify_forward() -> Result<(), Error> {
    if let Some(ref access_token) = *ACCESS_TOKEN.lock().await {
        let client = Client::new();
        let _response = client
            .post("https://api.spotify.com/v1/me/player/next")
            .header(
                reqwest::header::AUTHORIZATION,
                format!("Bearer {}", access_token),
            )
            .header(reqwest::header::CONTENT_LENGTH, 0)
            .send()
            .await;

        HttpResponse::InternalServerError().finish();
        Ok(())
    } else {
        HttpResponse::InternalServerError().finish();

        Ok(())
    }
}

#[tauri::command]
pub async fn spotify_info() -> Result<String, Error> {
    if let Some(ref access_token) = *ACCESS_TOKEN.lock().await {
        let client = Client::new();
        let currently_playing_api = client
            .get("https://api.spotify.com/v1/me/player/currently-playing")
            .header(
                reqwest::header::AUTHORIZATION,
                format!("Bearer {}", &access_token),
            )
            .send()
            .await
            .expect("Current-Play-API failed (spotify)");

        #[derive(Deserialize)]
        struct CurrentlyPlayingResponse {
            is_playing: bool,
            item: Item,
            progress_ms: i64,
        }

        #[derive(Deserialize)]
        struct Item {
            artists: Vec<Artist>,
            name: String,
            album: Album,
            duration_ms: i64,
        }

        #[derive(Deserialize)]
        struct Artist {
            name: String,
        }

        #[derive(Deserialize)]
        struct Album {
            images: Vec<Image>,
            uri: String,
        }

        #[derive(Deserialize)]
        struct Image {
            url: String,
        }

        let currently_playing: CurrentlyPlayingResponse = currently_playing_api
            .json()
            .await
            .expect("Current-Play-API to JSON failed (spotify)");

        let json_content = format!(
            "{{
                        \"song_name\": \"{}\",
                        \"artist_name\": \"{}\",
                        \"cover\": \"{}\",
                        \"is_playing\": {},
                        \"progress\": {},
                        \"uri\": \"{}\",
                        \"duration\": {}
                    }}",
            currently_playing.item.name,
            currently_playing.item.artists[0].name,
            currently_playing.item.album.images[0].url,
            currently_playing.is_playing,
            currently_playing.progress_ms,
            currently_playing.item.album.uri,
            currently_playing.item.duration_ms
        );

        HttpResponse::Found().finish();

        Ok(json_content)
    } else {
        HttpResponse::InternalServerError().finish();

        Ok("{
                    \"song_name\": \"\",
                    \"artist_name\": \"\",
                    \"cover\": \"\",
                    \"is_playing\": false,
                    \"progress\": 0,
                    \"duration\": 0
                }"
        .to_string())
    }
}

#[tauri::command]
pub async fn spotify_remove_token() -> Result<(), String> {
    *ACCESS_TOKEN.lock().await = None;

    Ok(())
}
