const { ipcRenderer } = require('electron');

const fnScript = document.createElement('script');
fnScript.async = true;
fnScript.type = 'text/javascript';
fnScript.src = 'js/functions.js';
document.querySelector('head').appendChild(fnScript);

// eslint-disable-next-line no-unused-vars
function close_window() {
	ipcRenderer.send('close-window');
}

// eslint-disable-next-line no-unused-vars
function max_window() {
	ipcRenderer.send('max-window');
}

// eslint-disable-next-line no-unused-vars
function min_window() {
	ipcRenderer.send('min-window');
}

ipcRenderer.on('load-banners-response', () => {
	const gamesList = document.querySelectorAll('#gamesList > div');
	const games = [];
	for (let i = 0; i < gamesList.length; i++) {
		if (gamesList.item(i).firstElementChild.getAttribute('src') === '../icon.ico') {
			games.push(gamesList.item(i).firstElementChild);
		}
	}
	const total = games.length;
	let loaded = 0;
	if (!games.length) {
		gamesList.forEach((gameElement) => {
			setTimeout(() => {
				gameElement.firstElementChild.style.opacity = '1';
			}, 200);
		});
		document.getElementById('game-loading-overlay').style.opacity = '0';
		document.getElementById('game-loading-overlay').style.visibility = 'hidden';
		return;
	}
	games.forEach((game) => {
		game.addEventListener('load', () => {
			loaded++;
			if ((loaded / 2) == total) {
				document.getElementById('game-loading-overlay').style.opacity = '0';
				document.getElementById('game-loading-overlay').style.visibility = 'hidden';
				setTimeout(() =>
					// eslint-disable-next-line max-nested-callbacks
					gamesList.forEach((gameElement) => gameElement.firstElementChild.style.opacity = '1'), 200,
				);
			}
		});
	});
});
