let { exec } = require('child_process');
const util = require('util');
const fs = require('fs');
const { Database } = require('sqlite3');
exec = util.promisify(exec);

async function getLutrisLocation(os = process.platform, checkForLutris = true) {
	let launcher_location;
	if (os === 'linux') {
		const homedir = require('os').userInfo().homedir;
		launcher_location = `${homedir}/.local/share/lutris`;
	}
	if (checkForLutris && !isLauncherInstalled(launcher_location)) return false;
	return launcher_location;
}

function isLauncherInstalled(path) {
	if (typeof path === 'string') {
		return fs.existsSync(path);
	} else if (Array.isArray(path)) {
		return path.map(x => fs.existsSync(x)).includes(true);
	}
}

async function getInstalledGames() {
	const path = await getLutrisLocation();
	if (!path) return [];

	else if (process.platform === 'linux') {
		let allDBGames = [];
		const LutrisDB = new Database(`${require('os').userInfo().homedir}/.local/share/lutris/pga.db`);
		await LutrisDB.all(`SELECT * FROM games`, (err, info) => {
			info.forEach(x => {
				const obj = {
					DisplayName: x.name,
					GameID: x.slug,
					LauncherName: 'Lutris',
					LaunchID: x.id,
					Executable: x.installer_slug,
					Location: x.directory,
					Size: x.installed_at,
				};
				allDBGames.push(obj);
			})
		});
		return allDBGames;
	}
}

module.exports = {
	getInstalledGames,
};