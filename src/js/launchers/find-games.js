/* eslint-disable indent */
console.info('OS:', process.platform, 'Arch:', process.arch);
const {
	ipcRenderer,
} = require('electron');
const Steam = require('./Steam.js');
const EpicGames = require('./EpicGames.js');
const RiotGames = require('./RiotGames.js');
const Uplay = require('./Uplay.js');
const Minecraft = require('./Minecraft.js');
const FiveM = require('./FiveM.js');
const path = require('path');
const APP_BASE_PATH = path.join(__dirname, path.relative(__dirname, './'));
const Constants = require('../../../Constants.json');
const fs = require('fs');
const md5 = require('md5');
const games_blacklist = [
	'228980', // Steamworks Common Redistributables
	'231350'
];
const processes = new Map();

const alphabets = 'abcdefghijklmnopqrstuvwxyz'.split('');
function sort(games, type = 'alphabetical') {
	if (type === 'alphabetical') {
		return games.sort((a, b) => {
			return (alphabets.findIndex(x => x === a.DisplayName.slice(0, 1).toLowerCase()) - alphabets.findIndex(x => x === b.DisplayName.slice(0, 1).toLowerCase()));
		});
	}
}

async function loadAllGames() {
	const gamesElement = document.querySelector('div#allGamesList');

	if (gamesElement.children.length >= 1) {
		return;
	}

	const games = sort([
		...(await Steam.getInstalledGames()),
		...(await Uplay.getInstalledGames()),
		...EpicGames.getInstalledGames(),
		...(await RiotGames.getInstalledGames()),
		...(await Minecraft.getInstalledGames()),
		...(await FiveM.getInstalledGames()),
	]);
	/*
	if (games.length == 0) {
		var exists = document.getElementsByClassName('notFound')

		if (exists.length == 0) {
			const gamesNotFound = document.createElement('div')
			gamesNotFound.className = 'notFound';
			gamesNotFound.textContent = 'Unable to detect games in your computer!';
			return gamesElement.appendChild(gamesNotFound);
		}
	}
	*/

	const uncachedGames = games.map((game) => {
		if (games_blacklist.includes(game.GameID)) return {};

		const gameElement = document.createElement('div');
		gameElement.id = 'game-div-' + game.DisplayName;
		gameElement.className += 'gamebox';
		gameElement.style.diplay = 'table';
		gamesElement.appendChild(gameElement);

		const gameBanner = document.createElement('img');

		let banner;
		if (fs.existsSync(Constants.GAME_BANNERS_BASE_PATH)) {
			const dirs = fs.readdirSync(Constants.GAME_BANNERS_BASE_PATH);
			const img = dirs.find(x => x === `${md5(game.DisplayName)}.png`);
			banner = img ? `${APP_BASE_PATH}/storage/Cache/Games/Images/${img}` : '../icon.ico';
		}
		else {
			banner = '../icon.ico';
		}
		gameBanner.setAttribute('src', banner);
		gameBanner.style = 'opacity: 0.2;';
		gameBanner.height = 500;
		gameBanner.width = 500;
		gameElement.appendChild(gameBanner);

		const gameText = document.createElement('span');
		if (game.DisplayName.length > 18) {
			gameText.innerHTML = game.DisplayName.slice(0, 18);
			gameText.innerHTML += '...';
		}
		else {
			gameText.innerHTML = game.DisplayName;
		}
		gameElement.appendChild(gameText);

		const starIcon = document.createElement('div');
		starIcon.classList.add('star');
		gameElement.appendChild(starIcon);

		gameBanner.addEventListener('click', () => {
			handleLaunch(game);
			ipcRenderer.send('min-tray');
		});

		gameBanner.addEventListener('mouseover', () => {
			const x = gameElement.getElementsByClassName('star');
			const isFavourite = JSON.parse(fs.readFileSync(APP_BASE_PATH + '/storage/Cache/Games/Data.json').toString()).Games.find(y => y.GameID === game.GameID && y.LauncherName === game.LauncherName && y.Favourite);
			for (let i = 0; i < x.length; i++) {
				starIcon.classList.add('fade');
				x[i].style.visibility = 'visible';
				if (isFavourite) {
					starIcon.style.content = 'url("../img/star-solid.svg")';
					starIcon.style.filter = 'invert(77%) sepia(68%) saturate(616%) hue-rotate(358deg) brightness(100%) contrast(104%)';
				}
			}
		});

		gameBanner.addEventListener('mouseout', () => {
			const x = gameElement.getElementsByClassName('star');
			const isFavourite = JSON.parse(fs.readFileSync(APP_BASE_PATH + '/storage/Cache/Games/Data.json').toString()).Games.find(y => y.GameID === game.GameID && y.LauncherName === game.LauncherName && y.Favourite);
			for (let i = 0; i < x.length; i++) {
				if (!x[i].matches(':hover')) {
					starIcon.classList.remove('fade');
					x[i].style.visibility = 'hidden';
					if (!isFavourite) {
						starIcon.style.content = 'url("../img/star-empty.svg")';
						starIcon.style.filter = 'invert(100%) sepia(0%) saturate(1489%) hue-rotate(35deg) brightness(116%) contrast(100%)';
					}
				}
			}
		});

		starIcon.addEventListener('click', () => {
			toggleFavourite(game.GameID, game.LauncherName);
		});

		game.Banner = banner;
		return game;
	}).filter(x => Object.keys(x).length > 0);
	saveGames(uncachedGames);

	ipcRenderer.send('load-banners-request', uncachedGames.filter((x) => x.Banner === '../icon.ico'));
}

