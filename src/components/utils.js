async function getBannerResponse(force, id) {
	if (id !== 'allGames') return;
	const gamesList = document.querySelectorAll('#allGamesList > div');

	setTimeout(() => {
		document.getElementById('game-loading-overlay').style.opacity = '0';
		document.getElementById('game-loading-overlay').style.visibility = 'hidden';
	}, 100);
	setTimeout(() =>
		// eslint-disable-next-line max-nested-callbacks
		gamesList.forEach((gameElement) => gameElement.firstElementChild.style.opacity = '1'), 200,
	);
}

async function load_banners_request(r, id) {
	const res = require('../components/modules/banners').fetch_banner(r, await window.__TAURI__.path.appDir());
	const a = await Promise.all(res);
	/* res.forEach(async (url, i) => {
		mainWindow.webContents.executeJavaScript(`
			var gameElement = document.getElementById('game-div-${r[i].DisplayName}');
			gameElement.firstElementChild.setAttribute('src', '${await url}');
		   `);
	});*/
	load_banners_response(id, a.filter(async x => await x === 'https://cdn.discordapp.com/attachments/814938072999395388/983977458120396830/IMG_4432.jpg').length === a.length);
}

function load_banners_response(id, force) {
	if (id !== 'allGames') return;
	const gamesList = document.querySelectorAll('#allGamesList > div');
	const games = [];
	for (let i = 0; i < gamesList.length; i++) {
		if (gamesList.item(i).firstElementChild.getAttribute('src') === 'https://cdn.discordapp.com/attachments/814938072999395388/983977458120396830/IMG_4432.jpg') {
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
			} else if (game.getAttribute('src') !== 'https://cdn.discordapp.com/attachments/814938072999395388/983977458120396830/IMG_4432.jpg') {
				loaded++;
			}
			if (loaded === total) {
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
}


export {
	getBannerResponse,
	load_banners_request
}
