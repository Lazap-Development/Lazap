// let { exec } = require('child_process');
// const util = require('util');
const fs = window.__TAURI__.fs;
import Database from "tauri-plugin-sql-api";
const homedir = window.__TAURI__.path.homeDir();
// const Database = require('tauri-plugin-sql-api');
// exec = util.promisify(exec);

async function getLutrisLocation(os = window.__TAURI__.os.platform(), checkForLutris = true) {
	let launcher_location;
	if (await os === 'linux') launcher_location = `${await homedir}/.local/share/lutris`;
	if (checkForLutris && !isLauncherInstalled(launcher_location)) return false;
	return launcher_location;
}

function isLauncherInstalled(path) {
	if (typeof path === 'string') {
		return fs.readDir(path);
	} else if (Array.isArray(path)) {
		return path.map(x => fs.readDir(x)).includes(true);
	}
}

async function getInstalledGames(os = window.__TAURI__.os.platform()) {
	const path = await getLutrisLocation();
	if (!path) return [];

	else if (await os === 'linux') {
		let allDBGames = [];
		const lutris = await Database.load(`sqlite:/home/logic/.local/share/lutris/pga.db`);
		const a = await lutris.select('SELECT * FROM games');
		a.map(x => {
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
		/*await LutrisDB.all(`SELECT * FROM games`, (err, info) => {
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

export {
	getInstalledGames,
};