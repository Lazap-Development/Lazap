console.log('OS:', process.platform, 'Arch:', process.arch);

const Steam = require('./Steam.js');
const EpicGames = require('./EpicGames.js');
const RiotGames = require('./RiotGames.js');

async function loadGames() {
	const gamesElement = document.querySelector("div#gamesList");
	const games = [...(await Steam.getInstalledGames()), ...EpicGames.getInstalledGames()];

	if (games.length == 0) {
		var exists = document.getElementsByClassName('notFound')

		if (exists.length == 0) {
			const gamesNotFound = document.createElement('div')
			gamesNotFound.className = 'notFound';
			gamesNotFound.textContent = 'Unable to detect games in your computer!';
			return gamesElement.appendChild(gamesNotFound);
		}
	}

	if (gamesElement.children.length > 1) return;

	games.forEach((game) => {
		const gameElement = document.createElement('div');
		gameElement.id = 'game-div-' + game.DisplayName.replace(' ', '_');
		gameElement.className += "gamebox";
		gameElement.style.diplay = "table"
		gamesElement.appendChild(gameElement);
		
		const gameBanner = document.createElement('img');
		gameBanner.setAttribute("src", `https://media-rockstargames-com.akamaized.net/tina-uploads/posts/51ko98182a41o9/ab7005bb38c318984e3003cdef14fee88ef1c014.jpg`);
		gameBanner.className += "head-pic";
		gameElement.appendChild(gameBanner);

		const gameText = document.createElement('span');
		gameText.innerHTML = game.DisplayName.slice(0, 17) + (game.DisplayName.length > 17 ? '...' : '');
		gameElement.appendChild(gameText);

		gameBanner.addEventListener("click", () => {
			runCommand(`${game.Location}\\${game.Executable}`, game.Args);
		});
	});
}

async function runCommand(command, args) {
	const { execFile } = require('child_process');
	const { promisify } = require('util');
	let res = await promisify(execFile)(command, args);
	return res;
}
module.exports = {
	loadGames,
};