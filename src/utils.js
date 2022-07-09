const fs = require('fs');

const handleStorageAndTransportData = (mainWindow, userDataPath) => {
	const Constants = require('../util/Constants.json');
	checkForDirAndCreate(userDataPath + '/storage/Settings/LauncherData.json', JSON.stringify(Constants.defaultLauncherData), userDataPath);
	checkForDirAndCreate(userDataPath + '/storage/Settings/userprofile.json', '{}', userDataPath);

	let LauncherData = JSON.parse(fs.readFileSync(userDataPath + '/storage/Settings/userprofile.json').toString());
	if (!Object.keys(LauncherData).length) {
		const os = require('os');
		LauncherData = {
			username: os.userInfo().username,
			pfp: 'default',
		};
		fs.writeFileSync(userDataPath + '/storage/Settings/userprofile.json', JSON.stringify(LauncherData));
	}
	else {
		if (LauncherData.pfp !== 'default' && !fs.existsSync(LauncherData.pfp)) {
			LauncherData.pfp = 'default';
		}
		fs.writeFileSync(userDataPath + '/storage/Settings/userprofile.json', JSON.stringify(LauncherData));
	}

	mainWindow.webContents.send('load-profile', LauncherData);
};

const editLocalStorage = (content, userDataPath) => {

	checkForDirAndCreate(userDataPath + '/storage/Settings/userprofile.json', '{}', userDataPath);
	fs.writeFile(userDataPath + '/storage/Settings/userprofile.json', JSON.stringify(content), (err) => {
		if (err) throw err;
	});
};

const checkForDirAndCreate = (dir, fileContent = '', userDataPath) => {
	if (fs.existsSync(dir.split(userDataPath)[1])) return true;
	dir.split(userDataPath)[1].split('/').slice(1).forEach((name, i, arr) => {
		dir = dir.replaceAll('\\', '/');
		if (!fs.existsSync(userDataPath + `/${arr.slice(0, i + 1).join('/')}`)) {
			if (name.split('.')[1]) {
				fs.writeFileSync(userDataPath + `/${arr.slice(0, i + 1).join('/')}`, fileContent);
				return;
			}
			else {
				fs.mkdirSync(userDataPath + `/${arr.slice(0, i + 1).join('/')}`);
			}
		}
	});
};

module.exports = {
	checkForDirAndCreate,
	editLocalStorage,
	handleStorageAndTransportData,
};
