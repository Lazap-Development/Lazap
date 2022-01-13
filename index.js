require('v8-compile-cache');
const electron = require('electron');
const ipcMain = require('electron').ipcMain;
const app = electron.app;
const fs = require('fs');
const axios = require('axios').default;
const os = require('os');
const { JSDOM } = require('jsdom');

app.commandLine.appendSwitch('auto-detect', 'false');
app.commandLine.appendSwitch('no-proxy-server');
app.commandLine.appendSwitch('high-dpi-support', 1);
app.commandLine.appendSwitch('force-device-scale-factor', 1);

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
		icon: 'icon.ico',
	});

	mainWindow.loadFile('src/login.html');

	mainWindow.once('ready-to-show', () => {
		mainWindow.webContents.setZoomFactor(0.9);
		setTimeout(() => {

			mainWindow.show();
		}, 100);
	});

	mainWindow.webContents.on('did-finish-load', () => {
		handleStorageAndTransportData(mainWindow);
	});

	ipcMain.on('load-main', () => {
		mainWindow.loadFile('src/index.html');
	});

	ipcMain.on('close-window', () => {
		mainWindow.close();
	});
	ipcMain.on('max-window', () => {
		mainWindow.isMaximized() ? mainWindow.unmaximize() : mainWindow.maximize();
	});
	ipcMain.on('min-window', () => {
		mainWindow.minimize();
	});
	ipcMain.on('update-profile', (e, data) => {
		editLocalStorage(data);
	});
	ipcMain.on('load-banners-request', async (e, r) => {
		const res = fetch_banner(r);
		res.forEach(async (url, i) => {
			mainWindow.webContents.executeJavaScript(`
			var banner_res = '${await url}';
			var gameElement = document.getElementById('game-div-${r[i].DisplayName.replaceAll(' ', '_')}');
			gameElement.firstElementChild.setAttribute('src', banner_res);
		   `);
		});
		mainWindow.webContents.send('load-banners-response');
	});
});

function handleStorageAndTransportData(mainWindow) {
	if (!fs.existsSync(__dirname + '\\storage')) {
		fs.mkdirSync(__dirname + '\\storage');
		fs.mkdirSync(__dirname + '\\storage\\Settings');
		fs.mkdirSync(__dirname + '\\storage\\Cache');
		fs.mkdirSync(__dirname + '\\storage\\Cache\\Games');
		fs.mkdirSync(__dirname + '\\storage\\Cache\\Games\\Images');
		fs.writeFileSync(__dirname + '\\storage\\Settings\\userprofile.json', '{}');
	}
	if (!fs.existsSync(__dirname + '\\storage\\Settings')) {
		fs.mkdirSync(__dirname + '\\storage\\Settings');
		fs.writeFileSync(__dirname + '\\storage\\Settings\\userprofile.json', '{}');
	}
	if (!fs.existsSync(__dirname + '\\storage\\Cache')) {
		fs.mkdirSync(__dirname + '\\storage\\Cache');
		fs.mkdirSync(__dirname + '\\storage\\Cache\\Games');
		fs.mkdirSync(__dirname + '\\storage\\Cache\\Games\\Images');
	}
	if (!fs.existsSync(__dirname + '\\storage\\Cache\\Games')) {
		fs.mkdirSync(__dirname + '\\storage\\Cache\\Games');
		fs.mkdirSync(__dirname + '\\storage\\Cache\\Games\\Images');
	}
	if (!fs.existsSync(__dirname + '\\storage\\Cache\\Games\\Images')) {
		fs.mkdirSync(__dirname + '\\storage\\Cache\\Games\\Images');
	}

	let LauncherData = require(`${__dirname}\\storage\\Settings\\userprofile.json`);
	if (!Object.keys(LauncherData).length) {
		LauncherData = {
			username: os.userInfo().username,
			pfp: 'default',
		};
		fs.writeFileSync(__dirname + '\\storage\\Settings\\userprofile.json', JSON.stringify(LauncherData));
	}
	else {
		if (LauncherData.pfp !== 'default' && !fs.existsSync(LauncherData.pfp)) {
			LauncherData.pfp = 'default';
		}
		fs.writeFileSync(__dirname + '\\storage\\Settings\\userprofile.json', JSON.stringify(LauncherData));
	}

	mainWindow.webContents.send('load-profile', LauncherData);
}

function editLocalStorage(content) {
	if (!fs.existsSync(__dirname + '\\storage')) {
		fs.mkdirSync(__dirname + '\\storage');
	}
	fs.writeFile(__dirname + '\\storage\\Settings\\userprofile.json', JSON.stringify(content), (err) => {
		if (err) throw err;
	});
}

function fetch_banner(data) {
	const arr = [];
	for (let i = 0; i < data.length; i++) {
		arr.push(new Promise((resolve, reject) => {
			switch (data[i].LauncherName) {
			case 'EpicGames': {
				axios({
					url: `https://www.epicgames.com/store/en-US/browse?q=${encodeURIComponent(data[i].DisplayName)}&sortBy=releaseDate&sortDir=DESC&count=5`,
					method: 'GET',
					responseType: 'arraybuffer',
				}).then(response => {
					const dom = new JSDOM(response.data);
					const element = dom.window.document.querySelector(`#dieselReactWrapper > div > div > main > div > div > div > div > div > section > div > section > div > section > section > ul > li > div > div > div > a > div > div > div > div > div > img[alt="${data[i].DisplayName}"]`);
					resolve(element.getAttribute('data-image'));
				}).catch((err) => {
					console.log(err);
					reject('UNABLE_TO_GET_DATA');
				});
				break;
			}
			case 'Steam': {
				resolve(`https://cdn.akamai.steamstatic.com/steam/apps/${data[i].GameID}/header.jpg`);
				break;
			}
			case 'RiotGames': {
				switch (data[i].GameID) {
				case 'Valorant': {
					resolve('https://valorant-config.fr/wp-content/uploads/2020/05/7d604cf06abf5866f5f3a2fbd0deacf9-200x300.png');
					break;
				}
				}
				break;
			}
			}
		}));
	}

	cacheBanners(data, arr);
	return arr;
	/*
	const res = data.map(async (r) => {
		let banner_res = await axios.post('http://localhost:3000/games/banner', r).catch(() => 0);
		if (!isNaN(banner_res)) {
			banner_res = '../icon.ico';
		} else {
			banner_res = banner_res.data;
		}
		return banner_res;
	});
	cacheBanners(data, res);
	return res;
	*/
}

function cacheBanners(data, res) {
	res.filter(async (x) => (await x).startsWith('http')).forEach(async (x, i) => {
		axios({
			url: await x,
			method: 'GET',
			responseType: 'stream',
		}).then(async (response) => {
			response.data.pipe(fs.createWriteStream(__dirname + `\\storage\\Cache\\Games\\Images\\${data[i].DisplayName}.${(await x).split('.')[(await x).split('.').length - 1].slice(0, 3) ?? 'jpg'}`));
		});
	});
}