const electron = require('electron');
const app = electron.app;
require('v8-compile-cache');

app.on('ready', () => {
    const mainWindow = new electron.BrowserWindow({
        width: 1100,
        height: 600,
        resizable: true,
        transparent: true,
        show: true
    });
    mainWindow.loadFile('src/splash.html')

    mainWindow.once('ready-to-show', () => {
        setTimeout(() => {
            mainWindow.loadFile('src/index.html')
        }, 2000);
    })
    
});