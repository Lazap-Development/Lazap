const { ipcRenderer } = require('electron');

const fnScript = document.createElement('script');
fnScript.async = true;
fnScript.type = 'text/javascript';
fnScript.src = 'js/functions.js';
document.querySelector('head').appendChild(fnScript);

function close_window() {
    ipcRenderer.send('close-window');
}

function max_window() {
    ipcRenderer.send('max-window');
}

function min_window() {
    ipcRenderer.send('min-window');
}
