const os = window.__TAURI__.os;
const fs = window.__TAURI__.fs;
const shell = window.__TAURI__.shell;
const path = window.__TAURI__.path;

async function getInstalledGames() {
	if (await os.platform() === 'win32') {
		// Minecraft Game Detection on Windows is Disabled until further notice. For more info check out https://discord.com/channels/644764850706448384/825224040641724416/997324642534559764
		return [await getMinecraftLauncher(), await getLunarClient()].filter(x => x !== false);
	}
	else if (await os.platform() === 'linux') {
		return [await getMinecraftLauncher(), await getLunarClient()].filter(x => x !== false);
	}
	else {
		return [];
	}
}

async function getMinecraftLauncher() {
	if (await os.platform() === 'win32') {
		return false;
	}
	else if (await os.platform() === 'linux') {
		try {
			const output = await new shell.Command('which', "minecraft-launcher").execute();

			if (output.stdout) {
				const homedir = await path.homeDir();
				try {
					await fs.readDir(`${homedir}/.minecraft`);
				} catch (e) {
					return console.log(e);
				}
				const Location = '/usr/bin/minecraft-launcher';
				const Executable = 'minecraft-launcher';
				return {
					DisplayName: 'Minecraft Launcher',
					LauncherName: 'Minecraft',
					GameID: 'Minecraft',
					Location,
					Executable,
					Args: [],
				};
			}
			else {
				return false;
			}
		}
		catch (e) {
			return false;
		}
	}
	else {
		return false;
	}
}

async function getLunarClient() {
	if (await os.platform() === 'win32') {
		return false;
	}
	else if (await os.platform() === 'linux') {
		try {
			const output = await new shell.Command('which', "lunarclient").execute();

			if (output.stdout) {
				const homedir = await path.homeDir();
				try {
					await fs.readDir(`${homedir}/.lunarclient`);
				} catch (e) {
					return console.log(e);
				}
				const Location = '/usr/bin/lunarclient';
				const Executable = 'lunarclient';
				return {
					DisplayName: 'Lunar Client',
					LauncherName: 'Lunar',
					GameID: 'Lunar',
					Location,
					Executable,
					Args: [],
				};
			}
			else {
				return false;
			}
		}
		catch (e) {
			return false;
		}
	}
	else {
		return false;
	}
}

export {
	getInstalledGames,
};