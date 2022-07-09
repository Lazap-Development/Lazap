let { exec } = require('child_process');
exec = require('util').promisify(exec);

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
	else {
		return;
	}
	if (checkForUplay && !isLauncherInstalled(launcher_location)) return false;
	return launcher_location;
}

const fs = require('fs');
async function getInstalledGames() {
	const path = await getUplayLocation();
	if (!path) return [];
	const dataPath = `${path}\\data`;
	const games = fs.readdirSync(dataPath).filter(x => !isNaN(x));
	return (await Promise.all(games.map(x => parseGameObject(x)))).filter(x => typeof x === 'object' && x !== null);
}

async function parseGameObject(GameID) {
	const { stdout: registry_res, error } = await exec(`Reg Query "HKEY_LOCAL_MACHINE\\SOFTWARE\\${process.arch === 'x64' ? 'Wow6432Node\\' : ''}Ubisoft\\Launcher\\Installs\\${GameID}" /v InstallDir`).catch(() => {
		return { error: `${GameID} NOT_FOUND` };
	});
	if (error) {
		console.error(error);
		return;
	}
	const Location = registry_res.split('REG_SZ')[1].split('\r\n\r\n')[0].trim();
	if (!fs.existsSync(Location)) return;
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
