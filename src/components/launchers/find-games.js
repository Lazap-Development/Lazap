const os = window.__TAURI__.os;
const fs = window.__TAURI__.fs;
const invoke = window.__TAURI__.invoke;
const path = window.__TAURI__.path;
const Window = window.__TAURI__.window

let lastCheck;
let cachedGames = [];
const games_blacklist = [
    '228980', // Steamworks Common Redistributables
    '231350', // 3D Mark Demo
    '1493710', // Proton Exp
    '1391110', // Steam Linux Runtime,
    '1113280', // Proton 4.11
    '1245040', // Proton 5.0
    '1420170', // Proton 5.13
    '1580130', // Proton 6.3
    '1887720', // Proton 7.0
];
let running = false;
const processes = new Map();

async function getInstalledGames() {
    // Cooldown
    if (!lastCheck) {
        lastCheck = Date.now();
    }
    else if (lastCheck + 1000 * 4 > Date.now()) {
        return `COOLDOWN_${(lastCheck + 1000 * 4) - Date.now()}`;
    }

    let rootDir = await path.resolve(await path.dirname(decodeURI(new URL(import.meta.url).pathname)));
    
    let data = await fs.readDir(rootDir);
    const launchers = data.map(x => x.name).filter(x => require(`./${x}`)?.getInstalledGames && !['find-games.js'].includes(x))
    console.log((await Promise.all(launchers.map(x => require(`./${x}`).getInstalledGames()))).flat().filter(x => Object.keys(x).length > 0))
    const games = (await Promise.all(launchers.map(x => require(`./${x}`).getInstalledGames()))).flat().filter(x => Object.keys(x).length > 0).flat();

    if (games.length < 1) {
        return 'NO_GAMES_FOUND';
    }

    cachedGames = games;
    return games;
}

