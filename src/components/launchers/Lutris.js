// let { exec } = require('child_process');
// const util = require('util');
const fs = window.__TAURI__.fs;
// import Database from "tauri-plugin-sql-api";
// const Database = require('tauri-plugin-sql-api');
// exec = util.promisify(exec);

async function getLutrisLocation(os = window.__TAURI__.os.platform(), checkForLutris = true) {
	let launcher_location;
	if (os === 'linux') {
		const homedir = window.__TAURI__.os.homedir;
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
		console.log("not yet because I can't import the sqlite package yet. so cringy");
		// console.log(Database);
		/*const LutrisDB = new Database(`${window.__tauri__.os.homedir}/.local/share/lutris/pga.db`);
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
		});*/
		return allDBGames;
	}
}

module.exports = {
	getInstalledGames,
};