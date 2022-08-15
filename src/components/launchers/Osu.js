const os = window.__TAURI__.os;
const fs = window.__TAURI__.fs;
const path = window.__TAURI__.path;

async function getInstalledGames() {
	const platform = await os.platform();
	if (platform === 'win32') return await fetchOsu();
	else return [];
}

async function fetchOsu() {
	const platform = await os.platform();
	const homedir = await path.homeDir();
	if (platform === 'win32') {
		const isInstalled = await fs.readDir(`${homedir}\\Appdata\\Local\\osu!`).catch(() => false);
		if (!isInstalled) return false
		else {
			const Location = `${homedir}\\Appdata\\Local\\osu!`;
			const Args = "";
			const Executable = `${homedir}\\Appdata\\Local\\osu!\\osu!.exe`;
			if (!(await fs.readDir(Location).catch(() => null))) return false;
			return {
				DisplayName: 'Osu!',
				LauncherName: 'Osu',
				GameID: 'osu',
				Size: null,
				Location,
				Executable,
				Args: [Args],
			};
		}
	}
	else return false;
}

export {
	getInstalledGames,
};
