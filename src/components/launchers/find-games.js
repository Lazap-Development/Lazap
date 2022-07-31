const os = window.__TAURI__.os;
const fs = window.__TAURI__.fs;
const invoke = window.__TAURI__.invoke;
const path = window.__TAURI__.path;
const Window = window.__TAURI__.window
const tauri = window.__TAURI__.tauri;
const { sha256 } = require('../modules/sha256.js');

const processes = new Map();

function fixPath(str, index, value) {
	return str.substr(0, index) + value + str.substr(index);
}
async function rootDir() {
	if (await os.platform() === 'win32') {
		return fixPath(await path.resolve(await path.dirname(decodeURI(new URL(import.meta.url).pathname))), 2, '\\');
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

async function filterAndSort(games, type) {
	const list = document.getElementById(type);

	// Check if the list already has the games loaded and prevent further execution
	if ((list.children.length === games.length) && list.children.length !== 0) {
		if (games.every((x, i) => list.children.item(i).id === `game-div-${x.DisplayName.replaceAll(' ', '_')}`)) return [];
	}

	// Filter out new games and delete old games
	const games_blacklist = JSON.parse(await fs.readTextFile(await path.join(await rootDir(), '../blacklist.json')));
	games = games.filter(x => !games_blacklist[0].includes(x.GameID) && !list.children.namedItem(`game-div-${x.DisplayName.replaceAll(' ', '_')}`));
	for (let i = 0; i < list.length; i++) {
		if (!games.map(x => x.GameID.replaceAll(' ', '_')).includes(list.children[i].id.slice(9))) {
			list.removeChild(list.children[i]);
			continue;
		}
	}

	if (type === 'allGamesList') {
		return games.map(x => x.DisplayName).sort().map(x => games[games.findIndex(y => y.DisplayName === x)]);
	}
	else if (['recentGamesListMainPage', 'recentGamesList'].includes(type)) {
		let final = [];
		for (let i = 0; i < games.length; i++) {
			const game = await getGames(games[i].GameID, games[i].LauncherName);
			if (typeof game?.LastLaunch === 'number' && typeof game?.Launches === 'number') final.push(game);
		}
		return final;
	}
	else if (type === 'favGamesList') {
		let final = [];
		for (let i = 0; i < games.length; i++) {
			const game = await getGames(games[i].GameID, games[i].LauncherName);
			if (typeof game?.Favourite === 'boolean' && game.Favourite === true) final.push(game);
		}
		return final;
	}
	else {
		return [];
	}
}

async function loadGames(id) {
	const _games_ = await getInstalledGames();
	const games = await filterAndSort(_games_, id);
	const list = document.getElementById(id);

	games.map(async (game) => {
		const gameElement = Elements.getGameElement(game, id);
		list.appendChild(gameElement);

		const gameBanner = await Elements.getGameBannerElement(game);
		gameElement.appendChild(gameBanner);
		// eslint-disable-next-line no-self-assign
		game.Banner = game.Banner;

		gameBanner.addEventListener('click', () => {
			handleLaunch(game);
		});

		if (id.startsWith('recent') && id.includes('Main')) return game;

		const gameText = Elements.getGameDisplayElement(game);
		gameElement.appendChild(gameText);

		const starIcon = await Elements.getStarElement(game, gameElement, gameBanner);
		gameElement.appendChild(starIcon);
		return game;
	}).filter(async x => Object.keys(await x).length > 0);
	if ((games.length > 0) && id === 'allGamesList') {
		setGames(games, 'all-games');
	}

	require('../modules/banners').getBanners(await Promise.all(_games_));

}

async function handleLaunch(game) {
	let res;
	if (await os.platform() === 'win32') {
		switch (game.LauncherName) {
			case 'EpicGames': {
				res = createProcess('cmd', `/C start /min cmd /c start com.epicgames.launcher://apps/${encodeURIComponent(game.LaunchID)}?action=launch&silent=true`, game.GameID);
				break;
			}
			case 'Steam': {
				res = createProcess('cmd', `/C start /min cmd /c start steam://rungameid/${game.GameID}`, game.GameID);
				break;
			}
			case 'Uplay': {
				res = createProcess('cmd', `/C start /min cmd /c start uplay://launch/${game.GameID}/0`, game.GameID);
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
				res = createProcess('minecraft-launcher', '', game.GameID);
				break;
			}
			case 'Lunar': {
				res = createProcess('lunarclient', '', game.gameID);
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

	if (res === 'RUNNING_ALREADY') return;

	addLaunch(game.GameID, game.LauncherName);
}
async function toggleFavourite(GameID, LauncherName) {
	const data = await getGames();
	if (!data) return;
	const game = data.find(x => x.GameID === GameID && x.LauncherName === LauncherName);
	game.Favourite = !game.Favourite;
	if (game.Favourite === false && document.getElementById('favGamesList').children.namedItem(`game-div-${game.DisplayName.replaceAll(' ', '_')}`)) {
		const element = document.getElementById('favGamesList').children.namedItem(`game-div-${game.DisplayName.replaceAll(' ', '_')}`);
		element.classList.add('fadeOutUpNoDelay');
		setTimeout(() => document.getElementById('favGamesList').removeChild(element), 500);
	}

	setGames(data, 'toggle-favourite');
}
async function addLaunch(GameID, LauncherName) {
	const data = await getGames();
	if (!data) return;
	data.find(x => x.GameID === GameID && x.LauncherName === LauncherName).LastLaunch = Date.now();
	data.find(x => x.GameID === GameID && x.LauncherName === LauncherName).Launches = typeof data.find(x => x.GameID === GameID && x.LauncherName === LauncherName).Launches === 'number' ? data.find(x => x.GameID === GameID && x.LauncherName === LauncherName).Launches + 1 : 1;
	setGames(data, 'add-launch');
}
function createProcess(Command, Args, GameID, force = false) {
	if (processes.get(GameID) && !force) return 'RUNNING_ALREADY';
	VisibilityState();
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

		if (LauncherData.trayMinLaunch === true) {
			if (await Window.appWindow.isVisible() === true) {
				Window.appWindow.hide()
			} else {
				Window.appWindow.show()
			}

		}
	} catch (e) {
		return console.log(e);
	}
}

async function setGames(games, source) {
	const appDirPath = await path.appDir();
	const GAMES_DATA_BASE_PATH = appDirPath + 'storage/Cache/Games/Data.json';
	const data = JSON.parse(await fs.readTextFile(GAMES_DATA_BASE_PATH).catch(() => '[]'));

	if (source === 'add-launch') {
		fs.writeTextFile(GAMES_DATA_BASE_PATH, JSON.stringify(games));
	}
	else if (source === 'toggle-favourite') {
		fs.writeTextFile(GAMES_DATA_BASE_PATH, JSON.stringify(games));
	}
	else if (source === 'all-games') {
		if (data.length > 0) {
			for (let i = 0; i < games.length; i++) {
				const game = data.find(x => x.GameID === games[i].GameID && x.LauncherName === games[i].LauncherName);
				if (!game) {
					data.push(games[i]);
				}
				else if (Object.keys(games[i]).length < Object.keys(data.find(x => x.GameID === games[i].GameID && x.LauncherName === games[i].LauncherName)).length) {
					let obj = { ...data.find(x => x.GameID === games[i].GameID && x.LauncherName === games[i].LauncherName) };
					Object.keys(data.find(x => x.GameID === games[i].GameID && x.LauncherName === games[i].LauncherName)).filter(x => !Object.keys(games[i]).includes(x)).forEach(x => obj[x] = data.find(x => x.GameID === games[i].GameID && x.LauncherName === games[i].LauncherName)[x]);
					data.splice(data.findIndex(x => x.GameID === games[i].GameID && x.LauncherName === games[i].LauncherName), 1, obj);
				}
				else if (Object.keys(games[i]).length > Object.keys(data.find(x => x.GameID === games[i].GameID && x.LauncherName === games[i].LauncherName)).length) {
					data[data.findIndex(x => x.GameID === games[i].GameID && x.LauncherName === games[i].LauncherName)] = games[i];
				}
			}
		}
		else {
			return fs.writeTextFile(GAMES_DATA_BASE_PATH, JSON.stringify(games));
		}
		fs.writeTextFile(GAMES_DATA_BASE_PATH, JSON.stringify(data));
	}
}
async function getGames(GameID, LauncherName) {
	const data = JSON.parse(await fs.readTextFile(await path.appDir() + 'storage/Cache/Games/Data.json').catch(() => '[]'));

	if (GameID && LauncherName) {
		return data.find(x => (x.GameID === GameID) && (x.LauncherName === LauncherName));
	}
	else {
		return data;
	}
}
export {
	getInstalledGames,
	loadGames,
};

class Elements {
	static getGameElement(game, id) {
		const gameElement = document.createElement('div');

		gameElement.id = `game-div-${game.DisplayName.replaceAll(' ', '_')}`;
		gameElement.className += id.startsWith('recent') && id.includes('Main') ? 'mainPageGamebox' : 'gamebox';
		gameElement.style.diplay = 'table';

		return gameElement;
	}

	static async getGameBannerElement(game) {
		const appDirPath = await path.appDir();
		const GAME_BANNERS_BASE_PATH = `${appDirPath}storage/Cache/Games/Images`;

		const gameBanner = document.createElement('img');

		let banner;
		const dirs = await fs.readDir(GAME_BANNERS_BASE_PATH).catch(() => []);
		const img = dirs.find(x => x.name === `${sha256(game.DisplayName.replaceAll(' ', '_'))}.png`);
		if (img) {
			banner = img ? tauri.convertFileSrc(appDirPath + `storage/Cache/Games/Images/${JSON.stringify(img.name).slice(1, -1)}`) : 'https://i.ibb.co/dK15dV3/e.jpg';
		}
		else {
			banner = 'https://i.ibb.co/dK15dV3/e.jpg';
		}

		gameBanner.setAttribute('src', banner);
		gameBanner.height = 500;
		gameBanner.width = 500;
		game.Banner = banner;

		return gameBanner;
	}
	static getGameDisplayElement(game) {
		// Set Game Display Name
		const gameText = document.createElement('span');
		if (game.DisplayName.length > 20) {
			gameText.innerHTML = game.DisplayName.slice(0, 20);
			gameText.innerHTML += '...';
		}
		else {
			gameText.innerHTML = game.DisplayName;
		}

		return gameText;
	}
	static async getStarElement(game, gameElement, gameBanner) {
		const appDirPath = await path.appDir();

		const starIcon = document.createElement('div');
		starIcon.classList.add('star');

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
		starIcon.addEventListener('click', async () => {
			const res = toggleFavourite(game.GameID, game.LauncherName);
			let rootDir = await path.resolve(await path.dirname(decodeURI(new URL(import.meta.url).pathname)));
			let solidOrEmpty = res ? 'solid' : 'empty';
			starIcon.style.content = `url("${tauri.convertFileSrc(await path.join(rootDir, '../../assets/star-' + solidOrEmpty + '.svg'))}")`;
			starIcon.style.filter = res ? 'invert(77%) sepia(68%) saturate(616%) hue-rotate(358deg) brightness(100%) contrast(104%)' : 'invert(100%) sepia(0%) saturate(1489%) hue-rotate(35deg) brightness(116%) contrast(100%)';
		});
		document.addEventListener('mousemove', () => {
			if (!gameBanner.matches(':hover') && !starIcon.matches(':hover')) starIcon.style.visibility = 'hidden';
		});

		return starIcon;
	}
}
