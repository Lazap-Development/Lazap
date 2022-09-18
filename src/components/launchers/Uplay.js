const shell = window.__TAURI__.shell;
const fs = window.__TAURI__.fs;
const os = window.__TAURI__.os;

async function getUplayLocation(checkForUplay = true) {
	let launcher_location;
	let registry_res;
	if (await os.platform() === 'win32') {
		const { stdout, stderr } = await new shell.Command(
			'cmd', ['/C', 'reg', 'query', `HKEY_LOCAL_MACHINE\\SOFTWARE\\${await os.arch() === 'x86_64' ? 'Wow6432Node\\' : ''}Ubisoft\\Launcher`, '/s'],
		).execute().catch(() => {
			launcher_location = null;
			return { stderr: 'NOT_FOUND' };
		});

		if (stderr) return;
		else {
			registry_res = stdout;
			launcher_location = registry_res.split(window.__TAURI__.os.EOL.repeat(2))[0].split('REG_SZ')[1].split('\r\n\r\n')[0].split('\r\n')[0].trim();
		}
	}
	else {
		return;
	}
	if (checkForUplay && !(await isLauncherInstalled(launcher_location))) return false;
	return { registry_res, launcher_location };
}

async function getInstalledGames() {
	const { launcher_location: path, registry_res } = await getUplayLocation() ?? {};
	if (!path) return [];
	const dataPath = `${path}\\data`;
	const games = (await fs.readDir(dataPath)).filter(x => !isNaN(x.name));
	return (await Promise.all(games.map(x => parseGameObject(x.name, registry_res)))).filter(x => typeof x === 'object' && x !== null);
}

async function parseGameObject(GameID, registry_res) {
	registry_res = registry_res.split(window.__TAURI__.os.EOL.repeat(2)).find(x => x.split(window.__TAURI__.os.EOL)[0].endsWith(GameID));
	const Location = registry_res.split('\r\n').map(x => x.trim()).filter(x => x.startsWith('InstallDir'))[0]?.split('REG_SZ')[1].trim();
	if (!(await fs.readDir(Location).catch(() => null))) return;
	const Executable = null;
	let title = Location.split('/').slice(-2)[0].replaceAll('_', ' ');
	if (title.match(/\d$/ig) && !title.replaceAll('\\d', '').endsWith(' ')) {
		const numlength = title.split('').reverse().join('').match(/\d/ig)[0].length;
		title = title.slice(0, title.length - numlength) + ' ' + title.slice(title.length - numlength);
	}
	return {
		Executable,
		Location,
		DisplayName: title,
		GameID,
		Size: null,
		LauncherName: 'Uplay',
	};
}

async function isLauncherInstalled(path) {
	return await fs.readDir(path).catch(() => null);
}

module.exports = {
	getUplayLocation,
	isLauncherInstalled,
	parseGameObject,
	getInstalledGames,
};