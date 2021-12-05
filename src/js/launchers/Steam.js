module.exports = {
    getSteamLocation,
    getInstalledGames,
    parseGameObject,
    acf_to_json,
};

let { exec } = require('child_process');
const util = require('util');
exec = util.promisify(exec);
const fs = require('fs');

async function getSteamLocation(os = process.platform) {
    let launcher_location;
    if (os === 'win32') {
        let registry_res;
        if (process.arch === 'x64') {
            let { stdout, err } = await exec(`Reg Query "HKEY_LOCAL_MACHINE\\SOFTWARE\\${process.arch === 'x64' ? 'Wow6432Node\\' : ''}Valve\\Steam" /v InstallPath`);
            if (err) {
                console.error(`Error while loading steam games: \n${err}`)
                alert(`An error occured while loading steam games.`);
            }
            registry_res = stdout; // \r\nHKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\Valve\\Steam\r\n    InstallPath    REG_SZ    C:\\Program Files (x86)\\Steam\r\n\r\n
            launcher_location = registry_res.split('REG_SZ')[1].split('\r\n\r\n')[0].trim();
        }
    }
    return launcher_location;
}

async function getInstalledGames() {
    const acf_basePath = `${await getSteamLocation()}\\steamapps`;
    const acf_files = fs.readdirSync(acf_basePath).filter(x => x.split('.')[1].toLowerCase() === 'acf').map(x => parseGameObject(acf_to_json(fs.readFileSync(`${acf_basePath}\\${x}`).toString())));

    return acf_files;
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
    let {
        LauncherPath: Executable,
        LauncherPath: Location,
        name: DisplayName,
        appid: GameID,
        BytesDownloaded: Size,
    } = acf_object;

    Executable = Executable.split('\\').reverse()[0];
    Location = Location.split('\\').slice(0, -1);
    Size = parseInt(Size);

    return {
        Executable,
        Location,
        DisplayName,
        GameID,
        Size,
        LauncherName: 'Steam',
    };
}

function acf_to_json(acf_content = '') {
    if (acf_content.length === 0) return;
	return (JSON.parse(acf_content.split('\n').slice(1).map((x, i, arr) => {
		if (x.length === 0) return;
		if (x.trim().includes('\t\t')) return x.trim().replace('\t\t', ':') + (['{', '}'].includes(arr[i + 1]?.trim().slice(0, 1)) ? '' : ',');
		return x.split('"').length > 1 ? x.trim() + ':' : x + (x.trim() === '{' || !arr[i + 1] || ['{', '}'].includes(arr[i + 1]?.trim().slice(0, 1)) ? '' : ',');
	}).join('\n')));
}