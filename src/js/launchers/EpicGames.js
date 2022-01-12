module.exports = {
	getInstalledGames,
	parseGameObject,
};
const fs = require('fs');
function getInstalledGames(os = process.platform) {
	if (os === 'win32') {
		if (!isLauncherInstalled()) return [];
		const games = fs
			.readdirSync(
				'C:\\ProgramData\\Epic\\EpicGamesLauncher\\Data\\Manifests',
			)
			.filter((x) => x.split('.')[1]?.toLowerCase() === 'item')
			.map((x) =>
				JSON.parse(
					fs.readFileSync(
						`C:\\ProgramData\\Epic\\EpicGamesLauncher\\Data\\Manifests\\${x}`,
					),
				),
			);

		return games.map((x) => parseGameObject(x));
	}
}

function isLauncherInstalled(
	path = 'C:\\ProgramData\\Epic\\EpicGamesLauncher\\Data\\Manifests',
) {
	return fs.existsSync(path);
}
/* Game Object Example
{
  "executable": "game.exe",
  "location": "C://...",
  "name": "",
  "size": 69420 // in bytes
}
*/
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
		Size,
		LaunchCommand,
		LauncherName: 'EpicGames',
	};
}
// LaunchExecutable, InstallSize, LaunchCommand, InstallLocation
