const { ipcRenderer } = require('electron');

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
