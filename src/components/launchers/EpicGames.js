const os = window.__TAURI__.os;
const fs = window.__TAURI__.fs;

async function getInstalledGames() {
	if (await os.platform() === 'win32') {
		if (!isLauncherInstalled()) return [];
		const games = await fs.readDir('C:\\ProgramData\\Epic\\EpicGamesLauncher\\Data\\Manifests')
			.filter((x) => x.split('.')[1]?.toLowerCase() === 'item')
			.map((x) =>
				JSON.parse(fs.readFileSync(`C:\\ProgramData\\Epic\\EpicGamesLauncher\\Data\\Manifests\\${x}`)),
			);

		return games.map((x) => parseGameObject(x));
	}
	return [];
}

async function isLauncherInstalled(path = 'C:\\ProgramData\\Epic\\EpicGamesLauncher\\Data\\Manifests') {
	try {
        return await fs.readDir(path);
    } catch (err) {
        return false;
    }
}

function parseGameObject(rawObj = {}) {
	const {
		LaunchExecutable: Executable,
		InstallLocation: Location,
		DisplayName,
		AppName: GameID,
		InstallSize: Size,
		LaunchCommand,
	} = rawObj;

	return {
		Executable,
		Location,
		DisplayName,
		GameID,
		LaunchID: `${rawObj.CatalogNamespace}:${rawObj.CatalogItemId}:${rawObj.AppName}`,
		Size,
		LaunchCommand,
		LauncherName: 'EpicGames',
	};
}

module.exports = {
	getInstalledGames,
	parseGameObject,
};