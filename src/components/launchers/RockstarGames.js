const shell = window.__TAURI__.shell;
const fs = window.__TAURI__.fs;
const os = window.__TAURI__.os;
const RockstarGamesData = require('../others/rockstargames.json');
async function getInstalledGames() {
	if (await os.platform() === 'linux') {
		return [];
	}
	else if (await os.platform() === 'win32') {
		let allGames = (await new shell.Command('cmd', ['/C', 'Reg', 'query', 'HKEY_LOCAL_MACHINE\\SOFTWARE\\WOW6432Node\\Rockstar Games', '/s']).execute().catch(() => ''))?.stdout || '';
        if(allGames === "") return [];
        allGames = allGames.split(window.__TAURI__.os.EOL.repeat(2)).map(x => {
            let res = x.split(window.__TAURI__.os.EOL).filter(x => x.length > 1);
            const name = res[0].split('HKEY_LOCAL_MACHINE\\SOFTWARE\\WOW6432Node\\Rockstar Games\\')[1];
            const paths = res.filter(x => x.trim().startsWith('Install')).filter(async x => await fs.readDir(x.split('REG_SZ')[1].trim()).catch(() => null));
            if (!paths[0]) return;
            const Install = paths[0].split('REG_SZ')[1].trim();
            return {
                name, Install,
            }
        }).filter(x => x).map(x => parseGameObject(x));
        return (await Promise.all(allGames)).filter(x => x);
    }
    else {
        return [];
    }
}

async function parseGameObject(data) {
    const Location = data.Install;
    const game = RockstarGamesData.titles.find(x => data.name === x.friendlyName || x.aliases?.includes(data.name));
    if (!game) return;
    let Executable;
    if (game.titleId === 'gta5') {
        const files = await fs.readDir(Location);
        Executable = files.find(x => game.modWhitelist.includes(x.name))?.name ?? game.executable;
    }
    else {
        Executable = game.executable;
    }

    return {
        DisplayName: game.friendlyName,
        GameID: `${game.titleId}-${game.rosTitleId}`,
        bannerId: game.bannerId,
        Executable,
        Location,
        Size: null,
        LauncherName: 'Rockstar',
    };
}

export {
    getInstalledGames,
}