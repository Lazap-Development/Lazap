const fnScript = document.createElement('script');
fnScript.async = true;
fnScript.type = 'text/javascript';
fnScript.src = 'js/functions.js';
document.querySelector('head').appendChild(fnScript);

// eslint-disable-next-line no-unused-vars
const close_window = () => {
	ipcRenderer.send('close-window');
}

// eslint-disable-next-line no-unused-vars
const max_window = () => {
	ipcRenderer.send('max-window');
}

// eslint-disable-next-line no-unused-vars
const min_window = () => {
	ipcRenderer.send('min-window');
}

const { ipcRenderer } = require('electron');

ipcRenderer.on('check-for-login', async (e, r) => {
	loggedin = r.status === 'SUCCESS';
});

ipcRenderer.on('handle-update-available', () => {
	document.querySelector('img.titlebar-update#update-btn').style.display = 'block';
});
document.querySelector('img.titlebar-update#update-btn').addEventListener('click', () => ipcRenderer.send('handle-update-install'));

const account = document.getElementById('account-btn');
let loggedin;

account.addEventListener('mouseover', () => {
	if (loggedin === true) {
		account.style['filter'] = 'invert(17%) sepia(86%) saturate(3285%) hue-rotate(239deg) brightness(85%) contrast(101%)';
	}
	else {
		account.style['filter'] = 'invert(19%) sepia(87%) saturate(4716%) hue-rotate(355deg) brightness(99%) contrast(104%)';
	}
});
account.addEventListener('mouseout', () => account.style['filter'] = 'none');
account.addEventListener('click', () => ipcRenderer.send('load-login'));

ipcRenderer.on('load-banners-response', (e, id, force) => {
	if (id !== 'allGames') return;
	const gamesList = document.querySelectorAll('#allGamesList > div');
	const games = [];
	for (let i = 0; i < gamesList.length; i++) {
		if (gamesList.item(i).firstElementChild.getAttribute('src') === '../icon.ico') {
			games.push(gamesList.item(i).firstElementChild);
		}
	}
	const total = games.length;
	let loaded = 0;
	if (games.length === 0) {
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
			if (force) {
				loaded++;
			}
			else if (game.getAttribute('src') !== '../icon.ico') {
				loaded++;
			}
			if (loaded == total) {
				setTimeout(() => {
					document.getElementById('game-loading-overlay').style.opacity = '0';
					document.getElementById('game-loading-overlay').style.visibility = 'hidden';
				}, 100);
				setTimeout(() =>
					// eslint-disable-next-line max-nested-callbacks
					gamesList.forEach((gameElement) => gameElement.firstElementChild.style.opacity = '1'), 200,
				);
			}
		});
	});
});