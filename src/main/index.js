require('v8-compile-cache');
const electron = require('electron');
const ipcMain = require('electron').ipcMain;
const app = electron.app;
const fs = require('fs');
const path = require('path');
const os = require('os');
const merge = require('deepmerge');
const axios = require('axios');

const STORAGE_PATH = path.join(__dirname, '../../storage');
const CFG_PATH = path.join(STORAGE_PATH, 'userprofile.json');

app.commandLine.appendSwitch('auto-detect', 'false');
app.commandLine.appendSwitch('no-proxy-server');

app.on('ready', () => {
    const mainWindow = new electron.BrowserWindow({
        width: 1150,
        height: 630,
        minWidth: 950,
        minHeight: 550,
        resizable: true,
        frame: false,
        show: false,
        title: 'Lazap',
        webPreferences: {
            nodeIntegration: true,
            contextIsolation: false,
            backgroundThrottling: false,
        },
        icon: path.join(__dirname, '../assets/img/icon.ico'),
    });

    mainWindow.loadFile(path.join(__dirname, '../renderer/login.html'));

    mainWindow.once('ready-to-show', () => {
        mainWindow.webContents.setZoomFactor(0.85);
        setTimeout(() => {
            mainWindow.show();
        }, 100);
    });

    mainWindow.webContents.on('did-finish-load', () => {
        mainWindow.webContents.setZoomFactor(0.85);
        handleStorageAndTransportData(mainWindow);
    });

    ipcMain.on('load-main', () => {
        mainWindow.loadFile(path.join(__dirname, '../renderer/index.html'));
    });

    ipcMain.on('close-window', () => {
        mainWindow.close();
    });

    ipcMain.on('max-window', () => {
        mainWindow.isMaximized()
            ? mainWindow.unmaximize()
            : mainWindow.maximize();
    });

    ipcMain.on('min-window', () => {
        mainWindow.minimize();
    });

    ipcMain.on('update-profile', (e, data) => {
        editLocalStorage(data);
    });
    // ipcMain.on('load-banners-request', async (e, r) => {
    // 	const res = fetch_banner(r);
    // 	res.forEach(async (url, i) => {
    // 		let banner_res = await url;
    // 		mainWindow.webContents.executeJavaScript(`
    // 		let banner_res = \'${banner_res}\';
    // 		const gameElement = document.querySelector(\'div#gamesList > div:nth-child(${i + 1})\');
    // 		gameElement.firstElementChild.setAttribute(\'src\', banner_res);
    // 		`);
    // 	});
    // });
});

function handleStorageAndTransportData(mainWindow) {
    if (fs.existsSync(STORAGE_PATH)) {
        const data = require(CFG_PATH);

        // If pfp is not a valid image, reset to default
        if (data.pfp !== 'default' && !fs.existsSync(data.pfp)) {
            const merged = merge(data, { pfp: 'default' });
            fs.writeFile(CFG_PATH, JSON.stringify(merged), (err) => {
                if (err) throw err;
                mainWindow.webContents.send('load-profile', merged);
            });
        } else {
            mainWindow.webContents.send('load-profile', data);
        }
    } else {
        fs.mkdirSync(STORAGE_PATH);

        const defaultCFG = {
            username: os.userInfo().username,
            pfp: 'default',
        };

        fs.writeFile(CFG_PATH, JSON.stringify(defaultCFG), (err) => {
            if (err) throw err;
            mainWindow.webContents.send('load-profile', defaultCFG);
        });
    }
}

function editLocalStorage(content) {
    if (fs.existsSync(STORAGE_PATH)) {
        fs.writeFile(CFG_PATH, JSON.stringify(content), (err) => {
            if (err) throw err;
        });
    } else {
        fs.mkdirSync(STORAGE_PATH);
        fs.writeFile(CFG_PATH, JSON.stringify(content), (err) => {
            if (err) throw err;
        });
    }
}

// function fetch_banner(data) {
// 	return data.map(async (r) => {
// 		let banner_res = await axios.post('http://localhost:3000/games/banner', r).catch(() => 0);
// 		if (!isNaN(banner_res)) {
// 			banner_res = `https://media-rockstargames-com.akamaized.net/tina-uploads/posts/51ko98182a41o9/ab7005bb38c318984e3003cdef14fee88ef1c014.jpg`;
// 		}
// 		else {
// 			banner_res = banner_res.data;
// 		}
// 		return banner_res;
// 	});
// }
