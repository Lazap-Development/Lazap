module.exports = {
	getInstalledGames,
	parseGameObject,
};

const fs = require('fs');
const { promisify } = require('util');
let { exec } = require('child_process');
exec = promisify(exec);

async function getInstalledGames(os = process.platform) {
	let launcher_location;
	if (os === 'win32') {
		const { stdout, error } = await exec(
			'Reg query HKEY_CLASSES_ROOT\\riotclient\\DefaultIcon',
		).catch(() => {
			launcher_location = null;
			return { error: 'NOT_FOUND' };
		});

		if (!stdout) return [];

		if (error) {
			console.error(
				'There was an error while trying to find Riot Games: \n' +
                    require('util').inspect(error, { depth: 1 }),
			);
			return [];
		}
		launcher_location = stdout.split('"')[1];
	}
	if (!fs.existsSync(launcher_location)) return [];
	return [await parseGameObject(launcher_location)].filter(x => typeof x === 'object' && x !== null);
}

async function parseGameObject(path) {
	const Executable = 'RiotClientServices.exe';
	const Location = path.slice(0, -22);
	const Args = ['--launch-product=valorant', '--launch-patchline=live'];
	const DisplayName = 'Valorant';
	if (!fs.existsSync(Location.slice(0, -12) + 'VALORANT')) return;
	const Size = fs.statSync(Location.slice(0, -12) + 'VALORANT').size;

	return {
		Executable,
		Location,
		Args,
		DisplayName,
		Size,
		LauncherName: 'RiotGames',
		GameID: 'Valorant',
	};
}
