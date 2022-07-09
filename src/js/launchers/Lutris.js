let {
	exec,
} = require('child_process');
const util = require('util');

exec = util.promisify(exec);

async function getLutrisLocation(os = process.platform, checkForSteam = true) {
	let launcher_location;
	let registry_res;
	if (os === 'win32') {
		const { stdout, error } = await exec(
			`Reg Query "HKEY_LOCAL_MACHINE\\SOFTWARE\\${process.arch === 'x64' ? 'Wow6432Node\\' : ''}Valve\\Steam" /v InstallPath`,
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
	else if (os === 'linux') {
		const homedir = require('os').userInfo().homedir;
		launcher_location = `${homedir}/.local/share/lutris`;
	}
	if (checkForSteam && !isLauncherInstalled(launcher_location)) return false;
	return launcher_location;
}

const fs = require('fs');
const VDF = require('../../modules/parseVDF');
const yaml = require('js-yaml');
const { loadGames } = require('./find-games');
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

	if (process.platform === 'win32') {
		const acf_basePath = `${path}\\steamapps`;
		if (!fs.existsSync(acf_basePath)) return [];
		const acf_files = fs.readdirSync(acf_basePath).filter((x) => x.split('.')[1] === 'acf')
			.map((x) => parseGameObject(acf_to_json(fs.readFileSync(`${acf_basePath}\\${x}`).toString())));

		return acf_files;
	}
	else if (process.platform === 'linux') {
		let allGames = [];
		let allDBGames = [];

		const path = fs.readdirSync(`${require('os').userInfo().homedir}/.config/lutris/games`);
		const LutrisDB = new Database(`${require('os').userInfo().homedir}/.local/share/lutris/pga.db`);
		// let obj = {};
		await LutrisDB.all(`SELECT * FROM games`, (err, info) => {
			info.forEach(x => {
				const obj = {
					DisplayName: x.name,
					GameID: 'yourmom',
					LauncherName: 'Lutris',
					Executable: 'xd',
					Location: 'xd',
					Size: 9999,
				}
				allDBGames.push(obj);
			})
		});
		// path.forEach(x => obj = yaml.load(fs.readFileSync(`${require('os').userInfo().homedir}/.config/lutris/games/${x}`, { encoding: 'utf-8' })));
		console.log(allDBGames);
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