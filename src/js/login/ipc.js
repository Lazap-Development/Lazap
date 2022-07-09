const { ipcRenderer } = require('electron');

// eslint-disable-next-line no-unused-vars
const load_main = () => {
	ipcRenderer.send('load-main');
};
// eslint-disable-next-line no-unused-vars
const close_window = () => {
	ipcRenderer.send('close-window');
};
// eslint-disable-next-line no-unused-vars
const max_window = () => {
	ipcRenderer.send('max-window');
};
// eslint-disable-next-line no-unused-vars
const min_window = () => {
	ipcRenderer.send('min-window');
};

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
