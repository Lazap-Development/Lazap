<script>

let { exec } = window.__TAURI__.child_process;
exec = window.__TAURI__.util.promisify(exec);
const fs = window.__TAURI__.fs;
const process = window.__TAURI__.os;
const user = process.userInfo().username;
async function getInstalledGames(os = process.platform) {
  if (os === 'win32') {
    return [await getMinecraftLauncher(), await getLunarClient()].filter(x => x !== false);
  }
  else if (os === 'linux') {
    return [await getMinecraftLauncherOnLinux(), await getLunarClient()].filter(x => x !== false);
  }
  else {
    return [];
  }
}

async function getMinecraftLauncher() {
  const { stdout, error } = await exec('Reg query HKEY_CLASSES_ROOT\\Applications\\MinecraftLauncher.exe\\shell\\open\\command /ve').catch(() => { return { error: 'NOT_FOUND' }; });
  if (error) {
    const isInstalled = fs.existsSync('C:\\Program Files\\WindowsApps\\Microsoft.4297127D64EC6_1.0.113.0_x64__8wekyb3d8bbwe\\Minecraft.exe');
    if (!isInstalled) return false;
    const Location = 'C:\\Program Files\\WindowsApps\\Microsoft.4297127D64EC6_1.0.113.0_x64__8wekyb3d8bbwe';
    const Executable = 'Minecraft.exe';
    return {
      DisplayName: 'Minecraft Launcher',
      LauncherName: 'Minecraft',
      GameID: 'Minecraft',
      Size: fs.statSync(Location).size,
      Location,
      Executable,
      Args: [],
    };
  }
  else {
    const Location = stdout.split('REG_SZ')[1].split('\r\n\r\n')[0].trim().split('" "')[0].split('"').join('').split('\\').slice(0, -1).join('\\');
    const Args = stdout.split('REG_SZ')[1].split('\r\n\r\n')[0].trim().split('" "')[1].split('"').join('');
    const Executable = stdout.split('REG_SZ')[1].split('\r\n\r\n')[0].trim().split('" "')[0].split('"').join('').split('\\').slice(-1)[0];
    if (!fs.existsSync(Location)) return {};
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
  const { error } = await exec('which minecraft-launcher').catch(() => { return { error: 'NOT_FOUND' }; });
  if (!error) {
    const homedir = window.__TAURI__.is.userInfo().homedir;
    const isInstalled = fs.existsSync(`${homedir}/.minecraft`);
    if (!isInstalled) return false;
    const Location = '/usr/bin/minecraft-launcher';
    const Executable = 'minecraft-launcher';
    return {
      DisplayName: 'Minecraft Launcher',
      LauncherName: 'Minecraft',
      GameID: 'Minecraft',
      Size: fs.statSync(Location).size,
      Location,
      Executable,
      Args: [],
    };
  }
  else {
    return false;
  }
}

function getLunarClient() {
  if(process.platform === 'win32') {
    const isLunarInstalled = fs.existsSync(`C:\\Users\\${user}\\.lunarclient`);
    if (!isLunarInstalled) return false;
    const Location = `C:\\Users\\${user}\\AppData\\Local\\Programs\\lunarclient`;
    const Executable = 'Lunar Client.exe';
    return {
      DisplayName: 'Lunar Client',
      LauncherName: 'Lunar',
      GameID: 'Lunar',
      Size: fs.statSync(Location).size,
      Location,
      Executable,
      Args: [],
    };
  } else if (process.platform === 'linux') {
    const homedir = window.__TAURI__.os.userInfo().homedir;
    const isLunarInstalled = fs.existsSync(`${homedir}/.config/lunarclient`);
    const Location = `${homedir}/.lunarclient`;
    const Executable = 'lunarclient';
    if (!isLunarInstalled) return false;
    return {
      DisplayName: 'Lunar Client',
      LauncherName: 'Lunar',
      GameID: 'Lunar',
      Size: fs.statSync(Location).size,
      Location,
      Executable,
      Args: [],
    };
  }
}
/*
async function getBadlion() {
	exec('Reg query "Computer\\HKEY_LOCAL_MACHINE\\SOFTWARE\\1de14785-dd8c-5cd2-aae8-d4a376f81d78" /v InstallLocation');
}
*/
module.exports = {
  getInstalledGames,
};
</script>