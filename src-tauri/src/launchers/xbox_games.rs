use std::process::Command;
use serde::{Deserialize, Serialize};
use crate::{launchers::GameObject, operations::custom_fs::read_file, modules::banners};
use reqwest::{Client, Url};
use scraper::{Html, Selector};

#[derive(Serialize, Deserialize)]
pub struct XboxGame {
    #[serde(rename = "Name")]
    name: String,
    #[serde(rename = "PackageFullName")]
    package_full_name: String,
    #[serde(rename = "InstallLocation", default)]
    install_location: String,
    #[serde(rename = "IsFramework")]
    is_framework: String,
    #[serde(rename = "PackageFamilyName")]
    package_family_name: String,
    #[serde(rename = "PublisherId")]
    publisher_id: String,
    #[serde(rename = "SignatureKind")]
    signature_kind: String,
    banner: String,
}

pub async fn get_installed_games() -> Vec<GameObject> {
    let output = Command::new("cmd")
        .args(&[
            "/C",
            "C:/Windows/System32/WindowsPowerShell/v1.0/powershell.exe",
            "get-appxpackage",
        ])
        .output()
        .expect("failed to execute process.");

    let games: Vec<XboxGame> = parse_raw_to_json(
        String::from_utf8_lossy(&output.stdout).to_string()
    )
        .await
        .into_iter()
        .filter(
            |x|
                x.is_framework != "True" &&
                x.signature_kind == "Store" &&
                x.publisher_id != "8wekyb3d8bbwe" &&
                !x.package_full_name.starts_with("MicrosoftWindows")
        )
        .collect::<Vec<XboxGame>>();

    let verified = verify_games(games).await;

    let objs = verified.into_iter().map(|x| parse_game_object(x)).collect::<Vec<_>>();

    return vec![];
}

pub async fn parse_raw_to_json(stdout: String) -> Vec<XboxGame> {
    let mut arr: Vec<XboxGame> = vec![];
    const VALUES: [&str;7] = ["Name", "PackageFullName", "InstallLocation", "IsFramework", "PackageFamilyName", "PublisherId", "SignatureKind"];

    stdout
    .split("\r\n\r\n")
    .collect::<Vec<_>>()[1..]
    .iter()
    .map(
        |x| x
            .split("\r\n")
            .filter(|y| !y.to_string().ends_with(" ") && VALUES.contains(&y.split(":").collect::<Vec<_>>()[0].trim()))
            .map(
                |y| {
                    let splited = y.splitn(3, ":").collect::<Vec<_>>();
                    return splited.iter().enumerate().map(
                        |(i, z)| {
                            let mut val = z.trim().to_owned();
                            if splited.len() == 3 && i != 0 {
                                if i == 2 {
                                    val = format!("{}\"", val);
                                }
                                else if i != 2 {
                                    val = format!("\"{}", val);
                                }
                            }
                            else {
                                val = format!("\"{}\"", val);
                            }
                            val
                        }
                    ).collect::<Vec<_>>().join(":");
                }
            ).collect::<Vec<_>>().join(",\n").replace("\\", "/")
    )
    .filter(|x| x.len() > 20)
    .for_each(|x| {
        arr.push(serde_json::from_str(&("{".to_string() + &x + &",\n\"banner\":\"\"}".to_string())).unwrap());
    });

    return arr;
}

pub async fn verify_games(games: Vec<XboxGame>) -> Vec<XboxGame> {
    let mut arr = vec![];

    for mut game in games {
        game.name = game.name.split(".").collect::<Vec<_>>()[1].to_string();
        let client = Client::new();
        let response = client
        .get(
            Url::parse(&format!("https://www.microsoft.com/en-in/search/shop/games?q={}&devicetype=pc", game.name)).unwrap()
        )
        .send()
        .await
        .expect("");

        let text = &response.text().await.unwrap();
        let document = Html::parse_document(text);
        let selector = Selector::parse(&"div[id=\"shopDetailsWrapper\"]").unwrap();

        let selected = document.select(&selector).collect::<Vec<_>>()[0];
        let ul_element = selected
        .child_elements().collect::<Vec<_>>()[0]
        .child_elements().collect::<Vec<_>>()[2]
        .child_elements().collect::<Vec<_>>();
        // .child_elements().collect::<Vec<_>>();

        if ul_element.len() == 0 {
            continue;
        }

        let li_element;
        if ul_element[0].first_child().is_some() {
            li_element = ul_element[0]
            .child_elements().collect::<Vec<_>>()[0];
        }
        else {
            continue;
        }

        let title = li_element
        .child_elements().collect::<Vec<_>>()[0]
        .child_elements().collect::<Vec<_>>()[0]
        .child_elements().collect::<Vec<_>>()[1]
        .child_elements().collect::<Vec<_>>()[0]
        .child_elements().collect::<Vec<_>>()[0].inner_html();

        let banner = li_element
        .child_elements().collect::<Vec<_>>()[0]
        .child_elements().collect::<Vec<_>>()[0]
        .child_elements().collect::<Vec<_>>()[0]
        .child_elements().collect::<Vec<_>>()[0]
        .child_elements().collect::<Vec<_>>().last()
        .unwrap()
        .attr("src")
        .unwrap();

        game.name = title;
        game.banner = banner.to_string();
        arr.push(game);
    }

    return arr;
}

// Not complete
pub fn parse_game_object(game: XboxGame) -> Option<GameObject> {
    if game.install_location.len() == 0 {
        return None;
    }
    let gameobject: GameObject;
    let regex = Regex::new();
    let location = &game.install_location;
    let manifeststr = read_file(location.to_string() + "/AppxManifest.xml").unwrap();
    let manifest = manifeststr
    .split("\r\n")
    .find(|x| x.trim().starts_with("<Application "))
    .unwrap();
    // .split(/<[/]{0,1}Application[>]{0,1}/);
    let executable = manifest.split("\"")[1].to_string();
    gameobject = GameObject::new(
        banners::get_banner(&game.name, "", "XboxGames", &game.banner),
        executable,
        location.to_string(),
        game.name,
        game.package_full_name,
        "".to_string(),
        0,
        "".to_string(),
        "XboxGames".to_string(),
        vec![],
    );
    return Some(gameobject);
}