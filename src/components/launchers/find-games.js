const os = window.__TAURI__.os;
const fs = window.__TAURI__.fs;
const invoke = window.__TAURI__.invoke;
const path = window.__TAURI__.path;
const Window = window.__TAURI__.window
const tauri = window.__TAURI__.tauri;

let lastCheck;
let cachedGames = [];
let running = false;
const processes = new Map();

function wintoesFix(str, index, value) { 
    return str.substr(0, index) + value + str.substr(index); 
}
async function rootDir() {
    if (await os.platform() === 'win32') {
        return wintoesFix(await path.resolve(await path.dirname(decodeURI(new URL(import.meta.url).pathname))), 2, "\\")
    } else if (await os.platform() === 'linux') {
        return await path.dirname(decodeURI(new URL(import.meta.url).pathname))
    }
}

async function getInstalledGames() {
    if (!lastCheck) {
        lastCheck = Date.now();
    }
    else if (lastCheck + 1000 * 4 > Date.now()) {
        return `COOLDOWN_${(lastCheck + 1000 * 4) - Date.now()}`;
    }

    let data = await fs.readDir(await rootDir());
    const launchers = data.map(x => x.name).filter(x => require(`./${x}`)?.getInstalledGames && !['find-games.js'].includes(x));
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

    const games_blacklist = JSON.parse(await fs.readTextFile(await path.join(await rootDir(), "../blacklist.json")))

    let resolvedGames = [];
    for (let i = 0; i < games.length; i++) {
        let x = games[i];
        if (games_blacklist[0].includes(x.GameID)) continue;
        if (list.children.namedItem(`game-div-${x.DisplayName}`) !== null) continue;
        if (id === 'allGames') {
            resolvedGames.push(x);
        }
        else if (id === 'recentGames') {
            const data = await getGames(x.GameID, x.LauncherName);
            if (typeof data?.Launches === 'number') resolvedGames.push(x);
        }
        else if (id === 'favGames') {
            const data = await getGames(x.GameID, x.LauncherName);
            if (typeof data?.Favourite === 'boolean' && data?.Favourite === true || false) resolvedGames.push(x);
        }
        else if (id.startsWith('recent') && id.includes('Main')) {
            const data = await getGames(x.GameID, x.LauncherName);
            if (typeof data?.Launches === 'number') resolvedGames.push(x);
        }
    }
    resolvedGames = await sort(resolvedGames, id === 'allGames' ? 'alphabetical' : id === 'recentGames' || (id.startsWith('recent') && id.includes('Main')) ? 'lastLaunch' : 'none');

    if ((list.children.length === resolvedGames.length) && list.children.length !== 0) {
        if (resolvedGames.every((x, i) => list.children.item(i).id === `game-div-${x.DisplayName}`)) {
            running = false;
            return;
        }
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
                const dirs = await fs.readDir(GAME_BANNERS_BASE_PATH);
                const img = dirs.find(x => x.name === `${require("../modules/sha256").sha256(game.DisplayName)}.png`);

                banner = img ? tauri.convertFileSrc(appDirPath + `storage/Cache/Games/Images/${JSON.stringify(img.name).slice(1, -1)}`) : 'https://i.ibb.co/dK15dV3/e.jpg';
            } catch (err) {
                banner = 'https://i.ibb.co/dK15dV3/e.jpg';
                console.log(err);
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

        gameBanner.addEventListener('click', async () => {
            handleLaunch(game);
            VisibilityState()
        });
        if (id.startsWith('recent') && id.includes('Main')) return game;

        // Set Game Display Name
        const gameText = document.createElement('span');
        if (game.DisplayName.length > 20) {
            gameText.innerHTML = game.DisplayName.slice(0, 20);
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
                    let rootDir = await path.resolve(await path.dirname(decodeURI(new URL(import.meta.url).pathname)));
                    starIcon.style.content = `url("${tauri.convertFileSrc(await path.join(rootDir, "../../assets/star-solid.svg"))}")`;
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
                        let rootDir = await path.resolve(await path.dirname(decodeURI(new URL(import.meta.url).pathname)));
                        starIcon.style.content = `url("${tauri.convertFileSrc(await path.join(rootDir, "../../assets/star-empty.svg"))}")`;
                        starIcon.style.filter = 'invert(100%) sepia(0%) saturate(1489%) hue-rotate(35deg) brightness(116%) contrast(100%)';
                    }
                }
            }
        });
        document.addEventListener('mousemove', () => {
            if (!gameBanner.matches(':hover') && !starIcon.matches(':hover')) starIcon.style.visibility = 'hidden';
        });

        starIcon.addEventListener('click', async () => {
            const res = toggleFavourite(game.GameID, game.LauncherName);
            let rootDir = await path.resolve(await path.dirname(decodeURI(new URL(import.meta.url).pathname)));
            let solidOrEmpty = res ? 'solid' : 'empty';
            starIcon.style.content = `url("${tauri.convertFileSrc(await path.join(rootDir, "../../assets/star-" + solidOrEmpty + ".svg"))}")`;
            starIcon.style.filter = res ? 'invert(77%) sepia(68%) saturate(616%) hue-rotate(358deg) brightness(100%) contrast(104%)' : 'invert(100%) sepia(0%) saturate(1489%) hue-rotate(35deg) brightness(116%) contrast(100%)';
        });

        return game;
    }).filter(async x => Object.keys(await x).length > 0);

    if (id === 'allGames') setGames(await Promise.all(resolvedGames), 'all-games');
    await require("../modules/banners").getBannerResponse(await Promise.all(resolvedGames), id);
    running = false;
}

