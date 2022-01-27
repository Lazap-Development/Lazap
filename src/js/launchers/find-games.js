console.info('OS:', process.platform, 'Arch:', process.arch);
const { ipcRenderer } = require('electron');
const Steam = require('./Steam.js');
const EpicGames = require('./EpicGames.js');
const RiotGames = require('./RiotGames.js');
const Uplay = require('./Uplay.js');
const Minecraft = require('./Minecraft.js');
const FiveM = require('./FiveM.js');
const fs = require('fs');
const md5 = require('md5');
const games_blacklist = [
	'228980', // Steamworks Common Redistributables
];

async function loadAllGames() {
	const games = [...(await Steam.getInstalledGames()), 
					...(await Uplay.getInstalledGames()), 
					...EpicGames.getInstalledGames(),
		 			...(await RiotGames.getInstalledGames()), 
		 			...(await Minecraft.getInstalledGames()),
					...(await FiveM.getInstalledGames()),
				];
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

	const gamesElement = document.querySelector('div#allGamesList');

	if (gamesElement.children.length >= 1) {
		document.querySelector('.leftbar-overlay').style.opacity = '0';
		document.querySelector('.leftbar-overlay').style.visibility = 'hidden';
		return;
	}

	const uncachedGames = games.map((game) => {
		if (games_blacklist.includes(game.GameID)) return {};

		const gameElement = document.createElement('div');
		gameElement.id = 'game-div-' + game.DisplayName;
		gameElement.className += 'gamebox';
		gameElement.style.diplay = 'table';
		gamesElement.appendChild(gameElement);

		const gameBanner = document.createElement('img');

		let banner;
		if (fs.existsSync(__dirname.split('\\').slice(0, -3).join('\\') + '\\storage\\Cache\\Games\\Images')) {
			const dirs = fs.readdirSync(__dirname.split('\\').slice(0, -3).join('\\') + '\\storage\\Cache\\Games\\Images');
			const img = dirs.find(x => x === `${md5(game.DisplayName)}.png`);
			banner = img ? `../storage/Cache/Games/Images/${img}` : '../icon.ico';
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
		if (game.DisplayName.length > 25) {
			gameText.innerHTML = game.DisplayName.slice(0, 25);
			gameText.innerHTML += '...';
		}
		else {
			gameText.innerHTML = game.DisplayName;
		}
		gameElement.appendChild(gameText);

		gameBanner.addEventListener('click', () => {
			addLaunch(game.GameID, game.LauncherName);
			setLastLaunch(game.GameID, game.LauncherName);
			switch (game.LauncherName) {
			case 'Steam': {
				window.open(`steam://rungameid/${game.GameID}`, '', 'show=false').close();
				break;
			}
			case 'EpicGames': {
				window.open(`com.epicgames.launcher://apps/${encodeURIComponent(game.LaunchID)}?action=launch&silent=true`, '', 'show=false').close();
				break;
			}
			case 'Uplay': {
				window.open(`uplay://launch/${game.GameID}/0`, '', 'show=false').close();
				break;
			}
			default: {
				runCommand(`${game.Location}/${game.Executable}`, game.Args);
				break;
			}
			}
		});

		game.Banner = banner;
		return game;
	});
	saveGames(uncachedGames);

	ipcRenderer.send('load-banners-request', uncachedGames.filter((x) => x.Banner === '../icon.ico'));
}
function loadFavouriteGames() {
	let Data;
	if (!fs.existsSync(__dirname.split('\\').slice(0, -3).join('\\') + '\\storage\\Cache\\Games\\Data.json')) {
		fs.writeFileSync(__dirname.split('\\').slice(0, -3).join('\\') + '\\storage\\Cache\\Games\\Data.json', '{\n\n}');
		Data = {
			Games: [],
		};
	}
	else {
		Data = require('../../../storage/Cache/Games/Data.json');
	}

	if (document.querySelector('.leftbar-overlay').style.opacity === '1') loadAllGames();

	const games = Data.Games.filter(x => x.Favourite);
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
		if (fs.existsSync(__dirname.split('\\').slice(0, -3).join('\\') + '\\storage\\Cache\\Games\\Images')) {
			const dirs = fs.readdirSync(__dirname.split('\\').slice(0, -3).join('\\') + '\\storage\\Cache\\Games\\Images');
			const img = dirs.find(x => x === `${md5(game.DisplayName)}.png`);
			banner = img ? `../storage/Cache/Games/Images/${img}` : '../icon.ico';
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
		if (game.DisplayName.length > 25) {
			gameText.innerHTML = game.DisplayName.slice(0, 25);
			gameText.innerHTML += '...';
		}
		else {
			gameText.innerHTML = game.DisplayName;
		}
		gameElement.appendChild(gameText);

		gameBanner.addEventListener('click', () => {
			addLaunch(game.GameID, game.LauncherName);
			setLastLaunch(game.GameID, game.LauncherName);
			switch (game.LauncherName) {
			case 'Steam': {
				window.open(`steam://rungameid/${game.GameID}`, '', 'show=false').close();
				break;
			}
			case 'EpicGames': {
				window.open(`com.epicgames.launcher://apps/${encodeURIComponent(game.LaunchID)}?action=launch&silent=true`, '', 'show=false').close();
				break;
			}
			case 'Uplay': {
				window.open(`uplay://launch/${game.GameID}/0`, '', 'show=false').close();
				break;
			}
			default: {
				runCommand(`${game.Location}/${game.Executable}`, game.Args);
				break;
			}
			}
		});

		game.Banner = banner;
		return game;
	});
	document.querySelector('div#favs > div#favs-loading-overlay').style.opacity = '0';
	document.querySelector('div#favs > div#favs-loading-overlay').style.visibility = 'hidden';
}

