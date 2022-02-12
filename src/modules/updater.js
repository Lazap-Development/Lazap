/* eslint-disable no-unused-vars */
const { autoUpdater } = require('electron-updater');
const { ipcMain } = require('electron');
const logger = require('electron-log');
const fs = require('fs');
const path = require('path');
const APP_BASE_PATH = path.join(__dirname, path.relative(__dirname, './'));
let mainWindow;
const { checkForDirAndCreate } = require('../utils.js');
// Logging
autoUpdater.logger = logger;

// Configuration
autoUpdater.autoInstallOnAppQuit = false;
autoUpdater.autoDownload = false;
autoUpdater.checkForUpdatesAndNotify();
setInterval(() => {
	// Check for updates regardless of the setting but do not notify or update if disallowed
	if (getAutoUpdateSetting()) {
		autoUpdater.checkForUpdatesAndNotify({ 'body': 'Test', 'title': 'New Update!' }).catch(() => '');
	}
	else {
		autoUpdater.checkForUpdates().catch(() => ''); // Handle errors thrown by these functions because .catch() doesn't seem to work
	}
}, 60 * 60 * 1000);

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
	autoUpdater.quitAndInstall(false, true);
});

function getAutoUpdateSetting() {
	checkForDirAndCreate(APP_BASE_PATH + '/storage/Settings/LauncherData.json', JSON.stringify(require('./Constants.json').defaultLauncherData));
	const data = JSON.parse(fs.readFileSync('./storage/Settings/LauncherData.json').toString());
	return data.checkForUpdates;
}

module.exports = (win) => mainWindow = win;