function loadFavouriteGames() {
	let Data;
	checkForDirAndCreate(__dirname + '/storage/Cache/Games/Images');
	if (!fs.existsSync(Constants.GAMES_DATA_BASE_PATH + '/Data.json')) {
		fs.writeFileSync(Constants.GAMES_DATA_BASE_PATH + '/Data.json', '{\n\n}');
		Data = {
			Games: [],
		};
	}
	else {
		Data = require(path.join(APP_BASE_PATH, Constants.GAMES_DATA_BASE_PATH.slice(2), '/Data.json'));
	}

	if (document.querySelector('#game-loading-overlay').style.opacity === '1') loadAllGames();

	const games = sort(Data.Games.filter(x => x.Favourite));
	const gamesElement = document.querySelector('div#favGamesList');
	gamesElement.replaceChildren([]);
	games.map((game) => {
		if (games_blacklist.includes(game.GameID)) return {};

		const gameElement = document.createElement('div');
		gameElement.id = 'game-div-' + game.DisplayName;
		gameElement.className += 'gamebox';
		gameElement.style.diplay = 'table';
		gamesElement.appendChild(gameElement);

		const gameBanner = document.createElement('img');

		let banner;
		if (fs.existsSync(Constants.GAME_BANNERS_BASE_PATH)) {
			const dirs = fs.readdirSync(Constants.GAME_BANNERS_BASE_PATH);
			const img = dirs.find(x => x === `${md5(game.DisplayName)}.png`);
			banner = img ? `${APP_BASE_PATH}/storage/Cache/Games/Images/${img}` : '../icon.ico';
		}
		else {
			banner = '../icon.ico';
		}
		gameBanner.setAttribute('src', banner);
		gameBanner.style = 'opacity: 1;';
		gameBanner.height = 500;
		gameBanner.width = 500;
		gameElement.appendChild(gameBanner);

		const gameText = document.createElement('span');
		if (game.DisplayName.length > 18) {
			gameText.innerHTML = game.DisplayName.slice(0, 18);
			gameText.innerHTML += '...';
		}
		else {
			gameText.innerHTML = game.DisplayName;
		}
		gameElement.appendChild(gameText);

		gameBanner.addEventListener('click', () => handleLaunch(game));

		game.Banner = banner;
		return game;
	});
	document.querySelector('div#favs > div#favs-loading-overlay').style.opacity = '0';
	document.querySelector('div#favs > div#favs-loading-overlay').style.visibility = 'hidden';
}

