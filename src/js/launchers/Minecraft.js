let { exec } = require('child_process');
exec = require('util').promisify(exec);
const fs = require('fs');

async function getInstalledGames(os = process.platform) {
	if (os === 'win32') {
		const { stdout, error } = await exec('Reg query HKEY_CLASSES_ROOT\\Applications\\MinecraftLauncher.exe\\shell\\open\\command /ve').catch(() => { return { error: 'NOT_FOUND' }; });
		if (error) {
			return [];
		}
		else {
			const Location = stdout.split('REG_SZ')[1].split('\r\n\r\n')[0].trim().split('" "')[0].split('"').join('').split('\\').slice(0, -1).join('\\');
			const Args = stdout.split('REG_SZ')[1].split('\r\n\r\n')[0].trim().split('" "')[1].split('"').join('');
			const Executable = stdout.split('REG_SZ')[1].split('\r\n\r\n')[0].trim().split('" "')[0].split('"').join('').split('\\').slice(-1)[0];
			return [{
				DisplayName: 'Minecraft Launcher',
				LauncherName: 'Minecraft',
				LaunchCommand: null,
				GameID: 'Minecraft',
				Size: fs.statSync(Location),
				Location,
				Executable,
				Args: [Args],
			}];
		}
	}
	return [];
}

module.exports = {
	getInstalledGames,
};