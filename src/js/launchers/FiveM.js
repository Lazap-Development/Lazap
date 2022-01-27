const os = require('os');
const fs = require('fs');
let { exec } = require('child_process');
exec = require('util').promisify(exec);

let user = os.userInfo().username

async function getInstalledGames(os = process.platform) {
	if (os === 'win32') {
		const isInstalled = fs.existsSync(`C:\\Users\\${user}\\AppData\\Local\\FiveM\\FiveM.exe`);
		if (!isInstalled) return [];
		const Location = `C:\\Users\\${user}\\AppData\\Local\\FiveM`;
		const Executable = 'FiveM.exe';
		return [{
				DisplayName: 'FiveM',
				LauncherName: 'FiveM',
				GameID: 'FiveM',
				Size: fs.statSync(Location),
				Location,
				Executable,
				Args: [],
			}];
		}};

module.exports = {
	getInstalledGames,
};