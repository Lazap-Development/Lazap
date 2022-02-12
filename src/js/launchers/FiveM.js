const fs = require('fs');

const user = require('os').userInfo().username;

function getInstalledGames(os = process.platform) {
	if (os === 'win32') {
		const isInstalled = fs.existsSync(`C:\\Users\\${user}\\AppData\\Local\\FiveM\\FiveM.exe`);
		if (!isInstalled) return [];
		const Location = `C:\\Users\\${user}\\AppData\\Local\\FiveM`;
		const Executable = 'FiveM.exe';
		return [{
			DisplayName: 'FiveM',
			LauncherName: 'FiveM',
			GameID: 'FiveM',
			Size: fs.statSync(Location).size,
			Location,
			Executable,
			Args: [],
		}];
	}
}

module.exports = {
	getInstalledGames,
};