function loadRecentGames() {
	let Data;
	if (!fs.existsSync(__dirname.split('\\').slice(0, -3).join('\\') + '\\storage\\Cache\\Games\\Data.json')) {
		fs.writeFileSync(__dirname.split('\\').slice(0, -3).join('\\') + '\\storage\\Cache\\Games\\Data.json', '{\n\n}');
		Data = {
			Games: [],
		};
	}
	else {
		Data = require('../../../storage/Cache/Games/Data.json');
	}

	if (document.querySelector('.leftbar-overlay').style.opacity === '1') loadAllGames();

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
		if (fs.existsSync(__dirname.split('\\').slice(0, -3).join('\\') + '\\storage\\Cache\\Games\\Images')) {
			const dirs = fs.readdirSync(__dirname.split('\\').slice(0, -3).join('\\') + '\\storage\\Cache\\Games\\Images');
			const img = dirs.find(x => x === `${md5(game.DisplayName)}.png`);
			banner = img ? `../storage/Cache/Games/Images/${img}` : '../icon.ico';
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
		if (game.DisplayName.length > 25) {
			gameText.innerHTML = game.DisplayName.slice(0, 25);
			gameText.innerHTML += '...';
		}
		else {
			gameText.innerHTML = game.DisplayName;
		}
		gameElement.appendChild(gameText);

		gameBanner.addEventListener('click', () => {
			addLaunch(game.GameID, game.LauncherName);
			setLastLaunch(game.GameID, game.LauncherName);
			switch (game.LauncherName) {
			case 'Steam': {
				window.open(`steam://rungameid/${game.GameID}`, '', 'show=false').close();
				break;
			}
			case 'EpicGames': {
				window.open(`com.epicgames.launcher://apps/${encodeURIComponent(game.LaunchID)}?action=launch&silent=true`, '', 'show=false').close();
				break;
			}
			case 'Uplay': {
				window.open(`uplay://launch/${game.GameID}/0`, '', 'show=false').close();
				break;
			}
			default: {
				runCommand(`${game.Location}/${game.Executable}`, game.Args);
				break;
			}
			}
		});

		game.Banner = banner;
		return game;
	});
	document.querySelector('div#recent > div#recent-loading-overlay').style.opacity = '0';
	document.querySelector('div#recent > div#recent-loading-overlay').style.visibility = 'hidden';
}

