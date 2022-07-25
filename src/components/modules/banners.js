const http = window.__TAURI__.http;
const fs = window.__TAURI__.fs;
const path = window.__TAURI__.path;
const tauri = window.__TAURI__.tauri;

async function getBannerResponse(games, id) {
    if (id !== 'allGames') return;
    const arr = [];
    for (let i = 0; i < games.length; i++) {
        arr.push((async () => {
            switch (games[i].LauncherName) {
                case 'EpicGames': {
                    return;
                }
                case 'Steam': {
                    return `https://cdn.akamai.steamstatic.com/steam/apps/${games[i].GameID}/library_600x900.jpg`;
                }
                case 'RiotGames': {
                    return 'https://valorant-config.fr/wp-content/uploads/2020/05/7d604cf06abf5866f5f3a2fbd0deacf9-200x300.png';
                }
                case 'Uplay': {
                    return;
                }
                case 'Minecraft': {
                    return 'https://image.api.playstation.com/vulcan/img/cfn/11307uYG0CXzRuA9aryByTHYrQLFz-HVQ3VVl7aAysxK15HMpqjkAIcC_R5vdfZt52hAXQNHoYhSuoSq_46_MT_tDBcLu49I.png';
                }
                case 'FiveM': {
                    return 'https://logos-world.net/wp-content/uploads/2021/03/FiveM-Symbol.png';
                }
                case 'Lunar': {
                    return 'https://www.lunarclient.com/assets/img/default-twitter-icon.webp';
                }
                case 'Lutris': {
                    if (games[i].DisplayName === 'Epic Games Store') return 'https://pcper.com/wp-content/uploads/2021/02/epic-games-store.png'
                    if (games[i].DisplayName === 'Rockstar Games Launcher') return 'https://cdn.player.one/sites/player.one/files/2019/08/26/rockstar-games.png'
                    return `https://thumbnails.pcgamingwiki.com/5/53/${games[i].DisplayName.replace(/ /g, "_")}_-_cover.png/300px-${games[i].DisplayName.replace(/ /g, "_")}_-_cover.png`
                }
                case 'XboxGames': {
                    return games[i].Banner;
                }
            }
        })());

    }

    cacheBanners(games, arr.filter(async x => { return (await x) !== '../img/icons/icon.ico'; }));
    return arr;
}

async function cacheBanners(data, res) {
    const appDirPath = await path.appDir();
    const sha256 = require("../modules/sha256")
    const GAME_BANNERS_BASE_PATH = appDirPath + 'storage/Cache/Games/Images';
    const ALREADY_GAME_BANNERS = await fs.readDir(GAME_BANNERS_BASE_PATH);
    let alreadyProcessed = false;
    let itemsProcessed = 0;

    ALREADY_GAME_BANNERS.forEach(async (x) => {
        data.forEach (async (i) => {
            if (x.name  === `${sha256.convert(i.DisplayName)}.png`) {
                alreadyProcessed = true;
            }
        })
    })

    if(alreadyProcessed === true) {
        document.getElementById('game-loading-overlay').style.opacity = '0';
        document.getElementById('game-loading-overlay').style.visibility = 'hidden';
        return console.log("[BANNER] Banners are already loaded. Skipping.");
    }

    res.filter(async (x) => (await x).startsWith('http')).forEach(async (x, i) => {
        await http.fetch(await x, {
            method: 'GET',
            headers: {
                'Content-Type': 'text/plain; charset=utf-8'
            },
            responseType: 3
        }).then(async (response) => {
            if (response.status === 404 && data[i].LauncherName === "Lutris") return;
            await fs.writeBinaryFile(GAME_BANNERS_BASE_PATH + `/${sha256.convert(data[i].DisplayName)}.png`, response.data);
            document.getElementById(`game-div-${data[i].DisplayName}`).firstElementChild.setAttribute('src', tauri.convertFileSrc(GAME_BANNERS_BASE_PATH + `/${sha256.convert(data[i].DisplayName)}.png`));
        }).catch((e) => console.log(e));
        
        itemsProcessed++;
        if (itemsProcessed === data.length) {
            document.getElementById('game-loading-overlay').style.opacity = '0';
            document.getElementById('game-loading-overlay').style.visibility = 'hidden';
            return console.log("[BANNER] Just finished processing banners.");
        }
    });
}

export {
    getBannerResponse
}








//IGDB (Useful for future :eyes:)
/* 
async function getBannerResponse(games, id) {
    if (id !== 'allGames') return;
    const appDirPath = await path.appDir();
    const gamesList = document.querySelectorAll('#allGamesList > div');

    for (const game of games) {
        const bannerPath = appDirPath + 'storage/Cache/Games/Images';
        const dirs = await fs.readDir(bannerPath);
        const imgs = dirs.find(x => x.name === `${game.DisplayName.replace(/ +/g, "").toLowerCase()}.png`);
        if (imgs) continue;

        const fetchGameInfo = await http.fetch('https://api.igdb.com/v4/games', {
            method: 'POST',
            mode: 'no-cors',
            headers: {
                Accept: "application/json",
                Authorization: "Bearer q4df69koo0tffljh63nbefbw5mhoap",
                "Client-ID": "jvvb7q3e8lf2wcztdv1po6mphhcfzc",
                "Content-Type": "application/x-www-form-urlencoded"
            },
            body: {
                type: 'Text',
                payload: `search "${game.DisplayName}"; limit 1; fields name,cover,url;`
            }
        });
        if (fetchGameInfo.data == "") continue;
        if (fetchGameInfo.data[0].name.toLowerCase() != game.DisplayName.toLowerCase()) continue;

        const getBanner = await http.fetch('https://api.igdb.com/v4/covers', {
            method: 'POST',
            mode: 'no-cors',
            headers: {
                Accept: "application/json",
                Authorization: "Bearer q4df69koo0tffljh63nbefbw5mhoap",
                "Client-ID": "jvvb7q3e8lf2wcztdv1po6mphhcfzc",
                "Content-Type": "application/x-www-form-urlencoded"
            },
            body: {
                type: 'Text',
                payload: `fields url; where id = ${fetchGameInfo.data[0].cover};`
            }
        });

        const GAME_BANNERS_BASE_PATH = appDirPath + 'storage/Cache/Games/Images';

        const bannerURL = "https://" + getBanner.data[0].url.replace("t_thumb", "t_cover_big").slice(2);

        await http.fetch(bannerURL, {
            method: 'GET',
            headers: {
                'Content-Type': 'text/plain; charset=utf-8'
            },
            responseType: 3
        }).then(async (response) => {
            await fs.writeBinaryFile(GAME_BANNERS_BASE_PATH + `/${game.DisplayName.replace(/ +/g, "").toLowerCase()}.png`, response.data)

        }).catch((e) => console.log(e));

        const updatedDir = await fs.readDir(bannerPath);
        const updatedImg = updatedDir.find(x => x.name === `${game.DisplayName.replace(/ +/g, "").toLowerCase()}.png`);
        var gameElement = document.getElementById(`game-div-${game.DisplayName}`);
        gameElement.firstElementChild.setAttribute('src', "asset://" + bannerPath + `/${JSON.stringify(updatedImg.name).slice(1, -1)}`);
    }

    setTimeout(() => {
        document.getElementById('game-loading-overlay').style.opacity = '0';
        document.getElementById('game-loading-overlay').style.visibility = 'hidden';
    }, 100);
    setTimeout(() =>
        // eslint-disable-next-line max-nested-callbacks
        gamesList.forEach((gameElement) => gameElement.firstElementChild.style.opacity = '1'), 200,
    );
}
*/
