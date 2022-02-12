const fs = require('fs');
const os = require('os');
const Constants = require('../Constants.json');
const path = require('path');
const APP_BASE_PATH = path.join(__dirname, path.relative(__dirname, './'));

function handleStorageAndTransportData(mainWindow) {
	checkForDirAndCreate(APP_BASE_PATH + '/storage/Settings/LauncherData.json', JSON.stringify(Constants.defaultLauncherData));
	checkForDirAndCreate(APP_BASE_PATH + '/storage/Settings/userprofile.json', '{}');

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
	checkForDirAndCreate(APP_BASE_PATH + '/storage/Settings/userprofile.json', '{}');
	fs.writeFile('./storage/Settings/userprofile.json', JSON.stringify(content), (err) => {
		if (err) throw err;
	});
}

function checkForDirAndCreate(dir, fileContent = '') {
	if (fs.existsSync(dir.split(APP_BASE_PATH)[1])) return true;
	dir.split(APP_BASE_PATH)[1].split('/').slice(1).forEach((name, i, arr) => {
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

module.exports = {
	checkForDirAndCreate,
	editLocalStorage,
	handleStorageAndTransportData,
};