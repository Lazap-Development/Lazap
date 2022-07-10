const fs = require('fs');

const user = require('os').userInfo().username;

function getInstalledGames(os = process.platform) {
	if (os === 'win32') {
		const isInstalled = fs.existsSync(`C:\\Users\\${user}\\AppData\\local\\osu!\\osu!.exe`);
		if (!isInstalled) return [];
		const Location = `C:\\Users\\${user}\\AppData\\local\\osu!`;
		const Executable = 'osu!.exe';
		return [{
			DisplayName: 'osu!',
			LauncherName: 'osu!',
			GameID: 'osu!',
			Size: fs.statSync(Location).size,
			Location,
			Executable,
			Args: [],
		}];
	}
	return [];
}

module.exports = {
	getInstalledGames,
};