function loadRecentGames() {
	let Data;
	checkForDirAndCreate(__dirname + '/storage/Cache/Games/Images');
	if (!fs.existsSync(Constants.GAMES_DATA_BASE_PATH + '/Data.json')) {
		fs.writeFileSync(Constants.GAMES_DATA_BASE_PATH + '/Data.json', '{\n\n}');
		Data = {
			Games: [],
		};
	}
	else {
		Data = require(path.join(APP_BASE_PATH, Constants.GAMES_DATA_BASE_PATH.slice(2), '/Data.json'));
	}

	if (document.querySelector('#game-loading-overlay').style.opacity === '1') loadAllGames();

	const games = Data.Games.filter(x => x.LastLaunch);
	const gamesElement = document.querySelector('div#recentGamesList');
	gamesElement.replaceChildren([]);
	games.sort((a, b) => b.LastLaunch - a.LastLaunch).map((game) => {
		if (games_blacklist.includes(game.GameID)) return {};

		const gameElement = document.createElement('div');
		gameElement.id = 'game-div-' + game.DisplayName;
		gameElement.className += 'gamebox';
		gameElement.style.diplay = 'table';
		gamesElement.appendChild(gameElement);

		const gameBanner = document.createElement('img');

		let banner;
		if (fs.existsSync(Constants.GAME_BANNERS_BASE_PATH)) {
			const dirs = fs.readdirSync(Constants.GAME_BANNERS_BASE_PATH);
			const img = dirs.find(x => x === `${md5(game.DisplayName)}.png`);
			banner = img ? `${APP_BASE_PATH}/storage/Cache/Games/Images/${img}` : '../icon.ico';
		}
		else {
			banner = '../icon.ico';
		}
		gameBanner.setAttribute('src', banner);
		gameBanner.style = 'opacity: 1;';
		gameBanner.height = 500;
		gameBanner.width = 500;
		gameElement.appendChild(gameBanner);

		const gameText = document.createElement('span');
		if (game.DisplayName.length > 18) {
			gameText.innerHTML = game.DisplayName.slice(0, 18);
			gameText.innerHTML += '...';
		}
		else {
			gameText.innerHTML = game.DisplayName;
		}
		gameElement.appendChild(gameText);

		gameBanner.addEventListener('click', () => handleLaunch(game));

		game.Banner = banner;
		return game;
	});
	document.querySelector('div#recent > div#recent-loading-overlay').style.opacity = '0';
	document.querySelector('div#recent > div#recent-loading-overlay').style.visibility = 'hidden';
}

function loadRecentGamesMainPage() {
	let Data;
	checkForDirAndCreate(__dirname + '/storage/Cache/Games/Images');
	if (!fs.existsSync(Constants.GAMES_DATA_BASE_PATH + '/Data.json')) {
		fs.writeFileSync(Constants.GAMES_DATA_BASE_PATH + '/Data.json', '{\n\n}');
		Data = {
			Games: [],
		};
	}
	else {
		Data = require(path.join(APP_BASE_PATH, Constants.GAMES_DATA_BASE_PATH.slice(2), '/Data.json'));
	}

	if (document.querySelector('#game-loading-overlay').style.opacity === '1') loadAllGames();

	const games = Data.Games.filter(x => x.LastLaunch).slice(0, 5);
	const gamesElement = document.querySelector('div#recentGamesListMainPage');
	gamesElement.replaceChildren([]);
	games.sort((a, b) => b.LastLaunch - a.LastLaunch).map((game) => {
		if (games_blacklist.includes(game.GameID)) return {};
		const gameElement = document.createElement('div');
		gameElement.id = 'game-div-' + game.DisplayName;
		gameElement.className += 'mainPageGamebox';
		gameElement.style.diplay = 'table';
		gamesElement.appendChild(gameElement);

		const gameBanner = document.createElement('img');

		let banner;
		if (fs.existsSync(Constants.GAME_BANNERS_BASE_PATH)) {
			const dirs = fs.readdirSync(Constants.GAME_BANNERS_BASE_PATH);
			const img = dirs.find(x => x === `${md5(game.DisplayName)}.png`);
			banner = img ? `${APP_BASE_PATH}/storage/Cache/Games/Images/${img}` : '../icon.ico';
		}
		else {
			banner = '../icon.ico';
		}
		gameBanner.setAttribute('src', banner);
		gameBanner.style = 'opacity: 1;';
		gameBanner.height = 500;
		gameBanner.width = 500;
		gameElement.appendChild(gameBanner);

		gameBanner.addEventListener('click', () => {
			handleLaunch(game);
			ipcRenderer.send('min-tray');
		});

		game.Banner = banner;
		return game;
	});
	document.querySelector('div#recent > div#recent-loading-overlay').style.opacity = '0';
	document.querySelector('div#recent > div#recent-loading-overlay').style.visibility = 'hidden';
}

