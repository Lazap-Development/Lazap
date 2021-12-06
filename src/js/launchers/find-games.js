console.log('OS:', process.platform, 'Arch:', process.arch);

const Steam = require(__dirname + '\\Steam.js');
const EpicGames = require(__dirname + '\\EpicGames.js');
const RiotGames = require(__dirname + '\\RiotGames.js');

async function loadGames() {
	const gamesElement = document.querySelector("div#games");
	const games = [...(await Steam.getInstalledGames()), ...EpicGames.getInstalledGames(), ...(await RiotGames.getInstalledGames())];
	const gamesNotFound = document.createElement('p')
	gamesNotFound.textContent = 'Unable to detect games in your computer.';

	if (games.length == 0) {
		return gamesElement.appendChild(gamesNotFound);
	}

	if (gamesElement.children.length > 1) return;

	games.forEach((game) => {
		const gameElement = document.createElement('div');
		gameElement.id = 'game-div-' + game.DisplayName.replace(' ', '_');
		gameElement.textContent = game.DisplayName;
		gamesElement.appendChild(gameElement);

		const playbtn = document.createElement('button');

		playbtn.addEventListener("click", () => {
			runCommand(`${game.Location}\\${game.Executable}`, game.Args);
		});

		playbtn.textContent = 'Play';
		gameElement.appendChild(playbtn);
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