async function sort(games, type) {
    if (type === 'alphabetical') {
        return games.map(x => x.DisplayName).sort().map(x => games[games.findIndex(y => y.DisplayName === x)]);
    }
    else if (type === 'lastLaunch') {
        const data = await getGames();
        return games.filter(x => data.find(y => y.GameID === x.GameID && y.LauncherName === x.LauncherName)?.LastLaunch).sort((a, b) => data.find(x => x.GameID === b.GameID && x.LauncherName === b.LauncherName).LastLaunch - data.find(x => x.GameID === a.GameID && x.LauncherName === a.LauncherName).LastLaunch);
    }
    return games;
}

async function setGames(games, src) {
    const appDirPath = await path.appDir();
    const GAMES_DATA_BASE_PATH = appDirPath + 'storage/Cache/Games/Data.json';
    if (!['all-games'].includes(src)) return fs.writeTextFile(GAMES_DATA_BASE_PATH, JSON.stringify(games));
    let data = await getGames();
    if (!Array.isArray(data)) {
        data = [];
    }
    for (let i = 0; i < games.length; i++) {
        let dgame = data.find(x => x.GameID === games[i].GameID);
        let ngame = games[i];
        if (!dgame) data.push(games[i]);
        if (dgame?.Launches && !ngame?.Launches) ngame.Launches = dgame.Launches;
        if (ngame?.Launches && !dgame?.Launches) dgame.Launches = ngame.Launches;
        if (dgame?.LastLaunch && !ngame?.LastLaunch) ngame.LastLaunch = dgame.LastLaunch;
        if (ngame?.LastLaunch && !dgame?.LastLaunch) dgame.LastLaunch = ngame.LastLaunch;
        if (dgame?.Favourite && !ngame?.Favourite) ngame.Favourite = dgame.Favourite;
        if (ngame?.Favourite && !dgame?.Favourite) dgame.Favourite = ngame.Favourite;
    }
    fs.writeTextFile(GAMES_DATA_BASE_PATH, JSON.stringify(data));
}
async function getGames(GameID, LauncherName) {
    const appDirPath = await path.appDir();
    const GAMES_DATA_BASE_PATH = appDirPath + 'storage/Cache/Games/Data.json';

    if (!Object.keys(JSON.parse(await fs.readTextFile(GAMES_DATA_BASE_PATH))).length) return;
    return GameID ? JSON.parse(await fs.readTextFile(GAMES_DATA_BASE_PATH)).find(x => x.GameID === GameID && x.LauncherName === LauncherName) : JSON.parse(await fs.readTextFile(GAMES_DATA_BASE_PATH));
}
async function toggleFavourite(GameID, LauncherName) {
    const data = await getGames();
    if (!data) return;
    data.find(x => x.GameID === GameID && x.LauncherName === LauncherName).Favourite = !data.find(x => x.GameID === GameID && x.LauncherName === LauncherName).Favourite;

    setGames(data, 'toggle-favourite');
}
async function addLaunch(GameID, LauncherName) {
    const data = await getGames();
    if (!data) return;
    data.find(x => x.GameID === GameID && x.LauncherName === LauncherName).LastLaunch = Date.now();
    data.find(x => x.GameID === GameID && x.LauncherName === LauncherName).Launches = typeof data.find(x => x.GameID === GameID && x.LauncherName === LauncherName).Launches === 'number' ? data.find(x => x.GameID === GameID && x.LauncherName === LauncherName).Launches + 1 : 1;
    setGames(data, 'add-launch');
}

async function handleLaunch(game) {
    let res;
    if (await os.platform() === 'win32') {
        switch (game.LauncherName) {
            case 'EpicGames': {
                res = createProcess('cmd', `/C start /min cmd /c start com.epicgames.launcher://apps/${encodeURIComponent(game.LaunchID)}?action=launch --wait`, game.GameID);
                break;
            }
            case 'Steam': {
                console.log(game.GameID)
                res = createProcess('cmd', `/C start /min cmd /c start steam://rungameid/${game.GameID} --wait`, game.GameID);
                break;
            }
            case 'Uplay': {
                res = createProcess('cmd', `/C start /min cmd /c start uplay://launch/${game.GameID}/0 --wait`, game.GameID);
                break;
            }
            //  case 'Minecraft': {
            //      res = createProcess('minecraft-launcher', [], game.GameID);
            //      break;
            //  }
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
            case 'Lunar': {
                res = createProcess('lunarclient', "", game.gameID);
                break;
            }
            case 'Lutris': {
                res = createProcess('lutris', `lutris:rungameid/${game.LaunchID}`, game.gameID);
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
