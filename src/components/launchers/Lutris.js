const fs = window.__TAURI__.fs;
import Database from "tauri-plugin-sql-api";
const homedir = window.__TAURI__.path.homeDir();

async function getLutrisLocation(os = window.__TAURI__.os.platform()) {
	let launcher_location;
	if (await os === 'linux') launcher_location = `${await homedir}.local/share/lutris`;
	try {
			await fs.readDir(launcher_location);
	} catch (err) {
			return false;
	}
	return launcher_location;
}

async function getInstalledGames(os = window.__TAURI__.os.platform()) {
	const path = await getLutrisLocation();
	if (!path) return [];

	else if (await os === 'linux') {
		let allDBGames = [];
		const dataDirPath = await window.__TAURI__.path.dataDir();
		const lutris = await Database.load(`sqlite:${dataDirPath}/lutris/pga.db`);
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
