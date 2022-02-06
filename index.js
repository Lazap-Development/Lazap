/* eslint-disable indent */
require('v8-compile-cache');
const electron = require('electron');
const { ipcMain, Tray, Menu } = require('electron');
const app = electron.app;
const fs = require('fs');
const axios = require('axios').default;
const os = require('os');
const { JSDOM } = require('jsdom');
const rpc = require('discord-rpc');
const md5 = require('md5');
const CONSTANTS = require('./Constants.json');

app.commandLine.appendSwitch('auto-detect', 'false');
app.commandLine.appendSwitch('no-proxy-server');
const gotTheLock = app.requestSingleInstanceLock();
if (!gotTheLock) return app.quit();

const rpcClient = new rpc.Client({ transport: 'ipc' });
rpcClient.login({ clientId: '932504287337148417' });

handleHardwareAcceleration();

let tray = null;

app.on('ready', () => {
	const mainWindow = new electron.BrowserWindow({
		width: 1360,
		height: 630,
		minWidth: 1150,
		minHeight: 550,
		resizable: true,
		frame: false,
		show: false,
		title: 'Lazap',
		webPreferences: {
			nodeIntegration: true,
			contextIsolation: false,
			backgroundThrottling: false,
			zoomFactor: 0.9,
		},
		icon: 'icon.ico',
	});
	require('./updater.js')(mainWindow);

	mainWindow.loadFile(
		fs.existsSync('./storage/Settings/LauncherData.json')
			? JSON.parse(fs.readFileSync('./storage/Settings/LauncherData.json').toString())?.skipLogin
				? 'src/index.html'
				: 'src/login.html'
			: 'src/login.html',
	);

	mainWindow.once('ready-to-show', async () => {
		mainWindow.show();
	});

	mainWindow.webContents.on('did-finish-load', async () => {
		// Checking for auto-login
		/*
		if (fs.existsSync(__dirname + '\\storage\\Settings\\userprofile.js')) {
			if (require('./storage/Settings/userprofile.json').token) {
				mainWindow.webContents.send('check-if-logged-in', await identify());
			}
			else {
				return;
			}
		}
		*/
		handleStorageAndTransportData(mainWindow);
		if (JSON.parse(fs.readFileSync('./storage/Settings/LauncherData.json').toString())?.enableRPC) {
			updateRPC({
				details: 'On Main Screen',
				startTimestamp: Date.now(),
				largeImageKey: 'lazap',
			});
		}
	});

	ipcMain.on('load-main', () => {
		mainWindow.loadFile('src/index.html');
		// setTimeout(async () => mainWindow.webContents.send('check-for-login', data || await identify()), 2000);
	});
	ipcMain.on('load-custom', (e, str) => {
		mainWindow.loadFile(str);
	});
	ipcMain.on('load-login', async () => {
		mainWindow.loadFile('src/login.html');
		mainWindow.webContents.once('did-finish-load', () => mainWindow.webContents.send('replace-ignore-and-continue'));
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
	ipcMain.on('min-tray', () => {
		if (JSON.parse(fs.readFileSync('./storage/Settings/LauncherData.json').toString())?.trayMinLaunch === true) {
			tray = new Tray(__dirname + '/icon.ico');
			tray.setToolTip('Lazap');

			const contextMenu = Menu.buildFromTemplate([
				{ label: 'Show', type: 'normal',  click: () => {
					mainWindow.show()
					tray.destroy()
				} },
				{ label: 'Exit', type: 'normal', click: () => mainWindow.close() }
			])
			tray.setContextMenu(contextMenu)

			tray.on('click', () => {
				if (!mainWindow.isVisible()) {
					mainWindow.show();
					tray.destroy();
				}
			});

			mainWindow.hide();
		}
	});
	ipcMain.on('update-profile', (e, data) => {
		editLocalStorage(data);
	});
	ipcMain.on('signup-request', async (e, data) => {
		mainWindow.webContents.send('signup-response', await handleSignup(data));
	});
	ipcMain.on('signin-request', async (e, data) => {
		mainWindow.webContents.send('signin-response', await handleSignin(data));
	});
	ipcMain.on('login-identify', async () => {
		mainWindow.webContents.send('login-identify-response', await identify());
	});
	ipcMain.on('load-banners-request', async (e, r) => {
		const res = fetch_banner(r);
		res.forEach(async (url, i) => {
			mainWindow.webContents.executeJavaScript(`
			var banner_res = '${await url}';
			var gameElement = document.getElementById('game-div-${r[i].DisplayName}');
			gameElement.firstElementChild.setAttribute('src', banner_res);
		   `);
		});
		mainWindow.webContents.send('load-banners-response');
		mainWindow.webContents.send('load-main-banner');
	});
	ipcMain.on('rpcUpdate', (e, d) => updateRPC(d));
	ipcMain.on('setLaunchOnStartup', (e, bool) => app.setLoginItemSettings({ 'openAtLogin': bool, 'enabled': bool }));
	ipcMain.on('restart', async () => {
		app.relaunch();
		app.exit();
	});
});

ipcMain.on('updateSetting', (e, key, bool) => {
	const requireRestart = ['disableHardwareAcceleration'];
	checkForDirAndCreate(__dirname + '/storage/Settings/LauncherData.json');
	const LauncherData = JSON.parse(fs.readFileSync('./storage/Settings/LauncherData.json').toString());
	LauncherData[key] = bool;
	fs.writeFileSync('./storage/Settings/LauncherData.json', JSON.stringify(LauncherData));
	if (requireRestart.includes(key)) ipcMain.emit('restart');

	switch (key) {
		case 'enableRPC': {
			updateRPC(bool ? { details: 'On Main Screen', startTimestamp: Date.now(), largeImageKey: 'lazap' } : undefined);
			break;
		}
		case 'launchOnStartup': {
			ipcMain.emit('setLaunchOnStartup', bool);
			break;
		}
	}
});

function handleStorageAndTransportData(mainWindow) {
	checkForDirAndCreate(__dirname + '/storage/Settings/LauncherData.json', JSON.stringify(CONSTANTS.defaultLauncherData));
	checkForDirAndCreate(__dirname + '/storage/Settings/userprofile.json', '{}');

	let LauncherData = JSON.parse(fs.readFileSync('./storage/Settings/userprofile.json').toString());
	if (!Object.keys(LauncherData).length) {
		LauncherData = {
			username: os.userInfo().username,
			pfp: 'default',
		};
		fs.writeFileSync('./storage/Settings/userprofile.json', JSON.stringify(LauncherData));
	}
	else {
		if (LauncherData.pfp !== 'default' && !fs.existsSync(LauncherData.pfp)) {
			LauncherData.pfp = 'default';
		}
		fs.writeFileSync('./storage/Settings/userprofile.json', JSON.stringify(LauncherData));
	}

	mainWindow.webContents.send('load-profile', LauncherData);
}

function editLocalStorage(content) {
	checkForDirAndCreate(__dirname + '/storage/Settings/userprofile.json', '{}');
	fs.writeFile('./storage/Settings/userprofile.json', JSON.stringify(content), (err) => {
		if (err) throw err;
	});
}

function fetch_banner(data) {
	let arr = [];
	for (let i = 0; i < data.length; i++) {
		arr.push(new Promise((resolve) => {
			switch (data[i].LauncherName) {
				case 'EpicGames': {
					axios({
						url: `https://www.epicgames.com/store/en-US/browse?q=${encodeURIComponent(data[i].DisplayName)}&sortBy=releaseDate&sortDir=DESC&count=5&category=Game&start=0`,
						method: 'GET',
						responseType: 'arraybuffer',
					}).then(response => {
						const dom = new JSDOM(response.data);
						const elements = dom.window.document.querySelectorAll('#dieselReactWrapper > div > div > main > div > div > div > div > div > section > div > section > div > section > section > ul > li > div > div > div > a > div > div > div > div > div > img[alt]');
						let index;
						elements.forEach((element, _index) => {
							if (typeof index === 'number') return;
							if (element.getAttribute('alt') === data[i].DisplayName) {
								index = _index;
							}
							else if (element.getAttribute('alt').includes(data[i].DisplayName)) {
								index = _index;
							}
						});
						const element = elements.item(index) ?? elements.item(0);
						resolve(element?.getAttribute('data-image') ?? '../icon.ico');
					}).catch((err) => {
						console.warn('[BANNER]', err);
						resolve('../icon.ico');
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
				case 'Uplay': {
					let ubisoftified = data[i].DisplayName.replaceAll('_', ' ');
					if (data[i].DisplayName.replaceAll('_', ' ').match(/\d$/ig) && !data[i].DisplayName.replaceAll('_', ' ').replaceAll('\\d', '').endsWith(' ')) {
						const numlength = ubisoftified.split('').reverse().join('').match(/\d/ig)[0].length;
						ubisoftified = ubisoftified.slice(0, ubisoftified.length - numlength) + ' ' + ubisoftified.slice(ubisoftified.length - numlength);
					}

					/* Use Epic Games to get banners of Uplay Games for now, unless new alternative found */
					axios({
						url: `https://www.epicgames.com/store/en-US/browse?q=${encodeURIComponent(ubisoftified)}&sortBy=releaseDate&sortDir=DESC&count=5&category=Game&start=0`,
						method: 'GET',
						responseType: 'arraybuffer',
					}).then(response => {
						const dom = new JSDOM(response.data);
						const elements = dom.window.document.querySelectorAll('#dieselReactWrapper > div > div > main > div > div > div > div > div > section > div > section > div > section > section > ul > li > div > div > div > a > div > div > div > div > div > img[alt]');
						let index;
						elements.forEach((element, _index) => {
							const name = element.getAttribute('alt').split(' ').map(x => x.toLowerCase()).includes('edition') ? element.getAttribute('alt').split(' ').slice(0, element.getAttribute('alt').split(' ').length - element.getAttribute('alt').split(' ').reverse().findIndex(x => x.toLowerCase() === 'edition') - 2).join(' ') : element.getAttribute('alt');
							if (typeof index === 'number') return;

							const matched = name.toLowerCase().split('').map((str, _i) =>
								str === ubisoftified.toLowerCase().split('')[_i],
							).filter(x => x === true).length;

							// This makes it prioritize results that matched 100%, 90%, 75%
							if (((matched / name.length) * 100) === 100 && ubisoftified.toLowerCase() === name.toLowerCase()) {
								index = _index;
							}
							else if (((matched / name.length) * 100) > 90 && ((matched / name.length) * 100) !== 100) {
								index = _index;
							}
							else if (((matched / name.length) * 100) > 75 && !((matched / name.length) * 100) > 90) {
								index = _index;
							}
						});
						const element = elements.item(index) ?? elements.item(0);
						resolve(element?.getAttribute('data-image') ?? '../icon.ico');
					}).catch((err) => {
						console.warn('[BANNER]', err);
						resolve('../icon.ico');
					});
					break;
				}
				case 'Minecraft': {
					resolve('https://image.api.playstation.com/vulcan/img/cfn/11307uYG0CXzRuA9aryByTHYrQLFz-HVQ3VVl7aAysxK15HMpqjkAIcC_R5vdfZt52hAXQNHoYhSuoSq_46_MT_tDBcLu49I.png');
					break;
				}
				case 'FiveM': {
					resolve('https://logos-world.net/wp-content/uploads/2021/03/FiveM-Symbol.png');
					break;
				}
				case 'Lunar': {
					resolve('https://www.lunarclient.com/assets/img/default-twitter-icon.webp');
					break;
				}
			}
		}));
	}

	arr = arr.map(async x => await x);
	cacheBanners(data, arr.filter(async x => (await x) !== '../icon.ico'));
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
	checkForDirAndCreate(__dirname + '/storage/Cache/Games/Images');

	res.filter(async (x) => (await x).startsWith('http')).forEach(async (x, i) => {
		axios({
			url: await x,
			method: 'GET',
			responseType: 'stream',
		}).then(async (response) => {
			response.data.pipe(fs.createWriteStream(`./storage/Cache/Games/Images/${md5(data[i].DisplayName)}.png`));
		}).catch(() => '');
	});
}

async function handleSignup(data) {
	let deniedCode;
	const res = await axios.post('http://localhost:3000/accounts/add-account', data).catch(e => {
		deniedCode = e.response?.status;
	});
	return res ? res.status : deniedCode;
}

async function handleSignin(data) {
	let deniedCode;
	const res = await axios.post('http://localhost:3000/accounts/login', data).catch(e => {
		deniedCode = e.response?.status;
	});

	if (res) {
		// console.log(res.data);
		if (fs.existsSync('./storage/Settings/userprofile.json')) {
			const json = JSON.parse(fs.readFileSync('./storage/Settings/userprofile.json').toString());
			json['token'] = res.data;
			json['password'] = data.pass;
			json['username'] = data.username;
			editLocalStorage(json);
		}
		else {
			editLocalStorage({
				token: res.data,
			});
		}

		return res.status;
	}
	else {
		return deniedCode || null;
	}
}

async function identify() {
	if (!fs.existsSync('./storage/Settings/userprofile.json')) return { status: 'ACCOUNT_NOT_FOUND', data: null };
	const { token, password, username } = JSON.parse(fs.readFileSync('./storage/Settings/userprofile.json').toString());
	const res = await axios.post('http://localhost:3000/accounts/identify', { token, pass: password, name: username }).catch((e) => e.response?.status || 0);

	const errcodes = {
		401: 'INVALID_BODY',
		402: 'ACCOUNT_NOT_FOUND',
		200: 'SUCCESS',
		0: 'OFFLINE/API_DOWN',
	};

	return { data: res.data, status: errcodes[res.request?.res.statusCode] || 'OFFLINE/API_DOWN' };
}

function updateRPC(data) {
	checkForDirAndCreate(__dirname + '/storage/Settings/LauncherData.json', JSON.stringify(CONSTANTS.defaultLauncherData));
	const LauncherData = JSON.parse(fs.readFileSync('./storage/Settings/LauncherData.json').toString());
	if (LauncherData.enableRPC !== true && typeof data === 'object' && data !== null) return 'DISABLED';
	if (!data) {
		rpcClient.clearActivity();
		return;
	}
	rpcClient.setActivity(data).catch(err => console.warn('[RPC]', err));
}

function checkForDirAndCreate(dir, fileContent = '') {
	if (fs.existsSync(dir.split(__dirname)[1])) return true;
	dir.split(__dirname)[1].split('/').slice(1).forEach((name, i, arr) => {
		dir = dir.replaceAll('\\', '/');
		if (!fs.existsSync(`./${arr.slice(0, i + 1).join('/')}`)) {
			if (name.split('.')[1]) {
				fs.writeFileSync(`./${arr.slice(0, i + 1).join('/')}`, fileContent);
				return;
			}
			else {
				fs.mkdirSync(`./${arr.slice(0, i + 1).join('/')}`);
			}
		}
	});
}

async function handleHardwareAcceleration() {
	// fs.readFileSync('./storage/Settings/LauncherData.json').toString();
	checkForDirAndCreate(__dirname + '/storage/Settings/LauncherData.json', JSON.stringify(CONSTANTS.defaultLauncherData));
	if (JSON.parse(fs.readFileSync('./storage/Settings/LauncherData.json').toString())?.disableHardwareAcceleration === true) {
		app.disableHardwareAcceleration();
	}
}