require('v8-compile-cache');
const electron = require('electron');
const ipcMain = require('electron').ipcMain;
const app = electron.app;
const fs = require('fs');
const axios = require('axios');

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
	ipcMain.on('load-banners-request', async (e, r) => {
		const res = fetch_banner(r);
		res.forEach(async (url, i) => {
			const banner_res = await url;
			mainWindow.webContents.executeJavaScript(`
			const banner_res = \'${banner_res}\';
			const gameElement = document.querySelector(\'div#gamesList > div:nth-child(${i + 1})\');
			gameElement.firstElementChild.setAttribute(\'src\', banner_res);
			`);
		});
	});
});

function handleStorageAndTransportData(mainWindow) {
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

function editLocalStorage(content) {
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

function fetch_banner(data) {
	return data.map(async (r) => {
		let banner_res = await axios.post('http://localhost:3000/games/banner', r).catch(() => 0);
		if (!isNaN(banner_res)) {
			banner_res = `https://media-rockstargames-com.akamaized.net/tina-uploads/posts/51ko98182a41o9/ab7005bb38c318984e3003cdef14fee88ef1c014.jpg`;
		}
		else {
			banner_res = banner_res.data
		}
		return banner_res;
	});
}