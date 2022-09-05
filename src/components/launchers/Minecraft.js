const os = window.__TAURI__.os;
const fs = window.__TAURI__.fs;
const shell = window.__TAURI__.shell;
const path = window.__TAURI__.path;

async function getInstalledGames() {
	const platform = await os.platform();
	if (platform === 'win32' || platform === 'linux') {
		const e = [await getMinecraftLauncher(platform), await getLunarClient(platform)].filter(x => x !== false);
		return e;
	}
	else {
		return [];
	}
}

async function getMinecraftLauncher(platform) {
	if (platform === 'win32') {
		const out = await new shell.Command('cmd', ['/C', 'Reg', 'query', 'HKEY_CLASSES_ROOT\\Applications\\MinecraftLauncher.exe\\shell\\open\\command']).execute().catch(() => null);
		if (out?.stderr) {
			const isInstalled = (await new shell.Command('cmd', ['/C', 'powershell', 'Get-appxpackage', 'Microsoft.4297127D64EC6']).execute().catch(() => null))?.stdout;
			if (isInstalled?.length < 1) return false;
			const Location = isInstalled.split('\r\n').find(x => x.trim().startsWith('InstallLocation')).split(':').slice(1).join(':').trim();
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
			const Location = out?.stdout.split('REG_SZ')[1].split('\r\n\r\n')[0].trim().split('" "')[0].split('"').join('').split('\\').slice(0, -1).join('\\');
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
	else if (platform === 'linux') {
		try {
			const output = await new shell.Command('which', "minecraft-launcher").execute();

			if (output.stdout) {
				const homedir = await path.homeDir();
				try {
					await fs.readDir(`${homedir}/.minecraft`);
				} catch (e) {
					return console.error(e);
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

async function getLunarClient(platform) {
	if (platform === 'win32') {
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
	else if (platform === 'linux') {
		try {
			const output = await new shell.Command('which', "lunarclient").execute();

			if (output.stdout) {
				const homedir = await path.homeDir();
				try {
					await fs.readDir(`${homedir}/.lunarclient`);
				} catch (e) {
					return console.error(e);
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
