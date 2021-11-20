require('v8-compile-cache');
const electron = require('electron');
const ipcMain = require('electron').ipcMain;
const app = electron.app;
//const delay = "3020"
const delay = "0"

app.commandLine.appendSwitch('auto-detect', 'false');
app.commandLine.appendSwitch('no-proxy-server')

app.on('ready', () => {
    const mainWindow = new electron.BrowserWindow({
        width: 1050,
        height: 580,
        minWidth: 850,
        minHeight: 450,
        resizable: true,
        frame: false,
        show: false,
        webPreferences: {
            nodeIntegration: true,
            contextIsolation: false,
            devTools: true,
        },
    });
    mainWindow.webContents.loadFile('src/splash.html')

    ipcMain.once('close-window', () => {
        mainWindow.close();
    })
    ipcMain.once('max_window', () => {
        mainWindow.maximize();
    })
    ipcMain.once('min_window', () => {
        mainWindow.minimize();
    })

    mainWindow.once('ready-to-show', () => {
        mainWindow.show()
        setTimeout(() => {
            mainWindow.loadFile('src/index.html')
        }, delay);
    })
});