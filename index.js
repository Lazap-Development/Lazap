require('v8-compile-cache');
const electron = require('electron');
const ipcMain = require('electron').ipcMain;
const app = electron.app;
const fs = require('fs');
const axios = require('axios').default;

app.commandLine.appendSwitch('auto-detect', 'false');
app.commandLine.appendSwitch('no-proxy-server');

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
		opacity: fs.existsSync(__dirname + '\\storage\\launcherprofile.json') ? require('./storage/launcherprofile.json')?.opacity / 100 : .89,
		webPreferences: {
			nodeIntegration: true,
			contextIsolation: false,
			backgroundThrottling: false,
		},
		icon: "icon.ico"
	});
	mainWindow.loadFile('src/login.html');

	mainWindow.once('ready-to-show', () => {
		mainWindow.webContents.setZoomFactor(.9);
		setTimeout(() => {
			mainWindow.show();
		}, 100);
	});

	mainWindow.webContents.on('did-finish-load', async () => {
		// Checking for auto-login
		if (fs.existsSync(__dirname + '\\storage\\userprofile.js')) {
			if (require('./storage/userprofile.json').token) {
				mainWindow.webContents.send('check-if-logged-in', await identify());
			}
			else {
				return;
			}
		};
		handleStorageAndTransportData(mainWindow);
	});

	ipcMain.on('load-main', (e, data) => {
		mainWindow.loadFile('src/index.html');
		setTimeout(async () => mainWindow.webContents.send('check-for-login', data || await identify()), 2000);
	});

	ipcMain.on('load-custom', (e, str) => {
		mainWindow.loadFile(str);
	});

	ipcMain.on('load-login', async (e, data) => {
		mainWindow.loadFile('src/login.html')
		mainWindow.webContents.once('did-finish-load', () => mainWindow.webContents.send('replace-ignore-and-continue'));
	});

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
	ipcMain.on('signup-request', async (e, data) => {
		mainWindow.webContents.send('signup-response', await handleSignup(data));
	});
	ipcMain.on('signin-request', async (e, data) => {
		mainWindow.webContents.send('signin-response', await handleSignin(data));
	});
});

function handleStorageAndTransportData(mainWindow) {
	if (fs.existsSync(__dirname + '\\storage') && fs.existsSync(__dirname + '\\storage\\userprofile.json')) {
		const data = require(`${__dirname}/storage/userprofile.json`);
		mainWindow.webContents.send('load-profile', data);
	} else {
		if (!fs.existsSync(__dirname + '\\storage')) {
			fs.mkdirSync(`${__dirname}/storage`);
		}
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

async function handleSignup(data) {
	let deniedCode;
	const res = await axios.post('http://localhost:3000/accounts/add-account', data).catch(e => {
		deniedCode = e.response?.status
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
		if (fs.existsSync(__dirname + '\\storage\\userprofile.json')) {
			const json = require(`${__dirname}/storage/userprofile.json`);
			json['token'] = res.data;
			json['password'] = data.pass;
			json['username'] = data.username;
			editLocalStorage(json);
		} else {
			editLocalStorage({
				token: res.data,
			});
		}

		return res.status;
	} else {
		return deniedCode || null;
	}
}

async function identify() {
	let res;

	if (!fs.existsSync(__dirname + '\\storage\\userprofile.json')) return { status: 'ACCOUNT_NOT_FOUND', data: null };
	const { token, password, username } = JSON.parse(fs.readFileSync(__dirname + '\\storage\\userprofile.json').toString());
	res = await axios.post('http://localhost:3000/accounts/identify', { token, pass: password, name: username }).catch((e) => e.response?.status || 0);

	const errcodes = {
		401: 'INVALID_BODY',
		402: 'ACCOUNT_NOT_FOUND',
		200: 'SUCCESS',
		0: 'OFFLINE/API_DOWN'
	};

	return { data: res.data, status: errcodes[res.request?.res.statusCode] || 'OFFLINE/API_DOWN' };
}