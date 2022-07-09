const marker = document.getElementById('indicator');
const home = document.getElementById('home');
const recent = document.getElementById('recent');
const games = document.getElementById('games');
const favs = document.getElementById('favs');
const friends = document.getElementById('friends');
const messages = document.getElementById('messages');
const activity = document.getElementById('activity');
const settings = document.getElementById('settings-popup');
const alertbox = document.getElementById('alertbox');
const settingsbackblur = document.getElementById('settings-backblur');

const { ipcRenderer } = require('electron');
const fs = require('fs');

let userDataPath;
(async () => {
	const result = await ipcRenderer.invoke('read-path');
	userDataPath = result;
})();

window.onload = async function() {
	// Cache games first before loading
	await require('./js/launchers/find-games.js').getInstalledGames();
	document.getElementById('main-loading-overlay').style.opacity = '0';
	document.getElementById('main-loading-overlay').style.visibility = 'hidden';
	setTimeout(() => require('./js/launchers/find-games.js').loadGames('recentGamesListMainPage'), 200);
	document.querySelectorAll('.side-tab').forEach((link) =>
		link.addEventListener('click', (e) => {
			marker.style.top = '0';
			marker.style.height = '0px';
			indicator(e.target);
		}),
	);

	document.getElementById('alertboxexit').addEventListener('click', function() {
		alertbox.style.display = 'none';
	});

	document.getElementById('home-btn').addEventListener('click', async function() {
		home.style.display = 'flex';
		recent.style.display = 'none';
		games.style.display = 'none';
		favs.style.display = 'none';
		friends.style.display = 'none';
		messages.style.display = 'none';
		activity.style.display = 'none';

		ipcRenderer.send('rpcUpdate', {
			details: 'On Main Screen',
			startTimestamp: Date.now(),
			largeImageKey: 'lazap',
			smallImageKey: 'home',
			largeImageText: 'Lazap',
			smallImageText: 'Home Screen',
		});

		await require('./js/launchers/find-games.js').loadGames('recentGamesListMainPage');
	});

	document.getElementById('recent-btn').addEventListener('click', async function() {
		home.style.display = 'none';
		recent.style.display = 'flex';
		games.style.display = 'none';
		favs.style.display = 'none';
		friends.style.display = 'none';
		messages.style.display = 'none';
		activity.style.display = 'none';

		ipcRenderer.send('rpcUpdate', {
			details: 'Browsing Recently Played',
			startTimestamp: Date.now(),
			largeImageKey: 'lazap',
			smallImageKey: 'recent',
			largeImageText: 'Lazap',
			smallImageText: 'Recently Played',
		});

		await require('./js/launchers/find-games.js').loadGames('recentGames');
	});

	document.getElementById('games-btn').addEventListener('click', async function() {
		home.style.display = 'none';
		recent.style.display = 'none';
		games.style.display = 'flex';
		favs.style.display = 'none';
		friends.style.display = 'none';
		messages.style.display = 'none';
		activity.style.display = 'none';

		ipcRenderer.send('rpcUpdate', {
			details: 'Browsing All Games',
			startTimestamp: Date.now(),
			largeImageKey: 'lazap',
			smallImageKey: 'games',
			largeImageText: 'Lazap',
			smallImageText: 'All Games',
		});

		await require('./js/launchers/find-games.js').loadGames('allGames');
	});

	document.getElementById('favs-btn').addEventListener('click', async function() {
		home.style.display = 'none';
		recent.style.display = 'none';
		games.style.display = 'none';
		favs.style.display = 'flex';
		messages.style.display = 'none';
		activity.style.display = 'none';
		friends.style.display = 'none';

		ipcRenderer.send('rpcUpdate', {
			details: 'Browsing Favourite Games',
			startTimestamp: Date.now(),
			largeImageKey: 'lazap',
			smallImageKey: 'favs',
			largeImageText: 'Lazap',
			smallImageText: 'Favourites',
		});

		await require('./js/launchers/find-games.js').loadGames('favGames');
	});

	document.getElementById('messages-btn').addEventListener('click', async function() {
		home.style.display = 'none';
		recent.style.display = 'none';
		games.style.display = 'none';
		favs.style.display = 'none';
		messages.style.display = 'flex';
		activity.style.display = 'none';
		friends.style.display = 'none';
	});

	document.getElementById('activity-btn').addEventListener('click', async function() {
		home.style.display = 'none';
		recent.style.display = 'none';
		games.style.display = 'none';
		favs.style.display = 'none';
		messages.style.display = 'none';
		activity.style.display = 'flex';
		friends.style.display = 'none';
	});

	document.getElementById('friends-btn').addEventListener('click', async function() {
		home.style.display = 'none';
		recent.style.display = 'none';
		games.style.display = 'none';
		favs.style.display = 'none';
		messages.style.display = 'none';
		activity.style.display = 'none';
		friends.style.display = 'flex';
	});

	document.getElementById('settings-btn').addEventListener('click', async function() {
		settingsbackblur.style.display = 'flex';
		settings.style.display = 'flex';
	});

	settingsbackblur.addEventListener('click', function() {
		settings.style.display = 'none';
		settingsbackblur.style.display = 'none';
	});

	document.querySelector('.titlebar-settings').addEventListener('click', () => {
		const Data = JSON.parse(fs.readFileSync(userDataPath + '/storage/Settings/LauncherData.json').toString());
		document.querySelectorAll('input[id^=setting-]').forEach((input) => {
			input.checked = Data[input.id.split('-')[1]] ? true : false;
		});
	});
	document.querySelectorAll('input[id^=setting-]').forEach((input) => {
		input.addEventListener('change', () => {
			ipcRenderer.send('updateSetting', input.id.split('-')[1], document.querySelector(`input[id=${input.id}]`).checked);
		});
	});
};

function indicator(item) {
	marker.style.top = item.offsetTop + 'px';
	marker.style.height = '30px';
}