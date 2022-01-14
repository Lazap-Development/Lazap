console.log('OS:', process.platform, 'Arch:', process.arch);
const { ipcRenderer } = require('electron');
const Steam = require('./Steam.js');
const EpicGames = require('./EpicGames.js');
const RiotGames = require('./RiotGames.js');
const fs = require('fs');
const games_blacklist = [
	'228980', // Steamworks Common Redistributables
];

async function loadGames() {
	const games = [...(await Steam.getInstalledGames()), ...EpicGames.getInstalledGames(), ...(await RiotGames.getInstalledGames())];

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

	const gamesElement = document.querySelector('div#gamesList');
	if (gamesElement.children.length >= 1) return;

	const uncachedGames = games.map((game) => {
		if (games_blacklist.includes(game.GameID)) return {};

		const gameElement = document.createElement('div');
		gameElement.id = 'game-div-' + game.DisplayName.replaceAll(' ', '_');
		gameElement.className += 'gamebox';
		gameElement.style.diplay = 'table';
		gamesElement.appendChild(gameElement);

		const gameBanner = document.createElement('img');

		let banner;
		if (fs.existsSync(__dirname.split('\\').slice(0, -3).join('\\') + '\\storage\\Cache\\Games\\Images')) {
			const dirs = fs.readdirSync(__dirname.split('\\').slice(0, -3).join('\\') + '\\storage\\Cache\\Games\\Images');
			const img = dirs.find(x => x.split('.')[0] === game.DisplayName);
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
			runCommand(`${game.Location}\\${game.Executable}`, game.Args);
		});

		game.Banner = banner;
		return game;
	}).filter((x) => x.Banner === '../icon.ico');

	document.querySelector('#game-loading-overlay').style.opacity = '0.9';
	ipcRenderer.send('load-banners-request', uncachedGames);
}

async function runCommand(command, args) {
	const { execFile } = require('child_process');
	const { promisify } = require('util');
	const res = await promisify(execFile)(command, args);
	return res;
}
module.exports = {
	loadGames,
};
