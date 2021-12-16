require('v8-compile-cache');
const electron = require('electron');
const ipcMain = require('electron').ipcMain;
const app = electron.app;
const fs = require('fs');

app.commandLine.appendSwitch('auto-detect', 'false');
app.commandLine.appendSwitch('no-proxy-server')

app.on('ready', () => {
    const mainWindow = new electron.BrowserWindow({
        width: 1150,
        height: 630,
        minWidth: 950,
        minHeight: 500,
        resizable: true,
        frame: false,
        show: false,
        title: "Lazap",
        webPreferences: {
            nodeIntegration: true,
            contextIsolation: false,
            backgroundThrottling: false,
        },
        icon: "icon.ico"
    });

    mainWindow.loadFile('src/login.html')

    mainWindow.once('ready-to-show', () => {
        mainWindow.webContents.setZoomFactor(.9);
        setTimeout(() => {

            mainWindow.show()
        }, 100);
    });

    mainWindow.webContents.on('did-finish-load', () => {
        handleStorageAndTransportData(mainWindow);
    });

    ipcMain.on('load-main', () => {
        mainWindow.loadFile('src/index.html')
    })
    
    ipcMain.on('close-window', () => {
        mainWindow.close();
    })
    ipcMain.on('max-window', () => {
        mainWindow.isMaximized() ? mainWindow.unmaximize() : mainWindow.maximize();
    })
    ipcMain.on('min-window', () => {
        mainWindow.minimize()
    })
    ipcMain.on('update-profile', (e, data) => {
        editLocalStorage(data);
    });
});

function handleStorageAndTransportData (mainWindow) {
    fs.readdir(`${__dirname}`, (err, data) => {
        if (data.includes('storage')) {
            const data = require(`${__dirname}/storage/userprofile.json`);
            mainWindow.webContents.send('load-profile', data);
        } else {
            fs.mkdirSync(`${__dirname}/storage`);
            const a = {
                username: "username",
                pfp: ""
            }
            fs.writeFile(`${__dirname}/storage/userprofile.json`, JSON.stringify(a), (err) => {
                if (err) {
                    throw err;
                }
                mainWindow.webContents.send('load-profile', a);
            });
        }
    })
}

function editLocalStorage (content) {
    fs.readdir(`${__dirname}`, (err, data) => {
        if (data.includes('storage')) {
            fs.writeFile(`${__dirname}/storage/userprofile.json`, JSON.stringify(content), (err) => {
                if (err) {
                    throw err;
                }
            });
        } else {
            fs.mkdirSync(`${__dirname}/storage`);
            fs.writeFile(`${__dirname}/storage/userprofile.json`, JSON.stringify(content), (err) => {
                if (err) {
                    throw err;
                }
            });
        }
    });
}
