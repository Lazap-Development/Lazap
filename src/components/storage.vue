<script>
const fs = window.__TAURI__.fs;
const path = window.__TAURI__.path;

(async () => {
  const appDirPath = await path.appDir();

  let data = new Object();
  data.enableRPC = true;
  data.launchOnStartup = false;
  data.skipLogin = false;
  data.trayMinLaunch = true;
  data.trayMinQuit = false;
  data.checkForUpdates = false
  let CONSTANTS = JSON.stringify(data);

  fs.createDir(appDirPath, { recursive: true }, (err) => {
    if (err) throw err;
  });
  fs.createDir(appDirPath + "storage", { recursive: true }, (err) => {
    if (err) throw err;
  });
  fs.createDir(appDirPath + "storage/Cache", { recursive: true }, (err) => {
    if (err) throw err;
  });
  fs.createDir(appDirPath + "storage/Cache/Games", { recursive: true }, (err) => {
    if (err) throw err;
  });

  if (!fs.readTextFile(appDirPath + 'storage/LauncherData.json')) {
    fs.writeTextFile(appDirPath + "storage/LauncherData.json", CONSTANTS, { recursive: true }, (err) => {
      if (err) throw err;
    });
  }

  if (!fs.readTextFile(appDirPath + 'storage/UserProfile.json')) {
    fs.writeTextFile(appDirPath + "storage/UserProfile.json", JSON.stringify({ username: "Lazap" }), { recursive: false }, (err) => {
      if (err) throw err;
    });
  }

  if (!fs.readTextFile(appDirPath + 'storage/Cache/Games/Data.json')) {
    fs.writeTextFile(appDirPath + "storage/Cache/Games/Data.json", CONSTANTS, { recursive: true }, (err) => {
      if (err) throw err;
    });
  }
})();

</script>
