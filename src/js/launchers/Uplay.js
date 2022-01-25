let { exec } = require('child_process');
exec = require('util').promisify(exec);
const fs = require('fs');

async function getUplayLocation(os = process.platform, checkForUplay = true) {
	let launcher_location;
	let registry_res;
	if (os === 'win32') {
		const { stdout, error } = await exec(
			`Reg Query "HKEY_LOCAL_MACHINE\\SOFTWARE\\${process.arch === 'x64' ? 'Wow6432Node\\' : ''}Ubisoft\\Launcher" /v InstallDir`,
		).catch(() => {
			launcher_location = null;
			return { error: 'NOT_FOUND' };
		});

		if (error) {
			return;
		}
		else {
			registry_res = stdout;
			launcher_location = registry_res.split('REG_SZ')[1].split('\r\n\r\n')[0].trim();
		}
	}
	if (checkForUplay && !isLauncherInstalled(launcher_location)) return false;
	return launcher_location;
}

async function getInstalledGames() {
	const path = await getUplayLocation();
	if (!path) return [];
	const dataPath = `${path}\\data`;
	const games = fs.readdirSync(dataPath).filter(x => !isNaN(x));
	return Promise.all(games.map(x => parseGameObject(x)));
}

async function parseGameObject(GameID) {
	const { stdout: registry_res, error } = await exec(`Reg Query "HKEY_LOCAL_MACHINE\\SOFTWARE\\${process.arch === 'x64' ? 'Wow6432Node\\' : ''}Ubisoft\\Launcher\\Installs\\${GameID}" /v InstallDir`);
	if (error) return;
	const Location = registry_res.split('REG_SZ')[1].split('\r\n\r\n')[0].trim();
	const Executable = null;
	const DisplayName = Location.split('/').slice(-2)[0];
	const Size = fs.statSync(Location).size;
	return {
		Executable,
		Location,
		DisplayName,
		GameID,
		Size,
		LauncherName: 'Uplay',
	};
}

function isLauncherInstalled(path) {
	return fs.existsSync(path);
}

module.exports = {
	getUplayLocation,
	isLauncherInstalled,
	parseGameObject,
	getInstalledGames,
};
