const os = window.__TAURI__.os;
const fs = window.__TAURI__.fs;
const invoke = window.__TAURI__.invoke;
const path = window.__TAURI__.path;
const Window = window.__TAURI__.window
const tauri = window.__TAURI__.tauri;
const { sha256 } = require('../modules/sha256.js');

const processes = new Map();

function wintoesFix(str, index, value) {
    return str.substr(0, index) + value + str.substr(index);
}
async function rootDir() {
    if (await os.platform() === 'win32') {
        return wintoesFix(await path.resolve(await path.dirname(decodeURI(new URL(import.meta.url).pathname))), 2, '\\');
    } else if (await os.platform() === 'linux') {
        return await path.dirname(decodeURI(new URL(import.meta.url).pathname));
    }
}

async function getInstalledGames() {
	// Fetch all games
	const launchers = (await fs.readDir(await rootDir())).map(x => x.name).filter(x => !['find-games.js'].includes(x));
	const games = (await Promise.all(launchers.map(x => require(`./${x}`)?.getInstalledGames()))).flat();

	return games;
}

async function loadGames() {
	let games = await filterAndSort(await getInstalledGames(), 'allGamesList');
	const list = document.getElementById('allGamesList');

	const appDirPath = await path.appDir();
	const GAME_BANNERS_BASE_PATH = `${appDirPath}storage/Cache/Games/Images`;

	games.map(async (game) => {
		const gameElement = document.createElement('div');

		gameElement.id = `game-div-${game.DisplayName.replaceAll(' ', '_')}`;
		gameElement.className += 'gamebox';
		gameElement.style.diplay = 'table';
		list.appendChild(gameElement);

		const gameBanner = document.createElement('img');

		let banner;
		const dirs = await fs.readDir(GAME_BANNERS_BASE_PATH).catch(() => []);
		const img = dirs.find(x => x.name === `${sha256(game.DisplayName.replaceAll(' ', '_'))}.png`);
		if (img) {
			banner = `${GAME_BANNERS_BASE_PATH}${img.name.slice(1, -1)}`;
		}
		else {
			banner = 'https://cdn.discordapp.com/attachments/910422768045133869/914878042508251156/icon.png';
		}

		gameBanner.setAttribute('src', banner);
		gameBanner.style = 'opacity: 0.2;';
		gameBanner.height = 500;
		gameBanner.width = 500;
		gameElement.appendChild(gameBanner);
		game.Banner = banner;

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

		gameBanner.addEventListener('click', () => {
			handleLaunch(game);
		});

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
	await require('../modules/banners').getBanners(await Promise.all(games));
}
async function filterAndSort(games, type) {
	const list = document.getElementById(type);

	// Check if the list already has the games loaded and prevent further execution
	if ((list.children.length === games.length) && list.children.length !== 0) {
		if (games.every((x, i) => list.children.item(i).id === `game-div-${x.DisplayName.replaceAll(' ', '_')}`)) return [];
	}

	// Filter out new games and delete old games
	const games_blacklist = JSON.parse(await fs.readTextFile(await path.join(await rootDir(), "../blacklist.json")))
	games = games.filter(x => !games_blacklist.includes(x.GameID) && !list.children.namedItem(`game-div-${x.DisplayName.replaceAll(' ', '_')}`));
	for (let i = 0; i < list.length; i++) {
		if (!games.map(x => x.GameID.replaceAll(' ', '_')).includes(list.children[i].id.slice(9))) {
			list.removeChild(list.children[i]);
			continue;
		}
	}

	if (type === 'allGamesList') {
		return games.map(x => x.DisplayName).sort().map(x => games[games.findIndex(y => y.DisplayName === x)]);
	}
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
				res = createProcess(`"${game.Location}/${game.Executable}"`, game.Args, game.GameID);
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
function createProcess(Command, Args, GameID, force = false) {
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
	getInstalledGames,
	loadGames,
};
