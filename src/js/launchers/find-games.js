console.log('OS:', process.platform, 'Arch:', process.arch);
console.log(__dirname);
const fs = require('fs');
const Steam = require(__dirname + '\\Steam.js');
const EpicGames = require(__dirname + '\\EpicGames.js');

async function loadGames() {
	const gamesElement = document.querySelector("div#games");
	const games = [...(await Steam.getInstalledGames()), ...EpicGames.getInstalledGames()];
	console.log(games)
	games.forEach((game) => {
		const gameElement = document.createElement('div');
		gameElement.id = 'game-div-' + game.DisplayName.replace(' ', '_');
		gameElement.textContent = game.DisplayName;
		gamesElement.appendChild(gameElement);

		const playbtn = document.createElement('button');

		playbtn.addEventListener("click", () => {
			runCommand(`${game.Location}\\${game.Executable}`);
		});

		playbtn.textContent = 'Play';
		gameElement.appendChild(playbtn);
	});
}

async function runCommand(command) {
	const { exec } = require('child_process');
	const { promisify } = require('util');
	let res = await promisify(exec)(command);
	return res;
}

module.exports = {
	loadGames,
};