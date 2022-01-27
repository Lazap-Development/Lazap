let { exec } = require('child_process');
exec = require('util').promisify(exec);
const fs = require('fs');

async function getInstalledGames(os = process.platform) {
	let games = [];
	if (os === 'win32') {
		games = [...(await getMinecraftLauncher()), ...(await getLunarClient())];
	}
	return games;
}

async function getMinecraftLauncher() {
	const { stdout, error } = await exec('Reg query HKEY_CLASSES_ROOT\\Applications\\MinecraftLauncher.exe\\shell\\open\\command /ve').catch(() => { return { error: 'NOT_FOUND' }; });
	if (error) {
		const isInstalled = fs.existsSync('C:\\Program Files\\WindowsApps\\Microsoft.4297127D64EC6_1.0.113.0_x64__8wekyb3d8bbwe\\Minecraft.exe');
		if (!isInstalled) return [];
		const Location = 'C:\\Program Files\\WindowsApps\\Microsoft.4297127D64EC6_1.0.113.0_x64__8wekyb3d8bbwe';
		const Executable = 'Minecraft.exe';
		return [{
			DisplayName: 'Minecraft Launcher',
			LauncherName: 'Minecraft',
			GameID: 'Minecraft',
			Size: fs.statSync(Location),
			Location,
			Executable,
			Args: [],
		}];
	}
	else {
		const Location = stdout.split('REG_SZ')[1].split('\r\n\r\n')[0].trim().split('" "')[0].split('"').join('').split('\\').slice(0, -1).join('\\');
		const Args = stdout.split('REG_SZ')[1].split('\r\n\r\n')[0].trim().split('" "')[1].split('"').join('');
		const Executable = stdout.split('REG_SZ')[1].split('\r\n\r\n')[0].trim().split('" "')[0].split('"').join('').split('\\').slice(-1)[0];
		if (!fs.existsSync(Location)) return [];
		return [{
			DisplayName: 'Minecraft Launcher',
			LauncherName: 'Minecraft',
			GameID: 'Minecraft',
			Size: fs.statSync(Location),
			Location,
			Executable,
			Args: [Args],
		}];
	}
}

async function getLunarClient() {
	const isLunarInstalled = fs.existsSync('C:\\Users\\sunil\\.lunarclient');
	if (!isLunarInstalled) return [];
	const Location = 'C:\\Users\\sunil\\AppData\\Local\\Programs\\lunarclient';
	const Executable = 'Lunar Client.exe';
	return [{
		DisplayName: 'Lunar Client',
		LauncherName: 'Lunar',
		GameID: 'Lunar',
		Size: fs.statSync(Location),
		Location,
		Executable,
		Args: [],
	}];
}
/*
async function getBadlion() {
	exec('Reg query "Computer\\HKEY_LOCAL_MACHINE\\SOFTWARE\\1de14785-dd8c-5cd2-aae8-d4a376f81d78" /v InstallLocation');
}
*/
module.exports = {
	getInstalledGames,
};