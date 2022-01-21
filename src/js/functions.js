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

const searchbar = document.querySelector('div.search-bar > input[type="text"]');
// eslint-disable-next-line no-unused-vars
searchbar.addEventListener('keyup', () => {
	const query = searchbar.value;
	const allGames = document.querySelectorAll('#gamesList > div[id^="game-div"]');

	allGames.forEach((game) => {
		if (game.id.split('-').slice(2).join('-').replaceAll('_', ' ').match(new RegExp(`${query}`, 'gi'))) {
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