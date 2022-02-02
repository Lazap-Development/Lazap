/* eslint-disable no-undef */
let img;

// eslint-disable-next-line no-unused-vars
function loadFile(event) {
	const image = document.getElementById('output');
	image.src = URL.createObjectURL(event.target.files[0]);
	img = event.target.files[0].path;

	ipcRenderer.send('update-profile', {
		username: document.getElementById('text').value,
		pfp: event.target.files[0].path,
	});
}

const searchbars = document.querySelectorAll('div.search-bar > input[type="text"]');

searchbars.item(0).addEventListener('keyup', () => {
	const query = searchbars.item(0).value;
	const allGames = document.querySelectorAll('#allGamesList > div[id^="game-div"]');

	allGames.forEach((game) => {
		if (game.id.split('-').slice(2).join('-').match(new RegExp(`${query}`, 'gi'))) {
			game.style.display = 'block';
		}
		else if (query.length === 0) {
			game.style.display = 'block';
		}
		else {
			game.style.display = 'none';
		}
	});
});
searchbars.item(1).addEventListener('keyup', () => {
	const query = searchbars.item(1).value;
	const allGames = document.querySelectorAll('#favGamesList > div[id^="game-div"]');

	allGames.forEach((game) => {
		if (game.id.split('-').slice(2).join('-').match(new RegExp(`${query}`, 'gi'))) {
			game.style.display = 'block';
		}
		else if (query.length === 0) {
			game.style.display = 'block';
		}
		else {
			game.style.display = 'none';
		}
	});
});

document.getElementById('text').addEventListener('change', (e) => {
	ipcRenderer.send('update-profile', {
		username: e.target.value,
		pfp: img,
	});
});

ipcRenderer.on('load-profile', (event, data) => {
	document.getElementById('output').src = data.pfp === 'default' ? '../img/default-profile.svg' : data.pfp;
	document.getElementById('text').value = data.username;
	if (!img) img = data.pfp;
});