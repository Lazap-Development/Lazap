// eslint-disable-next-line
const { loadGames } = require('@/components/launchers/find-games');
const os = window.__TAURI__.os;
const fs = window.__TAURI__.fs;
const path = window.__TAURI__.path;

let exec = "l";

async function getSteamLocation() {
    let launcher_location;
    let registry_res;
    if (await os.platform() === 'win32') {
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
    else if (await os.platform() === 'linux') {
        const homedir = await path.homeDir();

        const text = await fs.readTextFile(homedir + `.steam/steam/steamapps/libraryfolders.vdf`)

        const VDF = require('../modules/parseVDF');
        const parsed = VDF.parse(text);
        const toArray = Object.entries(parsed.libraryfolders);
        const mappedArray = toArray.map((item) => {
            return item[1].path;
        });
        launcher_location = mappedArray[0];
    }
    if (!await isLauncherInstalled(launcher_location)) return false;
    return launcher_location;
}

async function isLauncherInstalled(path) {
    if (typeof path === 'string') {
        console.log(await fs.readDir(path));
        return await fs.readDir(path);
    }
    else if (Array.isArray(path)) {
        console.log(path.map(async () => await fs.readDir("/home/logic/.local/share/Steam")).includes(true));
        return path.map(async x => await fs.readDir(x)).includes(true);
    }
}

async function getInstalledGames() {
    const path = await getSteamLocation();
    if (!path) return [];
    if (await os.platform() === 'win32') {
        const acf_basePath = `${path}\\steamapps`;
        if (!await fs.readDir(acf_basePath)) return [];
        const acf_files = fs.readdirSync(acf_basePath).filter((x) => x.split('.')[1] === 'acf')
            .map((x) => parseGameObject(acf_to_json(fs.readFileSync(`${acf_basePath}\\${x}`).toString())));

        return acf_files;
    }
    else if (await os.platform() === 'linux') {
        let allGames = [];
        /*await path.forEach(async location => {
            const acf_basePath = `${location}/steamapps`;
            if (!await fs.readDir(acf_basePath)) return [];
            const getPath = await fs.readDir(acf_basePath)

            const acf_files = getPath.filter((x) => x.split('.')[1] === 'acf')
                .map((x) => parseGameObject(acf_to_json(fs.readFileSync(`${acf_basePath}/${x}`).toString())));

            allGames.push(acf_files);
            const result = allGames.flat().reduce((unique, o) => {
                if (!unique.some(obj => obj.DisplayName === o.DisplayName)) {
                    unique.push(o);
                }
                return unique;
            }, []);
            console.log(result);
            allGames = result;
        });*/

        const acf_basePath = `${path}/steamapps`;
        if (!await fs.readDir(acf_basePath)) return [];
        const getPath = await fs.readDir(acf_basePath);
        // eslint-disable-next-line
        const acf_files = getPath.filter((x) => x.name.includes(".acf"))
            .map(x => parseGameObject(acf_to_json(fs.readTextFile(`${acf_basePath}/${x.name}`).toString())));
        //console.log(xd);

        allGames.push(acf_files);
        const result = allGames.flat().reduce((unique, o) => {
            if (!unique.some(obj => obj.DisplayName === o.DisplayName)) {
                unique.push(o);
            }
            return unique;
        }, []);

        console.log(result);

        return allGames;
    }
}


function parseGameObject(acf_object = {}) {
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

async function acf_to_json(acf_content = '') {
    if (acf_content.length === 0) return;
    console.log(acf_content.split('\n'));
    console.log(acf_content.split('\n').slice(1).map((x, i, arr) => {
        if (x.length === 0) return;
        if (x.trim().includes('\t\t')) {
            return (
                x.trim().replace('\t\t', ':') + (['{', '}'].includes(arr[i + 1]?.trim().slice(0, 1)) ? '' : ',')
            );
        }
        return (
            x.split('"').length > 1 ? x.trim() + ':' : x + (x.trim() === '{' || !arr[i + 1] || ['{', '}'].includes(arr[i + 1]?.trim().slice(0, 1)) ? '' : ',')
        );
    }));
    const o = acf_content.split('\n').slice(1).map((x, i, arr) => {
        if (x.length === 0) return;
        if (x.trim().includes('\t\t')) {
            return (
                x.trim().replace('\t\t', ':') + (['{', '}'].includes(arr[i + 1]?.trim().slice(0, 1)) ? '' : ',')
            );
        }
        return (
            x.split('"').length > 1 ? x.trim() + ':' : x + (x.trim() === '{' || !arr[i + 1] || ['{', '}'].includes(arr[i + 1]?.trim().slice(0, 1)) ? '' : ',')
        );
    }).join('\n');
    console.log(o);
    const ha = JSON.parse(o,);
    console.log(ha);
    return "ha"
}

module.exports = {
    getSteamLocation,
    getInstalledGames,
    parseGameObject,
    acf_to_json,
    isLauncherInstalled,
};