const os = window.__TAURI__.os;
const fs = window.__TAURI__.fs;
const shell = window.__TAURI__.shell;
const path = window.__TAURI__.path;

async function getInstalledGames() {
	if (await os.platform() == 'win32') {
		return [await getMinecraftLauncher()].filter(x => x !== false);
	}
	else if (await os.platform() == 'linux') {
		return [await getMinecraftLauncherOnLinux()].filter(x => x !== false);
	}
	else {
		return [];
	}
}

async function getMinecraftLauncher() {
	const output = await new shell.Command('cmd', ["/C", "Reg", "Query", `HKEY_CLASSES_ROOT\\Applications\\MinecraftLauncher.exe\\shell\\open\\command`, "/ve"]).execute();
	if (!output.stdout) {
		try {
			await fs.readTextFile('C:\\Program Files\\WindowsApps\\Microsoft.4297127D64EC6_1.0.113.0_x64__8wekyb3d8bbwe\\Minecraft.exe'); 
		} catch (err) {
			return false;
		}
		const Location = 'C:\\Program Files\\WindowsApps\\Microsoft.4297127D64EC6_1.0.113.0_x64__8wekyb3d8bbwe';
		const Executable = 'Minecraft.exe';
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
		const Location = output.stdout.split('REG_SZ')[1].split('\r\n\r\n')[0].trim().split('" "')[0].split('"').join('').split('\\').slice(0, -1).join('\\');
		const Args = output.stdout.split('REG_SZ')[1].split('\r\n\r\n')[0].trim().split('" "')[1].split('"').join('');
		const Executable = output.stdout.split('REG_SZ')[1].split('\r\n\r\n')[0].trim().split('" "')[0].split('"').join('').split('\\').slice(-1)[0];
		if (!fs.readDir(Location)) return {};
		return {
			DisplayName: 'Minecraft Launcher',
			LauncherName: 'Minecraft',
			GameID: 'Minecraft',
			Size: fs.statSync(Location).size,
			Location,
			Executable,
			Args: [Args],
		};
	}
}

async function getMinecraftLauncherOnLinux() {
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
		return;
	}
}

export {
	getInstalledGames,
};