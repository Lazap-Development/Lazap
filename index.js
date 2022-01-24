require('v8-compile-cache');
const electron = require('electron');
const ipcMain = require('electron').ipcMain;
const app = electron.app;
const fs = require('fs');
const axios = require('axios').default;
const os = require('os');
const { JSDOM } = require('jsdom');
const rpc = require('discord-rpc');
const md5 = require('md5');

app.commandLine.appendSwitch('auto-detect', 'false');
app.commandLine.appendSwitch('no-proxy-server');

const rpcClient = new rpc.Client({ transport: 'ipc' });
rpcClient.login({ clientId: '932504287337148417' });

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

	mainWindow.webContents.on('did-finish-load', async () => {
		// Checking for auto-login
		if (fs.existsSync(__dirname + '\\storage\\Settings\\userprofile.js')) {
			if (require('./storage/Settings/userprofile.json').token) {
				mainWindow.webContents.send('check-if-logged-in', await identify());
			}
			else {
				return;
			}
		}
		handleStorageAndTransportData(mainWindow);
		updateRPC({
			details: 'Browsing games',
			startTimestamp: Date.now(),
			largeImageKey: 'lazap',
		});
	});

	ipcMain.on('load-main', (e, data) => {
		mainWindow.loadFile('src/index.html');
		setTimeout(async () => mainWindow.webContents.send('check-for-login', data || await identify()), 2000);
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
	});
});

function handleStorageAndTransportData(mainWindow) {
	if (!fs.existsSync(__dirname + '\\storage')) {
		fs.mkdirSync(__dirname + '\\storage');
		fs.mkdirSync(__dirname + '\\storage\\Settings');
		fs.writeFileSync(__dirname + '\\storage\\Settings\\userprofile.json', '{}');
	}
	if (!fs.existsSync(__dirname + '\\storage\\Settings')) {
		fs.mkdirSync(__dirname + '\\storage\\Settings');
		fs.writeFileSync(__dirname + '\\storage\\Settings\\userprofile.json', '{}');
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
	let arr = [];
	for (let i = 0; i < data.length; i++) {
		arr.push(new Promise((resolve) => {
			switch (data[i].LauncherName) {
			case 'EpicGames': {
				axios({
					url: `https://www.epicgames.com/store/en-US/browse?q=${encodeURIComponent(data[i].DisplayName)}&sortBy=releaseDate&sortDir=DESC&count=5`,
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
					console.log(err);
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
			}
		}));
	}

	arr = arr.map(async x => await x);
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
	if (!fs.existsSync(__dirname + '\\storage')) {
		fs.mkdirSync(__dirname + '\\storage');
		fs.mkdirSync(__dirname + '\\storage\\Cache');
		fs.mkdirSync(__dirname + '\\storage\\Cache\\Games');
		fs.mkdirSync(__dirname + '\\storage\\Cache\\Games\\Images');
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

	res.filter(async (x) => (await x).startsWith('http')).forEach(async (x, i) => {
		axios({
			url: await x,
			method: 'GET',
			responseType: 'stream',
		}).then(async (response) => {
			response.data.pipe(fs.createWriteStream(__dirname + `\\storage\\Cache\\Games\\Images\\${md5(data[i].DisplayName)}.png`));
		});
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
		if (fs.existsSync(__dirname + '\\storage\\Settings\\userprofile.json')) {
			const json = require(`${__dirname}/storage/Settings/userprofile.json`);
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
	if (!fs.existsSync(__dirname + '\\storage\\Settings\\userprofile.json')) return { status: 'ACCOUNT_NOT_FOUND', data: null };
	const { token, password, username } = JSON.parse(fs.readFileSync(__dirname + '\\storage\\Settings\\userprofile.json').toString());
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
	rpcClient.setActivity(data).catch(err => console.log(err));
}