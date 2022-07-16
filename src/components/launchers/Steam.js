const os = window.__TAURI__.os;
const fs = window.__TAURI__.fs;
const path = window.__TAURI__.path;
const shell = window.__TAURI__.shell

async function getSteamLocation() {
    let launcher_location;
    let registry_res;
    if (await os.platform() === 'win32') {
        const output = await new shell.Command('cmd', ["/C", "Reg", "Query", `HKEY_LOCAL_MACHINE\\SOFTWARE\\${await os.arch() === 'x86_64' ? 'WOW6432Node\\' : ''}Valve\\Steam`, "/v", "InstallPath"]).execute();

        if (!output.stdout) {
            return;
        }
        else {
            registry_res = output.stdout;
            const steamDir = registry_res.split('REG_SZ')[1].split('\r\n\r\n')[0].trim();

            const text = await fs.readTextFile(steamDir + `\\steamapps\\libraryfolders.vdf`)

            const VDF = require('../modules/parseVDF');
            const parsed = VDF.parse(text);
            const toArray = Object.entries(parsed.libraryfolders).filter(x => x[1].path);

            launcher_location = toArray.map((item) => {
                return item[1].path;
            });
        }
    }
    else if (await os.platform() === 'linux') {
        const homedir = await path.homeDir();

        const text = await fs.readTextFile(homedir + `.steam/steam/steamapps/libraryfolders.vdf`)

        const VDF = require('../modules/parseVDF');
        const parsed = VDF.parse(text);
        const toArray = Object.entries(parsed.libraryfolders);
        launcher_location = toArray.map((item) => {
            return item[1].path;
        });
    }
    if (!isLauncherInstalled(launcher_location)) return false;
    return launcher_location;
}

async function isLauncherInstalled(path) {
    if (typeof path === 'string') {
        return await fs.readDir(path);
    }
    else if (Array.isArray(path)) {
        return path.map(x => fs.readDir(x)).includes(true);
    }
}

async function getInstalledGames() {
    const path = await getSteamLocation();
    if (!path) return [];


    let allGames = [];

    for (const location of path) {
        let acf_basePath;
        if (await os.platform() === 'win32') {
            acf_basePath = `${location}\\steamapps`;
        } else if (await os.platform() === 'linux') {
            acf_basePath = `${location}/steamapps`;
        }

        if (!await fs.readDir(acf_basePath)) return [];

        const readDirOfBasePath = await fs.readDir(acf_basePath)
        const acf_files = await Promise.all(readDirOfBasePath.filter(x => x.name.split(".")[1] === 'acf').map(async x => {
            return parseGameObject(acf_to_json(await fs.readTextFile(`${acf_basePath}/${JSON.stringify(x.name).replace(/['"]+/g, '')}`)));
        }))

        allGames.push(acf_files);

        const result = allGames.flat().reduce((unique, o) => {
            if (!unique.some(obj => obj.DisplayName === o.DisplayName)) {
                unique.push(o);
            }
            return unique;
        }, []);
        allGames = result;

    }
    return allGames;
}


async function parseGameObject(acf_object = {}) {
    const {
        LauncherExe: Executable,
        LauncherPath: Location,
        name: DisplayName,
        appid: GameID,
        BytesDownloaded: Size,
    } = acf_object;

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
    getSteamLocation,
    getInstalledGames,
    parseGameObject,
    acf_to_json,
    isLauncherInstalled,
};