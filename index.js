
const electron = require('electron');
const app = electron.app;

app.on('ready', () => {
    const mainWindow = new electron.BrowserWindow({width: 800, height: 600, frame: false});

    mainWindow.loadFile('src/index.html')
});