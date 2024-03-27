use crate::launchers::GameObject;
use crate::operations::custom_fs::{d_f_exists, read_dir};
use std::process::Command;

fn get_riot_games_location(launcher_location: &str) -> String {
    let s = launcher_location.split("\\").collect::<Vec<_>>();
    return s[0..s.len() - 2].join("\\");
}

pub async fn get_installed_games() -> Vec<GameObject> {
    #[cfg(target_os = "windows")]
    let output = Command::new("cmd")
        .args(&[
            "/C",
            "Reg",
            "Query",
            "HKEY_CLASSES_ROOT\\riotclient\\DefaultIcon",
        ])
        .output()
        .expect("failed to execute process.");
    if output.stdout.is_empty() {
        return vec![];
    }

    let launcher_location = String::from_utf8_lossy(&output.stdout)
        .to_string()
        .split('"')
        .collect::<Vec<_>>()[1]
        .to_string();

    let gameobj: Vec<_>;
    if d_f_exists(&get_riot_games_location(&launcher_location))
        .await
        .expect("Something went wrong")
    {
        let games: Vec<String> = read_dir(&get_riot_games_location(&launcher_location))
            .await
            .expect("Something went wrong")
            .into_iter()
            .filter(|x| x != "Riot Client")
            .collect::<Vec<_>>();

        gameobj = games
            .iter()
            .map(|x| parse_game_object(&launcher_location, &x))
            .collect::<Vec<_>>();
    } else {
        return vec![];
    }

    let mut games: Vec<GameObject> = vec![];

    for obj in gameobj.into_iter() {
        games.push(obj);
    }
    return games;
}

pub fn parse_game_object(path: &str, game: &str) -> GameObject {
    let correct_args: &str;
    const VALORANT: &str = "VALORANT";
    const LOL: &str = "League of Legends";
    const LOR: &str = "LoR";
    match game {
        VALORANT => {
            correct_args = "valorant";
        }
        LOL => {
            correct_args = "league_of_legends";
        }
        LOR => {
            correct_args = "bacon";
        }
        &_ => todo!()
    }
    let executable = "RiotClientServices.exe";
    let location = &path[0..path.len() - 22];
    let args = vec![
        format!("launch-product={}", correct_args),
        "--launch-patchline=live".to_string(),
    ];
    let display_name = game;
    return GameObject::new(
        banners::get_banner(&display_name, "", "RiotGames").await,
        executable.to_string(),
        location.to_string(),
        display_name.to_string(),
        game.to_string(),
        0.to_string(),
        0,
        "".to_string(),
        "RiotGames".to_string(),
        args,
    )
}

// export { getInstalledGames, parseGameObject };
