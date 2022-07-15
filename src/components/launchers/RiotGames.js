const fs = window.__TAURI__.fs;
const shell = window.__TAURI__.shell;
const os = window.__TAURI__.os;

async function getInstalledGames() {
    let launcher_location;
    if (await os.platform() !== 'win32') return [];

    const output = await new shell.Command('cmd', ["/C", "Reg", "Query", `HKEY_CLASSES_ROOT\\riotclient\\DefaultIcon`]).execute();

    if (!output.stdout) {
        console.error(
            'There was an error while trying to find Riot Games: \n' + output.error
        );
        return [];
    }

    launcher_location = output.stdout.split('"')[1];

    if (!await fs.readDir(launcher_location)) return [];
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

module.exports = {
    getInstalledGames,
    parseGameObject,
};