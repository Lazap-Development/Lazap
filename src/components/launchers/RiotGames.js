const os = window.__TAURI__.os;
const fs = window.__TAURI__.fs;
const { promisify } = require('util');
let { exec } = require('child_process');
exec = promisify(exec);

function getRiotGamesLocation(launcher_location) {
	return launcher_location.split('\\').slice(0, -2).join('\\');
}

async function getInstalledGames() {
	let launcher_location;
	if (await os.platform() === 'win32') {
        const output = await new shell.Command('cmd', ["/C", "Reg", "query", "HKEY_CLASSES_ROOT\\riotclient\\DefaultIcon"]).execute();
        if (!output.stdout) return [];

		launcher_location = output.stdout.split('"')[1];
	}

	if (!fs.existsSync(launcher_location)) return [];
	const games = fs.readdirSync(getRiotGamesLocation((launcher_location))).filter(x => x !== 'Riot Client');

	if (games.includes('VALORANT')) {
		games[games.indexOf('VALORANT')] = 'Valorant';
	}
	if (games.includes('LoR')) {
		games[games.indexOf('LoR')] = 'Legends of Runeterra';
	}

	return games.map(x => parseGameObject(launcher_location, x));
}

function parseGameObject(path, game = '') {
	const correctArgs = {
		'Valorant': 'valorant',
		'League of Legends': 'league_of_legends',
		'Legends of Runeterra': 'bacon',
	};
	const correctPathName = {
		'Valorant': 'VALORANT',
		'League of Legends': 'League of Legends',
		'Legends of Runeterra': 'LoR',
	};
	const Executable = 'RiotClientServices.exe';
	const Location = path.slice(0, -22);
	const Args = [`--launch-product=${correctArgs[game]}`, '--launch-patchline=live'];
	const DisplayName = game;
	if (!fs.existsSync(Location.slice(0, -12) + correctPathName[game])) return;
	const Size = fs.statSync(Location.slice(0, -12) + correctPathName[game]).size;

	return {
		Executable,
		Location,
		Args,
		DisplayName,
		Size,
		LauncherName: 'RiotGames',
		GameID: game,
	};
}

module.exports = {
	getInstalledGames,
	parseGameObject,
};