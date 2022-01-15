const marker = document.getElementById('indicator');
const items = document.querySelectorAll('.side-tab');
const home = document.getElementById('home');
const recent = document.getElementById('recent');
const games = document.getElementById('games');
const favs = document.getElementById('favs');
const friends = document.getElementById('friends');
const messages = document.getElementById('messages');
const activity = document.getElementById('activity');
const settings = document.getElementById('settings-popup');
const settingsbackblur = document.getElementById('settings-backblur');

window.onload = async function() {
	document.getElementById('main-loading-overlay').style.opacity = '0';
	document.getElementById('main-loading-overlay').style.visibility = 'hidden';
};

items.forEach((link) =>
	link.addEventListener('click', (e) => {
		marker.style.top = '0';
		marker.style.height = '0px';
		indicator(e.target);
	}),
);

document.getElementById('home-btn').addEventListener('click', function() {
	home.style.display = 'flex';
	recent.style.display = 'none';
	games.style.display = 'none';
	favs.style.display = 'none';
	friends.style.display = 'none';
	messages.style.display = 'none';
	activity.style.display = 'none';
});

document.getElementById('recent-btn').addEventListener('click', function() {
	home.style.display = 'none';
	recent.style.display = 'flex';
	games.style.display = 'none';
	favs.style.display = 'none';
	friends.style.display = 'none';
	messages.style.display = 'none';
	activity.style.display = 'none';
});

document.getElementById('games-btn').addEventListener('click', function() {
	home.style.display = 'none';
	recent.style.display = 'none';
	games.style.display = 'flex';
	favs.style.display = 'none';
	friends.style.display = 'none';
	messages.style.display = 'none';
	activity.style.display = 'none';

	if (document.getElementById('game-loading-overlay').style.opacity !== '0') {
		document.querySelector('.leftbar-overlay').style.opacity = '0.7';
		document.querySelector('.leftbar-overlay').style.visibility = 'visible';
	}
	setTimeout(async () => {
		await require('./js/launchers/find-games.js').loadGames();
	}, 400);
});

document.getElementById('favs-btn').addEventListener('click', function() {
	home.style.display = 'none';
	recent.style.display = 'none';
	games.style.display = 'none';
	favs.style.display = 'flex';
	messages.style.display = 'none';
	activity.style.display = 'none';
	friends.style.display = 'none';
});

document.getElementById('messages-btn').addEventListener('click', function() {
	home.style.display = 'none';
	recent.style.display = 'none';
	games.style.display = 'none';
	favs.style.display = 'none';
	messages.style.display = 'flex';
	activity.style.display = 'none';
	friends.style.display = 'none';
});

document.getElementById('activity-btn').addEventListener('click', function() {
	home.style.display = 'none';
	recent.style.display = 'none';
	games.style.display = 'none';
	favs.style.display = 'none';
	messages.style.display = 'none';
	activity.style.display = 'flex';
	friends.style.display = 'none';
});

document.getElementById('friends-btn').addEventListener('click', function() {
	home.style.display = 'none';
	recent.style.display = 'none';
	games.style.display = 'none';
	favs.style.display = 'none';
	messages.style.display = 'none';
	activity.style.display = 'none';
	friends.style.display = 'flex';
});

document.getElementById('settings-btn').addEventListener('click', function() {
	settingsbackblur.style.display = 'flex';
	settings.style.display = 'flex';
});

settingsbackblur.addEventListener('click', function() {
	settings.style.display = 'none';
	settingsbackblur.style.display = 'none';
});

items.forEach((link) =>
	link.addEventListener('click', (e) => {
		marker.style.top = '0';
		marker.style.height = '0px';
		indicator(e.target);
	}),
);

function indicator(item) {
	marker.style.top = item.offsetTop + 'px';
	marker.style.height = '30px';
}