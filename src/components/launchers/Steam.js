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

        const text = await fs.readTextFile(homedir + `.steam/steam/steamapps/libraryfolders.vdf`);

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
        return fs.readdirSync(acf_basePath).filter((x) => x.split('.')[1] === 'acf')
            .map((x) => parseGameObject(acf_to_json(fs.readFileSync(`${acf_basePath}\\${x}`).toString())));
    }
    else if (await os.platform() === 'linux') {
        // eslint-disable-next-line
        const readTextFile = (file) => {
            const rawFile = new XMLHttpRequest();
            rawFile.open("GET", file, false);
            rawFile.onreadystatechange = function () {
                if(rawFile.readyState === 4) {
                    if(rawFile.status === 200 || rawFile.status === 0) {
                        const allText = rawFile.responseText;
                        alert(allText);
                    }
                }
            }
            rawFile.send(null);
        }

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
        console.log(acf_basePath);
        await fetch(`${acf_basePath}/appmanifest_706990.acf`).then(e => e.text()).then(text => console.log(text))
        // eslint-disable-next-line
        const acf_files = getPath.filter((x) => x.name.includes(".acf"))
            .map(x => parseGameObject(acf_to_json(fs.readTextFile(`${acf_basePath}/${x.name}`).toString())));

        allGames.push(acf_files);
        console.log(getPath.filter((x) => x.name.includes(".acf"))
            .map(x => parseGameObject(acf_to_json(fetch(`${acf_basePath}/${x.name}`).then(response => response.text())))));
        console.log(getPath.filter((x) => x.name.includes(".acf"))
            .map(x => parseGameObject(acf_to_json(fs.readTextFile(`${acf_basePath}/${x.name}`).toString()))));
        // eslint-disable-next-line
        const result = allGames.flat().reduce((unique, o) => {
            if (!unique.some(obj => obj.DisplayName === o.DisplayName)) {
                unique.push(o);
            }
            return unique;
        }, []);

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
        DisplayName,
        GameID,
        Executable,
        Location,
        Size: parseInt(Size),
        LauncherName: 'Steam',
    };
}

async function acf_to_json(acf_content = '') {
    if (acf_content.length === 0) return;
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
    // eslint-disable-next-line
    const ha = JSON.parse(o,);
    return {
        DisplayName: "gay"
    }
}

module.exports = {
    getSteamLocation,
    getInstalledGames,
    parseGameObject,
    acf_to_json,
    isLauncherInstalled,
};