async function runCommand(command, args) {
	const { execFile } = require('child_process');
	const { promisify } = require('util');
	const res = await promisify(execFile)(command, args);
	return res;
}
function checkDirs() {
	if (!fs.existsSync(__dirname.split('\\').slice(0, -3).join('\\') + '\\storage')) {
		fs.mkdirSync(__dirname.split('\\').slice(0, -3).join('\\') + '\\storage');
		fs.mkdirSync(__dirname.split('\\').slice(0, -3).join('\\') + '\\storage\\Cache');
		fs.mkdirSync(__dirname.split('\\').slice(0, -3).join('\\') + '\\storage\\Cache\\Games');
	}
	if (!fs.existsSync(__dirname.split('\\').slice(0, -3).join('\\') + '\\storage\\Cache')) {
		fs.mkdirSync(__dirname.split('\\').slice(0, -3).join('\\') + '\\storage\\Cache');
		fs.mkdirSync(__dirname.split('\\').slice(0, -3).join('\\') + '\\storage\\Cache\\Games');
	}
	if (!fs.existsSync(__dirname.split('\\').slice(0, -3).join('\\') + '\\storage\\Cache\\Games')) {
		fs.mkdirSync(__dirname.split('\\').slice(0, -3).join('\\') + '\\storage\\Cache\\Games');
	}
}
function saveGames(games) {
	let Data;
	checkDirs();
	if (!fs.existsSync(__dirname.split('\\').slice(0, -3).join('\\') + '\\storage\\Cache\\Games\\Data.json')) {
		fs.writeFileSync(__dirname.split('\\').slice(0, -3).join('\\') + '\\storage\\Cache\\Games\\Data.json', '{\n\n}');
		Data = {
			Games: [],
		};
	}
	else {
		Data = require('../../../storage/Cache/Games/Data.json');
	}

	const GameIDs = games.map(x => x.GameID);
	if (!Data.Games) Data.Games = [];
	Data.Games = Data.Games.filter(x => GameIDs.includes(x.GameID));
	const StoredGameIDs = Data.Games.map(x => x.GameID);
	Data.Games.push(...games.filter(x => !StoredGameIDs.includes(x.GameID)));
	fs.writeFileSync(__dirname.split('\\').slice(0, -3).join('\\') + '\\storage\\Cache\\Games\\Data.json', JSON.stringify(Data));
}
function addLaunch(GameID, LauncherName) {
	let Data;
	checkDirs();
	if (!fs.existsSync(__dirname.split('\\').slice(0, -3).join('\\') + '\\storage\\Cache\\Games\\Data.json')) {
		fs.writeFileSync(__dirname.split('\\').slice(0, -3).join('\\') + '\\storage\\Cache\\Games\\Data.json', '{\n\n}');
		Data = {
			Games: [],
		};
	}
	else {
		Data = require('../../../storage/Cache/Games/Data.json');
	}

	if (!Data.Games.find(x => x.GameID === GameID && x.LauncherName === LauncherName).Launches) Data.Games.find(x => x.GameID === GameID).Launches = 0;
	Data.Games.find(x => x.GameID === GameID && x.LauncherName === LauncherName).Launches++;

	fs.writeFileSync(__dirname.split('\\').slice(0, -3).join('\\') + '\\storage\\Cache\\Games\\Data.json', JSON.stringify(Data));
}
function toggleFavourite(GameID, LauncherName) {
	let Data;
	checkDirs();
	if (!fs.existsSync(__dirname.split('\\').slice(0, -3).join('\\') + '\\storage\\Cache\\Games\\Data.json')) {
		fs.writeFileSync(__dirname.split('\\').slice(0, -3).join('\\') + '\\storage\\Cache\\Games\\Data.json', '{\n\n}');
		Data = {
			Games: [],
		};
	}
	else {
		Data = require('../../../storage/Cache/Games/Data.json');
	}

	if (!Data.Games.find(x => x.GameID === GameID && x.LauncherName === LauncherName).Favourite) Data.Games.find(x => x.GameID === GameID).Favourite = false;
	Data.Games.find(x => x.GameID === GameID && x.LauncherName === LauncherName).Favourite = !Data.Games.find(x => x.GameID === GameID).Favourite;

	fs.writeFileSync(__dirname.split('\\').slice(0, -3).join('\\') + '\\storage\\Cache\\Games\\Data.json', JSON.stringify(Data));
}
function setLastLaunch(GameID, LauncherName) {
	let Data;
	checkDirs();
	if (!fs.existsSync(__dirname.split('\\').slice(0, -3).join('\\') + '\\storage\\Cache\\Games\\Data.json')) {
		fs.writeFileSync(__dirname.split('\\').slice(0, -3).join('\\') + '\\storage\\Cache\\Games\\Data.json', '{\n\n}');
		Data = {
			Games: [],
		};
	}
	else {
		Data = require('../../../storage/Cache/Games/Data.json');
	}

	if (!Data.Games.find(x => x.GameID === GameID && x.LauncherName === LauncherName).Favourite) Data.Games.find(x => x.GameID === GameID).LastLaunch = Date.now();
	Data.Games.find(x => x.GameID === GameID && x.LauncherName === LauncherName).LastLaunch = Date.now();

	fs.writeFileSync(__dirname.split('\\').slice(0, -3).join('\\') + '\\storage\\Cache\\Games\\Data.json', JSON.stringify(Data));
}
module.exports = {
	loadAllGames,
	loadFavouriteGames,
	loadRecentGames,
	saveGames,
	toggleFavourite,
	addLaunch,
	setLastLaunch,
};
