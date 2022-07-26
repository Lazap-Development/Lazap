const os = window.__TAURI__.os;
const fs = window.__TAURI__.fs;

async function getInstalledGames() {
	if (await os.platform() === 'win32') {
		if (!await isLauncherInstalled()) return [];
		const read = await fs.readDir('C:\\ProgramData\\Epic\\EpicGamesLauncher\\Data\\Manifests')
		const games = read.filter((x) => x.name.split('.')[1]?.toLowerCase() === 'item')
			.map(async (x) =>
				JSON.parse(await fs.readTextFile(`C:\\ProgramData\\Epic\\EpicGamesLauncher\\Data\\Manifests\\${JSON.stringify(x.name).replace(/['"]+/g, '')}`)),
			);

		return await Promise.all(games.map((x) => parseGameObject(x)));
	}
}

async function isLauncherInstalled(path = 'C:\\ProgramData\\Epic\\EpicGamesLauncher\\Data\\Manifests') {
	try {
		return await fs.readDir(path);
	} catch (err) {
		return false;
	}
}

async function parseGameObject(rawObj = {}) {
	const {
		LaunchExecutable: Executable,
		InstallLocation: Location,
		DisplayName,
		AppName: GameID,
		InstallSize: Size,
		LaunchCommand,
		CatalogNamespace,
		CatalogItemId,
		AppName
	} = await rawObj;


	return {
		Executable,
		Location,
		DisplayName,
		GameID,
		LaunchID: `${CatalogNamespace}:${CatalogItemId}:${AppName}`,
		Size,
		LaunchCommand,
		LauncherName: 'EpicGames',
	};
}

export {
	getInstalledGames,
	parseGameObject,
};