function runCommand(command, args, id, force = false) {
	const {
		spawn,
	} = require('child_process');
	if (processes.get(id) && !force) return 'RUNNING_ALREADY';
	const res = spawn(`${command}`, args, {
		detached: true,
		shell: true,
	});
	res.on('error', (error) => console.log('[PROC] Error on process', id, ':', error));
	setTimeout(() => {
		res.on('exit', (code, signal) => {
			ipcRenderer.send('show-window')
			console.log('[PROC] Exited on process', id, 'with code', code, 'and signal', signal);
		});
	}, 1000);

	res.on('spawn', () => console.log('[PROC] Started process with id', id));
	processes.set(id, res);
	return res;
}

function checkForDirAndCreate(dir, fileContent = '') {
	if (fs.existsSync(dir.split(__dirname)[1])) return true;
	dir.split(__dirname)[1].split('/').slice(1).forEach((name, i, arr) => {
		dir = dir.replaceAll('\\', '/');
		if (!fs.existsSync(`./${arr.slice(0, i + 1).join('/')}`)) {
			if (name.split('.')[1]) {
				fs.writeFileSync(`./${arr.slice(0, i + 1).join('/')}`, fileContent);
				return;
			}
			else {
				fs.mkdirSync(`./${arr.slice(0, i + 1).join('/')}`);
			}
		}
	});
}

function saveGames(games) {
	let Data;
	checkForDirAndCreate(__dirname + '/storage/Cache/Games/Images');
	if (!fs.existsSync(Constants.GAMES_DATA_BASE_PATH + '/Data.json')) {
		fs.writeFileSync(Constants.GAMES_DATA_BASE_PATH + '/Data.json', '{\n\n}');
		Data = {
			Games: [],
		};
	}
	else {
		Data = require(path.join(APP_BASE_PATH, Constants.GAMES_DATA_BASE_PATH.slice(2), '/Data.json'));
	}

	const GameIDs = games.map(x => x.GameID);
	if (!Data.Games) Data.Games = [];
	Data.Games = Data.Games.filter(x => GameIDs.includes(x.GameID));
	const StoredGameIDs = Data.Games.map(x => x.GameID);
	Data.Games.push(...games.filter(x => !StoredGameIDs.includes(x.GameID)));
	fs.writeFileSync(Constants.GAMES_DATA_BASE_PATH + '/Data.json', JSON.stringify(Data));
}

function addLaunch(GameID, LauncherName) {
	let Data;
	checkForDirAndCreate(__dirname + '/storage/Cache/Games/Images');
	if (!fs.existsSync(Constants.GAMES_DATA_BASE_PATH + '/Data.json')) {
		fs.writeFileSync(Constants.GAMES_DATA_BASE_PATH + '/Data.json', '{\n\n}');
		Data = {
			Games: [],
		};
	}
	else {
		Data = require(path.join(APP_BASE_PATH, Constants.GAMES_DATA_BASE_PATH.slice(2), '/Data.json'));
	}

	if (!Data.Games.find(x => x.GameID === GameID && x.LauncherName === LauncherName).Launches) Data.Games.find(x => x.GameID === GameID).Launches = 0;
	Data.Games.find(x => x.GameID === GameID && x.LauncherName === LauncherName).Launches++;

	fs.writeFileSync(Constants.GAMES_DATA_BASE_PATH + '/Data.json', JSON.stringify(Data));
}