async function loadGames(id) {
    const appDirPath = await path.appDir();

    if (running === true) return;
    running = true;

    let games = await getInstalledGames();
    const list = document.getElementById(id) ?? document.getElementById(`${id}List`);

    // Cache results or get from cache if cooldown
    if (typeof games === 'string' && games.startsWith('COOLDOWN')) {
        games = cachedGames;
        list.replaceChildren([]);
    }
    else if (typeof games === 'string') {
        // When games not found
    }
    else if (Array.isArray(games)) {
        cachedGames = games;
    }

    let resolvedGames = sort(games.filter(x => {
        if (games_blacklist.includes(x.GameID)) return false;
        if (id === 'allGames') {
            return true;
        }
        else if (id === 'recentGames') {
            const data = getGames(x.GameID, x.LauncherName);
            return !data?.Launches;
        }
        else if (id === 'favGames') {
            const data = getGames(x.GameID, x.LauncherName);
            return !data?.Favourite;
        }
        else if (id.startsWith('recent') && id.includes('Main')) {
            const data = getGames(x.GameID, x.LauncherName);
            return !data?.Launches;
        }
    }), id === 'allGames' ? 'alphabetical' : id === 'recentGames' || (id.startsWith('recent') && id.includes('Main')) ? 'lastLaunch' : 'none');

    if ((list.children.length === resolvedGames.length) && list.children.length !== 0) {
        if (resolvedGames.every((x, i) => list.children.item(i).id === `game-div-${x.DisplayName}`)) return;
    }

    resolvedGames = resolvedGames.map(async (game) => {
        const GAME_BANNERS_BASE_PATH = appDirPath + 'storage/Cache/Games/Images';

        // Gamebox creation
        const gameElement = document.createElement('div');
        gameElement.id = 'game-div-' + game.DisplayName;
        gameElement.className += id.startsWith('recent') && id.includes('Main') ? 'mainPageGamebox' : 'gamebox';
        gameElement.style.diplay = 'table';
        list.appendChild(gameElement);

        // Game Banner creation
        const gameBanner = document.createElement('img');
        let banner;
        if (game.LauncherName !== 'XboxGames') {

            try {
                await fs.readDir(GAME_BANNERS_BASE_PATH)
            } catch (e) {
                banner = 'https://cdn.discordapp.com/attachments/814938072999395388/983977458120396830/IMG_4432.jpg';
            }
        }
        else {
            banner = game.Banner;

        }
        gameBanner.setAttribute('src', banner);
        gameBanner.style = `opacity: ${id === 'allGames' ? '0.2' : '1'};`;
        gameBanner.height = 500;
        gameBanner.width = 500;
        gameElement.appendChild(gameBanner);

        game.Banner = banner;

        if (id.startsWith('recent') && id.includes('Main')) return game;

        // Set Game Display Name
        const gameText = document.createElement('span');
        if (game.DisplayName.length > 18) {
            gameText.innerHTML = game.DisplayName.slice(0, 18);
            gameText.innerHTML += '...';
        }
        else {
            gameText.innerHTML = game.DisplayName;
        }
        gameElement.appendChild(gameText);

        gameBanner.addEventListener('click', async () => {
            handleLaunch(game);
            VisibilityState()
        });

        if (id !== 'allGames') return game;

        // Handle starring
        const starIcon = document.createElement('div');
        starIcon.classList.add('star');
        gameElement.appendChild(starIcon);

        gameBanner.addEventListener('mouseover', async () => {
            const x = gameElement.getElementsByClassName('star');
            const isFavourite = JSON.parse(await fs.readTextFile(appDirPath + 'storage/Cache/Games/Data.json')).find(y => y.GameID === game.GameID && y.LauncherName === game.LauncherName && y.Favourite);
            for (let i = 0; i < x.length; i++) {
                starIcon.classList.add('fade');
                x[i].style.visibility = 'visible';
                if (isFavourite) {
                    starIcon.style.content = 'url("../assets/star-solid.svg")';
                    starIcon.style.filter = 'invert(77%) sepia(68%) saturate(616%) hue-rotate(358deg) brightness(100%) contrast(104%)';
                }
            }
        });

        gameBanner.addEventListener('mouseout', async () => {
            const x = gameElement.getElementsByClassName('star');
            const isFavourite = JSON.parse(await fs.readTextFile(appDirPath + 'storage/Cache/Games/Data.json')).find(y => y.GameID === game.GameID && y.LauncherName === game.LauncherName && y.Favourite);
            for (let i = 0; i < x.length; i++) {
                if (!x[i].matches(':hover')) {
                    starIcon.classList.remove('fade');
                    x[i].style.visibility = 'hidden';
                    if (!isFavourite) {
                        starIcon.style.content = 'url("../assets/star-empty.svg")';
                        starIcon.style.filter = 'invert(100%) sepia(0%) saturate(1489%) hue-rotate(35deg) brightness(116%) contrast(100%)';
                    }
                }
            }
        });
        document.addEventListener('mousemove', () => {
            if (!gameBanner.matches(':hover') && !starIcon.matches(':hover')) starIcon.style.visibility = 'hidden';
        });

        starIcon.addEventListener('click', () => {
            const res = toggleFavourite(game.GameID, game.LauncherName);
            starIcon.style.content = `url("../assets/star-${res ? 'solid' : 'empty'}.svg")`;
            starIcon.style.filter = res ? 'invert(77%) sepia(68%) saturate(616%) hue-rotate(358deg) brightness(100%) contrast(104%)' : 'invert(100%) sepia(0%) saturate(1489%) hue-rotate(35deg) brightness(116%) contrast(100%)';
        });

        return game;
    }).filter(x => Object.keys(x).length > 0);

    setGames(games);
    require("../utils").getBannerResponse(resolvedGames.filter((x) => x.Banner === '../img/icons/icon.ico'), id);
    running = false;
}

function sort(games, type) {
    if (type === 'alphabetical') {
        return games.map(x => x.DisplayName).sort().map(x => games[games.findIndex(y => y.DisplayName === x)]);
    }
    else if (type === 'lastLaunch') {
        const data = getGames().Games;
        return games.filter(x => data.find(y => y.GameID === x.GameID && y.LauncherName === x.LauncherName)?.LastLaunch).sort((a, b) => data.find(x => x.GameID === b.GameID && x.LauncherName === b.LauncherName).LastLaunch - data.find(x => x.GameID === a.GameID && x.LauncherName === a.LauncherName).LastLaunch);
    }
    return games;
}

