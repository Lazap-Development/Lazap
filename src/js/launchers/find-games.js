console.log('OS:', process.platform, 'Arch:', process.arch);
const { ipcRenderer } = require('electron');
const Steam = require('./Steam.js');
const EpicGames = require('./EpicGames.js');
const RiotGames = require('./RiotGames.js');

let games;

async function loadGames() {
	games = [...(await Steam.getInstalledGames()), ...EpicGames.getInstalledGames()];

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

	// ipcRenderer.send('load-banners-request', games.map(x => { return { name: x.DisplayName, id: x.GameID }; }));
}

async function addGames() {
    const gamesElement = document.querySelector("div#gamesList");
    if (gamesElement.children.length >= 1) return;

	games.forEach((game) => {
        console.log(game);
		const gameElement = document.createElement('div');
		gameElement.id = 'game-div-' + game.DisplayName.replace(' ', '_');
		gameElement.className += "gamebox";
		gameElement.style.diplay = "table"
		gamesElement.appendChild(gameElement);

		const gameBanner = document.createElement('img');
        if (game.LauncherName === 'Steam') {
            gameBanner.setAttribute("src", `https://cdn.akamai.steamstatic.com/steam/apps/${game.GameID}/header.jpg`);
        } else {
            gameBanner.setAttribute("src", `https://www.powerpyx.com/wp-content/uploads/gta-3-definitive-edition-wallpaper.jpg`);
        }
		
		gameBanner.height = 500;
		gameBanner.width = 500;
		gameElement.appendChild(gameBanner);

		const gameText = document.createElement('span');
		if (game.DisplayName.length > 25) {
			gameText.innerHTML = game.DisplayName.slice(0, 25);
			gameText.innerHTML += `...`;
		} else {
			gameText.innerHTML = game.DisplayName
		}
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
    addGames
};