function toggleFavourite(GameID, LauncherName) {
	let Data;
	checkForDirAndCreate(__dirname + '/storage/Cache/Games/Images');
	if (!fs.existsSync(Constants.GAMES_DATA_BASE_PATH + '/Data.json')) {
		fs.writeFileSync(Constants.GAMES_DATA_BASE_PATH + '/Data.json', '{\n\n}');
		Data = {
			Games: [],
		};
	}
	else {
		Data = require(path.join(APP_BASE_PATH, Constants.GAMES_DATA_BASE_PATH.slice(2), '/Data.json'));
	}

	if (!Data.Games.find(x => x.GameID === GameID && x.LauncherName === LauncherName).Favourite) Data.Games.find(x => x.GameID === GameID).Favourite = false;
	Data.Games.find(x => x.GameID === GameID && x.LauncherName === LauncherName).Favourite = !Data.Games.find(x => x.GameID === GameID).Favourite;

	fs.writeFileSync(Constants.GAMES_DATA_BASE_PATH + '/Data.json', JSON.stringify(Data));
}

function setLastLaunch(GameID, LauncherName) {
	let Data;
	checkForDirAndCreate(__dirname + '/storage/Cache/Games/Images');
	if (!fs.existsSync(Constants.GAMES_DATA_BASE_PATH + '/Data.json')) {
		fs.writeFileSync(Constants.GAMES_DATA_BASE_PATH + '/Data.json', '{\n\n}');
		Data = {
			Games: [],
		};
	}
	else {
		Data = require(path.join(APP_BASE_PATH, Constants.GAMES_DATA_BASE_PATH.slice(2), '/Data.json'));
	}

	if (!Data.Games.find(x => x.GameID === GameID && x.LauncherName === LauncherName).Favourite) Data.Games.find(x => x.GameID === GameID).LastLaunch = Date.now();
	Data.Games.find(x => x.GameID === GameID && x.LauncherName === LauncherName).LastLaunch = Date.now();

	fs.writeFileSync(Constants.GAMES_DATA_BASE_PATH + '/Data.json', JSON.stringify(Data));
}

function handleLaunch(game) {
	addLaunch(game.GameID, game.LauncherName);
	setLastLaunch(game.GameID, game.LauncherName);
	let res;
	switch (game.LauncherName) {
		case 'Steam': {
			res = runCommand('start', [`steam://rungameid/${game.GameID}`, '--wait'], game.GameID);
			break;
		}
		case 'EpicGames': {
			res = runCommand('start', [`com.epicgames.launcher://apps/${encodeURIComponent(game.LaunchID)}?action=launch&silent=true`, '--wait'], game.GameID);
			break;
		}
		case 'Uplay': {
			res = runCommand('start', [`uplay://launch/${game.GameID}/0`, '--wait'], game.GameID);
			break;
		}
		default: {
			res = runCommand(`"${game.Location}/${game.Executable}"`, game.Args, game.GameID);
			break;
		}
	}
	if (res == 'RUNNING_ALREADY') {
		document.querySelector('.alert-box-message').textContent = `${game.DisplayName} is already running!`;
		document.querySelector('.alert-box').style.marginTop = '40px';
		document.querySelector('.alert-box').style.visibility = 'visible';
		return document.querySelector('.alert-box').style.opacity = '1';
	}
}
module.exports = {
	loadAllGames,
	loadFavouriteGames,
	loadRecentGames,
	loadRecentGamesMainPage,
	saveGames,
	toggleFavourite,
	addLaunch,
	setLastLaunch,
};