async function setGames(games) {
    const appDirPath = await path.appDir();
    const GAMES_DATA_BASE_PATH = appDirPath + 'storage/Cache/Games/Data.json';
    fs.writeTextFile(GAMES_DATA_BASE_PATH, JSON.stringify(games));
}
async function getGames(GameID, LauncherName) {
    const appDirPath = await path.appDir();
    const GAMES_DATA_BASE_PATH = appDirPath + 'storage/Cache/Games/Data.json';

    return GameID ? await fs.readTextFile(GAMES_DATA_BASE_PATH).Games.find(x => x.GameID === GameID && x.LauncherName === LauncherName) : await fs.readTextFile(GAMES_DATA_BASE_PATH);
}
function toggleFavourite(GameID, LauncherName) {
    const data = getGames();
    if (!data.Games) return;
    data.Games.find(x => x.GameID === GameID && x.LauncherName === LauncherName).Favourite = !data.Games.find(x => x.GameID === GameID && x.LauncherName === LauncherName).Favourite;

    setGames(data);
}
function addLaunch(GameID, LauncherName) {
    const data = getGames();
    if (!data.Games) return;
    data.Games.find(x => x.GameID === GameID && x.LauncherName === LauncherName).LastLaunch = Date.now();
    data.Games.find(x => x.GameID === GameID && x.LauncherName === LauncherName).Launches = typeof data.Games.find(x => x.GameID === GameID && x.LauncherName === LauncherName).Launches === 'number' ? data.Games.find(x => x.GameID === GameID && x.LauncherName === LauncherName).Launches + 1 : 1;
    setGames(data);
}

async function handleLaunch(game) {
    let res;
    if (await os.platform() === 'win32') {
        switch (game.LauncherName) {
            case 'EpicGames': {
                res = createProcess('cmd', `/C start com.epicgames.launcher://apps/${encodeURIComponent(game.LaunchID)}?action=launch --wait`, game.GameID);
                break;
            }
            case 'Steam': {
                console.log(game.GameID)
                res = createProcess('cmd', `/C start steam://rungameid/${game.GameID} --wait`, game.GameID);
                break;
            }
            case 'Uplay': {
                res = createProcess('cmd', `/C start uplay://launch/${game.GameID}/0 --wait`, game.GameID);
                break;
            }
            case 'Minecraft': {
                res = createProcess('minecraft-launcher', [], game.GameID);
                break;
            }
            default: {
                res = createProcess(`"${game.Location}/${game.Executable}"`, game.Args, game.GameID);
                break;
            }
        }
    }
    else if (await os.platform() === 'linux') {
        switch (game.LauncherName) {
            case 'Steam': {
                res = createProcess('steam', `steam://rungameid/${game.GameID} -silent`, game.GameID);
                break;
            }
            case 'Minecraft': {
                res = createProcess('minecraft-launcher', "", game.GameID);
                break;
            }
            default: {
                res = createProcess(`"${game.Location}	/${game.Executable}"`, game.Args, game.GameID);
                break;
            }
        }
    }

    if (res === 'RUNNING_ALREADY') {
        document.querySelector('.alert-box-message').textContent = `${game.DisplayName} is already running!`;
        document.querySelector('.alert-box').style.marginTop = '40px';
        document.querySelector('.alert-box').style.visibility = 'visible';
        document.querySelector('.alert-box').style.opacity = '1';
        return document.querySelector('.alert-box').style.display = 'flex';
    }

    addLaunch(game.GameID, game.LauncherName);
}

async function createProcess(Command, Args, GameID, force = false) {
    if (processes.get(GameID) && !force) return 'RUNNING_ALREADY';
    console.log(Args)
    const instance = invoke('run_game', { exec: Command, args: Args })
        .then(() => {
            VisibilityState();
            processes.delete(GameID);
        });
    processes.set(GameID, instance);


    return instance;
}

async function VisibilityState() {
    const appDirPath = await path.appDir();

    try {
        const LauncherData = JSON.parse(await fs.readTextFile(appDirPath + 'storage/LauncherData.json'));

        if (LauncherData.trayMinLaunch == true) {
            if (await Window.appWindow.isVisible() == true) {
                Window.appWindow.hide()
            } else {
                Window.appWindow.show()
            }

        }
    } catch (e) {
        return console.log(e);
    }
}


export {
    loadGames,
    getInstalledGames
}