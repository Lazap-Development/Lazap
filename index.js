require('v8-compile-cache');
const electron = require('electron');
const ipcMain = require('electron').ipcMain;
const app = electron.app;
const os = require('os');

app.commandLine.appendSwitch('auto-detect', 'false');
app.commandLine.appendSwitch('no-proxy-server')

app.on('ready', () => {
    const mainWindow = new electron.BrowserWindow({
        width: 1100,
        height: 580,
        minWidth: 850,
        minHeight: 450,
        resizable: true,
        frame: findOS(),
        show: false,
        webPreferences: {
            nodeIntegration: true,
            contextIsolation: false,
        },
    });
    mainWindow.webContents.loadFile('src/splash.html')

    ipcMain.on('close-window', () => {
        mainWindow.close();
    })
    ipcMain.on('max-window', () => {
        mainWindow.isMaximized() ? mainWindow.unmaximize() : mainWindow.maximize();
    })
    ipcMain.on('min-window', () => {
        mainWindow.minimize()
    })

    mainWindow.once('ready-to-show', () => {
        mainWindow.show()
        setTimeout(() => {
            mainWindow.loadFile('src/index.html')
        }, 2000);
    })
});

function findOS() {
    let result;
    
    if (os.platform() === "darwin") {
        result = true;
    } else {
        result = false;
    }
    return result;
}