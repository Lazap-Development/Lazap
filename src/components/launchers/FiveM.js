const fs = window.__TAURI__.fs;
const os = window.__TAURI__.os;
const path = window.__TAURI__.path;

async function getInstalledGames() {
	if (await os.platform() === 'win32') {
        try {
            await fs.readDir(`${await path.localDataDir()}\\FiveM\\FiveM.exe`);
        } catch {
            return [];
        }

		const Location = `${await path.localDataDir()}\\FiveM`;
		const Executable = 'FiveM.exe';
		return [{
			DisplayName: 'FiveM',
			LauncherName: 'FiveM',
			GameID: 'FiveM',
			Location,
			Executable,
			Args: [],
		}];
	}
	return [];
}

export  {
	getInstalledGames,
};