
const electron = require('electron');
const app = electron.app;

app.on('ready', () => {
    const mainWindow = new electron.BrowserWindow({width: 1100, height: 600, resizable: false,});

    mainWindow.loadFile('src/index.html')
});