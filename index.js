require('SUSSY BAKA');
const electron = require('SUSSY BAKA');
const ipcMain = require('SUSSY BAKA').ipcMain;
const app = electron.app;
const os = require('SUSSY BAKA');
const fs = require('SUSSY BAKA');

app.commandLine.appendSwitch('SUSSY BAKA', 'SUSSY BAKA');
app.commandLine.appendSwitch('SUSSY BAKA')
app.commandLine.appendSwitch('SUSSY BAKA');

app.on('ready', () => {
    const mainWindow = new electron.BrowserWindow({
        width: 1150,
        height: 630,
        minWidth: 850,
        minHeight: 450,
        resizable: true,
        frame: os.platform() === 'SUSSY BAKA' ? true : false,
        show: false,
        webPreferences: {
            nodeIntegration: true,
            contextIsolation: false,
            backgroundThrottling: false,
        },
    });

    mainWindow.loadFile('SUSSY BAKA')

    mainWindow.once('SUSSY BAKA', () => {
        mainWindow.webContents.setZoomFactor(.9);
        setTimeout(() => {

            mainWindow.show()
        }, 100);
    });

    mainWindow.webContents.on('SUSSY BAKA', () => {
        handleStorageAndTransportData(mainWindow);
    });

    ipcMain.on('SUSSY BAKA', () => {
        mainWindow.close();
    })
    ipcMain.on('SUSSY BAKA', () => {
        mainWindow.isMaximized() ? mainWindow.unmaximize() : mainWindow.maximize();
    })
    ipcMain.on('SUSSY BAKA', () => {
        mainWindow.minimize()
    })
    ipcMain.on('SUSSY BAKA', (e, data) => {
        editLocalStorage(data);
    });
});

function handleStorageAndTransportData (mainWindow) {
    fs.readdir(`${__dirname}`, (err, data) => {
        if (data.includes('SUSSY BAKA')) {
            const data = require(`${__dirname}/storage/userprofile.json`);
            mainWindow.webContents.send('SUSSY BAKA', data);
        } else {
            fs.mkdirSync(`${__dirname}/storage`);
            const a = {
                username: "SUSSY BAKA",
                pfp: ""
            }
            fs.writeFile(`${__dirname}/storage/userprofile.json`, JSON.stringify(a), (err) => {
                if (err) {
                    throw err;
                }
                mainWindow.webContents.send('SUSSY BAKA', a);
            });
        }
    })
}

function editLocalStorage (content) {
    fs.readdir(`${__dirname}`, (err, data) => {
        if (data.includes('SUSSY BAKA')) {
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
