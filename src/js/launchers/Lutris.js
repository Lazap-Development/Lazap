let {
	exec,
} = require('child_process');
const util = require('util');

exec = util.promisify(exec);

async function getLutrisLocation(os = process.platform, checkForSteam = true) {
	let launcher_location;
	if (os === 'linux') {
		const homedir = require('os').userInfo().homedir;
		launcher_location = `${homedir}/.local/share/lutris`;
	}
	if (checkForSteam && !isLauncherInstalled(launcher_location)) return false;
	return launcher_location;
}

const fs = require('fs');
const { Database } = require('sqlite3');

function isLauncherInstalled(path) {
	if (typeof path === 'string') {
		return fs.existsSync(path);
	}
	else if (Array.isArray(path)) {
		return path.map(x => fs.existsSync(x)).includes(true);
	}
}

async function getInstalledGames() {
	const path = await getLutrisLocation();
	if (!path) return [];

	else if (process.platform === 'linux') {
		let allGames = [];
		let allDBGames = [];

		const path = fs.readdirSync(`${require('os').userInfo().homedir}/.config/lutris/games`);
		const LutrisDB = new Database(`${require('os').userInfo().homedir}/.local/share/lutris/pga.db`);
		// let obj = {};
		await LutrisDB.all(`SELECT * FROM games`, (err, info) => {
			console.log(info);
			/*for (let i = 0; i >= info.length; i++) {
				const game = info[i];
				const obj = {
					DisplayName: game.name,
					GameID: game.slug,
					LauncherName: 'Lutris',
					Executable: game.installer_slug,
					Location: game.directory,
					Size: game.installed_at,
				}
				allDBGames.push(obj);
			}*/
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
		});
		// path.forEach(x => obj = yaml.load(fs.readFileSync(`${require('os').userInfo().homedir}/.config/lutris/games/${x}`, { encoding: 'utf-8' })));
		// console.log(obj);
		return allDBGames;
	}
}

/* Game Object Example
{
  "executable": "game.exe",
  "location": "C://...",
  "name": "",
  "size": 69420 // in bytes
}
*/

function parseGameObject(acf_object = {}) {
	const {
		LauncherExe: Executable,
		LauncherPath: Location,
		name: DisplayName,
		appid: GameID,
		BytesDownloaded: Size,
	} = acf_object;

	// Executable = Executable.split('/')[Executable.split('/').length - 1];
	// Location = Location.split('/').slice(0, -1).join('/');

	return {
		Executable,
		Location,
		DisplayName,
		GameID,
		Size: parseInt(Size),
		LauncherName: 'Steam',
	};
}

function acf_to_json(acf_content = '') {
	if (acf_content.length === 0) return;
	return JSON.parse(
		acf_content.split('\n').slice(1).map((x, i, arr) => {
			if (x.length === 0) return;
			if (x.trim().includes('\t\t')) {
				return (
					x.trim().replace('\t\t', ':') + (['{', '}'].includes(arr[i + 1]?.trim().slice(0, 1)) ? '' : ',')
				);
			}
			return (
				x.split('"').length > 1 ? x.trim() + ':' : x + (x.trim() === '{' || !arr[i + 1] || ['{', '}'].includes(arr[i + 1]?.trim().slice(0, 1)) ? '' : ',')
			);
		}).join('\n'),
	);
}

module.exports = {
	getLutrisLocation,
	getInstalledGames,
	parseGameObject,
	acf_to_json,
	isLauncherInstalled,
};