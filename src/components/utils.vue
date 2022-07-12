<script>
const app = window.__TAURI__.app;
const fs = window.__TAURI__.fs;
const path = window.__TAURI__.path;


const handleStorageAndTransportData = (mainWindow, userDataPath) => {
    let LauncherData = JSON.parse(fs.readFileSync(userDataPath + '/storage/Settings/userprofile.json').toString());
    if (!Object.keys(LauncherData).length) {
        const os = window.__TAURI__.os;
        LauncherData = {
            username: os.userInfo().username,
            pfp: 'default',
        };
        fs.writeFileSync(userDataPath + 'storage/Settings/userprofile.json', JSON.stringify(LauncherData));
    }
    else {
        if (LauncherData.pfp !== 'default' && !fs.existsSync(LauncherData.pfp)) {
            LauncherData.pfp = 'default';
        }
        fs.writeFileSync(userDataPath + 'storage/Settings/userprofile.json', JSON.stringify(LauncherData));
    }

    mainWindow.webContents.send('load-profile', LauncherData);
};

/*const rpcUpdate = (data) => {
  const LauncherData = JSON.parse(fs.readFileSync(userDataPath + '/storage/Settings/LauncherData.json').toString());
  if (LauncherData.enableRPC !== true && typeof data === 'object' && data !== null) return 'DISABLED';
  if (!data) {
    rpcClient.clearActivity();
    return;
  }
  rpcClient.setActivity(data).catch(err => console.warn('[RPC]', err.stack.includes('connection closed') ? 'OFFLINE' : err));
}*/

const restart = () => {
  app.relaunch();
  app.exit();
}

const checkForDirAndCreate = (dir, fileContent = '', userDataPath) => {
  if (fs.existsSync(dir.split(userDataPath)[1])) return true;
  dir.split(userDataPath)[1].split('/').slice(1).forEach((name, i, arr) => {
    dir = dir.replaceAll('\\', '/');
    if (!fs.existsSync(userDataPath + `/${arr.slice(0, i + 1).join('/')}`)) {
      if (name.split('.')[1]) {
        fs.writeFileSync(userDataPath + `/${arr.slice(0, i + 1).join('/')}`, fileContent);
        return;
      }
      else {
        fs.mkdirSync(userDataPath + `/${arr.slice(0, i + 1).join('/')}`);
      }
    }
  });
};

const setLaunchOnStartup = (e, bool) => app.setLoginItemSettings({ 'openAtLogin': bool, 'enabled': bool });

const updateSetting = async (e, key, bool) => {
  const userDataPath = await path.appDir();
  const requireRestart = ['disableHardwareAcceleration'];
  checkForDirAndCreate(userDataPath + '/storage/Settings/LauncherData.json', '{}', app.getPath('userData', userDataPath));
  const LauncherData = JSON.parse(fs.readFileSync(userDataPath + '/storage/Settings/LauncherData.json').toString());
  LauncherData[key] = bool;
  fs.writeFileSync(userDataPath + '/storage/Settings/LauncherData.json', JSON.stringify(LauncherData));
  if (requireRestart.includes(key)) restart();

  switch (key) {
  case 'enableRPC': {
    // rpcUpdate(bool ? { details: 'On Main Screen', startTimestamp: Date.now(), largeImageKey: 'lazap' } : undefined);
    break;
  }
  case 'launchOnStartup': {
    setLaunchOnStartup('setLaunchOnStartup', bool);
    break;
  }
  }
};

module.exports = {
    handleStorageAndTransportData,
    // rpcUpdate,
    updateSetting,
};
</script>