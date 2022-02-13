// eslint-disable-next-line no-unused-vars
function load_main() {
	ipcRenderer.send('load-main');
}
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

const { ipcRenderer } = require('electron');

ipcRenderer.on('alert', (e, msg) => {
	const alert = document.querySelector('.alert-box');
	alert.firstElementChild.lastElementChild.firstElementChild.textContent = msg;
	alert.style.display = 'flex';
});

ipcRenderer.on('replace-ignore-and-continue', () => {
	document.querySelector('button.ignore').textContent = 'Back To Launcher';
});

ipcRenderer.on('check-if-logged-in', async (e, r) => {
	const loggedIn = r;

	if (loggedIn.status === 'SUCCESS') {
		ipcRenderer.send('load-main', r);
	}
	else {
		ipcRenderer.emit('alert', null, 'Something went wrong while trying to login!');
	}
});
