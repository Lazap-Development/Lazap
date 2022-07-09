const fs = require('fs');
const { promisify } = require('util');
let { exec } = require('child_process');
exec = promisify(exec);

function getRiotGamesLocation(launcher_location) {
        const array = launcher_location.split("\\");
        array.splice(array.indexOf("Riot Client"), 2);
        return array.join("\\");
}

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
        
        let allGames = [];
        const games = fs.readdirSync(getRiotGamesLocation((launcher_location)));
        const indexToRemove = games.indexOf("Riot Client");
        games.splice(indexToRemove, 1);

        if (games.includes("VALORANT")) {
          games[games.indexOf("VALORANT")] = "Valorant";
        }

        for (i = 0; i < games.length; i++) {
                  allGames.push(await parseGameObject(launcher_location, games[i]));
        }

        return allGames;
}

async function parseGameObject(path, game = '') {
	const Executable = 'RiotClientServices.exe';
	const Location = path.slice(0, -22);
	const Args = [`--launch-product=${game.toLowerCase().replace(" ", "_").replace(" ", "_")}`, '--launch-patchline=live'];
	const DisplayName = game;
	if (!fs.existsSync(Location.slice(0, -12) + game)) return;
	const Size = fs.statSync(Location.slice(0, -12) + game).size;

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
