/* eslint-disable no-unused-vars */
const { autoUpdater } = require('electron-updater');
const { ipcMain } = require('electron');
const logger = require('electron-log');
const fs = require('fs');
let mainWindow;

// Logging
autoUpdater.logger = logger;

// Configuration
autoUpdater.autoInstallOnAppQuit = true;
autoUpdater.autoDownload = false;
autoUpdater.checkForUpdates();
setInterval(() => {
	// Check for updates regardless of the setting but do not notify or update if disallowed
	if (getAutoUpdateSetting()) {
		autoUpdater.checkForUpdatesAndNotify({ 'body': 'Test', 'title': 'New Update!' }).catch(() => '');
	}
	else {
		autoUpdater.checkForUpdates().catch(() => ''); // Handle errors thrown by these functions because .catch() doesn't seem to work
	}
}, 60 * 1000);

autoUpdater.on('error', (...args) => {
	console.log(args[0]);
});

autoUpdater.on('update-available', () => {
	autoUpdater.downloadUpdate();
});

autoUpdater.on('update-downloaded', (info) => {
	const isPreRelease = autoUpdater.currentVersion.version.split('.')[2].split('-')[1] ? true : false;
	const minorVersion = autoUpdater.currentVersion.version.split('.')[1];
	const majorVersion = autoUpdater.currentVersion.version.split('.')[0];
	const patchVersion = autoUpdater.currentVersion.version.split('.')[2].split('-')[0];

	// Auto-install patches regardless of setting
	if (parseInt(info.version.split('.')[2].split('-')[0]) > parseInt(patchVersion)) {
		return mainWindow.webContents.send('handle-update-available', info);
	}

	if (getAutoUpdateSetting()) {
		mainWindow.webContents.send('handle-update-available', info);
	}
});

ipcMain.on('handle-update-install', () => {
	autoUpdater.quitAndInstall();
});

function getAutoUpdateSetting() {
	checkForDirAndCreate(__dirname + '/storage/Settings/LauncherData.json', JSON.stringify(require('./Constants.json').defaultLauncherData));
	const data = JSON.parse(fs.readFileSync('./storage/Settings/LauncherData.json').toString());
	return data.checkForUpdates;
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

module.exports = (win) => mainWindow = win;