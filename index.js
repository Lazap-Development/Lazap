/* eslint-disable indent */
require('v8-compile-cache');
const { ipcMain, Tray, Menu, app, BrowserWindow } = require('electron');

app.commandLine.appendSwitch('auto-detect', 'false');
app.commandLine.appendSwitch('no-proxy-server');
const gotTheLock = app.requestSingleInstanceLock();
if (!gotTheLock) return app.quit();

let tray = null;

app.on('ready', () => {
	const mainWindow = new BrowserWindow({
		width: 1360,
		height: 630,
		minWidth: 1150,
		minHeight: 550,
		resizable: true,
		frame: false,
		show: false,
		title: 'Lazap Nightly',
		webPreferences: {
			nodeIntegration: true,
			contextIsolation: false,
			backgroundThrottling: false,
			zoomFactor: 0.9,
		},
		icon: __dirname + (process.platform === 'linux' ? '/img/icons/icon.png' : '/img/icons/icon.ico'),
	});

	mainWindow.loadFile(
		fs.existsSync('./storage/Settings/LauncherData.json')
			? JSON.parse(fs.readFileSync('./storage/Settings/LauncherData.json').toString())?.skipLogin
				? 'src/index.html'
				: 'src/login.html'
			: 'src/login.html',
	);

	mainWindow.once('ready-to-show', async () => {
		mainWindow.show();

		tray = new Tray(__dirname + (process.platform === 'linux' ? '/img/icons/icon.png' : '/img/icons/icon.ico'));
		tray.setToolTip('Lazap');

		const contextMenu = Menu.buildFromTemplate([
			{
				label: 'Show', type: 'normal', click: () => {
					mainWindow.show();
				},
			},
			{ label: 'get sussy amogus/hide', type: 'normal', click: () => mainWindow.hide() },
			{ label: 'Exit', type: 'normal', click: () => mainWindow.close() },
		]);
		tray.setContextMenu(contextMenu);

		tray.on('click', () => {
			if (!mainWindow.isVisible()) {
				mainWindow.show();
			}
		});
	});

	mainWindow.webContents.on('did-finish-load', async () => {
		handleStorageAndTransportData(mainWindow);
		if (!fs.existsSync('./storage/Settings/LauncherData.json')) {
			checkForDirAndCreate(__dirname + '/storage/Settings/LauncherData.json', '{}');
		}
		if (JSON.parse(fs.readFileSync('./storage/Settings/LauncherData.json').toString())?.enableRPC) {
			updateRPC({
				details: 'On Main Screen',
				startTimestamp: Date.now(),
				largeImageKey: 'lazap',
			});
		}
		require('./src/modules/updater.js')(mainWindow);
	});

	eventHandler(ipcMain, app);

	ipcMain.on('load-main', () => {
		mainWindow.loadFile('src/index.html');
	});
	ipcMain.on('load-custom', (e, str) => {
		mainWindow.loadFile(str);
	});
	ipcMain.on('load-login', async () => {
		mainWindow.loadFile('src/login.html');
		mainWindow.webContents.once('did-finish-load', () => mainWindow.webContents.send('replace-ignore-and-continue'));
	});
	ipcMain.on('close-window', () => {
		if (!fs.existsSync('./storage/Settings/LauncherData.json')) {
			checkForDirAndCreate(__dirname + '/storage/Settings/LauncherData.json', '{}');
		}
		if (JSON.parse(fs.readFileSync('./storage/Settings/LauncherData.json').toString())?.trayMinQuit === true) {
			mainWindow.hide();
		}
		else {
			if (!tray.isDestroyed) {
				tray.destroy();
			}
			tray = null;
			mainWindow.close();
		}
	});
	ipcMain.on('max-window', () => {
		mainWindow.isMaximized() ? mainWindow.unmaximize() : mainWindow.maximize();
	});
	ipcMain.on('min-window', () => {
		mainWindow.minimize();
	});
	ipcMain.on('min-tray', () => {
		if (!fs.existsSync('./storage/Settings/LauncherData.json')) {
			checkForDirAndCreate(__dirname + '/storage/Settings/LauncherData.json', '{}');
		}
		if (JSON.parse(fs.readFileSync('./storage/Settings/LauncherData.json').toString())?.trayMinLaunch === true) {
			mainWindow.hide();
		}
	});
	ipcMain.on('show-window', () => {
		if (!fs.existsSync('./storage/Settings/LauncherData.json')) {
			checkForDirAndCreate(__dirname + '/storage/Settings/LauncherData.json', '{}');
		}
		if (JSON.parse(fs.readFileSync('./storage/Settings/LauncherData.json').toString())?.trayMinLaunch === true) {
			mainWindow.show();
		}
	});
	ipcMain.on('update-profile', (e, data) => {
		editLocalStorage(data);
	});
	ipcMain.on('load-banners-request', async (e, r, id) => {
		const { fetch_banner } = require('./src/modules/banners.js');
		const res = fetch_banner(r);
		res.forEach(async (url, i) => {
			mainWindow.webContents.executeJavaScript(`
			var gameElement = document.getElementById('game-div-${r[i].DisplayName}');
			gameElement.firstElementChild.setAttribute('src', '${await url}');
		   `);
		});
		mainWindow.webContents.send('load-banners-response', id, res.filter(async x => (await x) === '../icon.ico').length === res.length ? true : false);
	});
	ipcMain.on('rpcUpdate', (e, d) => updateRPC(d));
	ipcMain.on('setLaunchOnStartup', (e, bool) => app.setLoginItemSettings({ 'openAtLogin': bool, 'enabled': bool }));
	/* ipcMain.on('restart', async () => {
		app.relaunch();
		app.exit();
	});*/
});

ipcMain.on('updateSetting', (e, key, bool) => {
	const requireRestart = ['disableHardwareAcceleration'];
	checkForDirAndCreate(__dirname + '/storage/Settings/LauncherData.json', '{}');
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

const rpc = require('discord-rpc');
const rpcClient = new rpc.Client({ transport: 'ipc' });
rpcClient.login({ clientId: '932504287337148417' });

const updateRPC = (data) => {
	checkForDirAndCreate(__dirname + '/storage/Settings/LauncherData.json', JSON.stringify(CONSTANTS.defaultLauncherData));
	const LauncherData = JSON.parse(fs.readFileSync('./storage/Settings/LauncherData.json').toString());
	if (LauncherData.enableRPC !== true && typeof data === 'object' && data !== null) return 'DISABLED';
	if (!data) {
		rpcClient.clearActivity();
		return;
	}
	rpcClient.setActivity(data).catch(err => console.warn('[RPC]', err.stack.includes('connection closed') ? 'OFFLINE' : err));
}

const CONSTANTS = require('./util/Constants.json');
const { checkForDirAndCreate, handleStorageAndTransportData, editLocalStorage } = require('./src/utils.js');
const fs = require('fs');
const { eventHandler } = require('./util/functions');

const handleHardwareAcceleration = async () => {
	// fs.readFileSync('./storage/Settings/LauncherData.json').toString();
	checkForDirAndCreate(__dirname + '/storage/Settings/LauncherData.json', JSON.stringify(CONSTANTS.defaultLauncherData));
	if (JSON.parse(fs.readFileSync('./storage/Settings/LauncherData.json').toString())?.disableHardwareAcceleration === true) {
		app.disableHardwareAcceleration();
	}
}

handleHardwareAcceleration();