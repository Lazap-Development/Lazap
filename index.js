require('v8-compile-cache');
const electron = require('electron');
const app = electron.app;
const delay = "3020"
//const delay = "0"
app.commandLine.appendSwitch('auto-detect', 'false');
app.commandLine.appendSwitch('no-proxy-server')

app.on('ready', () => {
    const mainWindow = new electron.BrowserWindow({
        width: 1000,
        height: 510,
        minWidth: 850,
        minHeight: 450,
        resizable: true,
        frame: false,
        show: false,
        webPreferences: {
            nodeIntegration: true,
            devTools: true,
            contextIsolation: false,
        }
    });
    mainWindow.webContents.loadFile('src/splash.html')

    mainWindow.once('ready-to-show', () => {
        mainWindow.show()
        setTimeout(() => {
            mainWindow.loadFile('src/index.html')
        }, delay);
    })
});