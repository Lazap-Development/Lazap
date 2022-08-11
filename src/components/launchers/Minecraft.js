const os = window.__TAURI__.os;
const fs = window.__TAURI__.fs;
const shell = window.__TAURI__.shell;
const path = window.__TAURI__.path;

async function getInstalledGames() {
	if (await os.platform() === 'win32') {
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
		const out = await new shell.Command('cmd', ['/C', 'Reg', 'query', 'HKEY_CLASSES_ROOT\\Applications\\MinecraftLauncher.exe\\shell\\open\\command', '/ve']).execute().catch(() => null);
		if (out?.stderr) {
			const isInstalled = await fs.readDir('C:\\Program Files\\WindowsApps\\Microsoft.4297127D64EC6_1.1.21.0_x64__8wekyb3d8bbwe').catch(() => null);
			if (!isInstalled) return false;
			const Location = 'C:\\Program Files\\WindowsApps\\Microsoft.4297127D64EC6_1.1.21.0_x64__8wekyb3d8bbwe';
			const Executable = 'Minecraft.exe';
			return {
				DisplayName: 'Minecraft Launcher',
				LauncherName: 'Minecraft',
				GameID: 'Minecraft',
				Size: null,
				Location,
				Executable,
				Args: [],
			};
		}
		else {
			console.warn(out);
			const Location = out?.stdout.split('REG_SZ')[1].split('\r\n\r\n')[0].trim().split('" "')[0].split('"').join('').split('\\').slice(0, -1).join('\\');
			console.warn(Location);
			const Args = out?.stdout.split('REG_SZ')[1].split('\r\n\r\n')[0].trim().split('" "')[1].split('"').join('');
			const Executable = out?.stdout.split('REG_SZ')[1].split('\r\n\r\n')[0].trim().split('" "')[0].split('"').join('').split('\\').slice(-1)[0];
			if (!(await fs.readDir(Location).catch(() => null))) return false;
			return {
				DisplayName: 'Minecraft Launcher',
				LauncherName: 'Minecraft',
				GameID: 'Minecraft',
				Size: null,
				Location,
				Executable,
				Args: [Args],
			};
		}
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
		const isLunarInstalled = await fs.readDir(`${await path.localDataDir()}Programs\\lunarclient`).catch(() => null);
		if (!isLunarInstalled) return false;
		const Location = `${await path.localDataDir()}Programs\\lunarclient`;
		const Executable = 'Lunar Client.exe';
		return {
			DisplayName: 'Lunar Client',
			LauncherName: 'Lunar',
			GameID: 'Lunar',
			Size: null,
			Location,
			Executable,
			Args: [],
		};
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