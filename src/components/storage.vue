<script>
const fs = window.__TAURI__.fs;
const path = window.__TAURI__.path;

(async () => {
  const appDirPath = await path.appDir();

  let data = {};
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

  try {
    JSON.parse(await fs.readTextFile(appDirPath + "storage/LauncherData.json"));
  } catch (e) {
    fs.writeTextFile(appDirPath + "storage/LauncherData.json", CONSTANTS);
  }

  try {
    JSON.parse(await fs.readTextFile(appDirPath + "storage/UserProfile.json"));
  } catch (e) {
    fs.writeTextFile(appDirPath + "storage/UserProfile.json", JSON.stringify({ username: "Lazap" }));
  }

  try {
    JSON.parse(await fs.readTextFile(appDirPath + "storage/Cache/Games/Data.json"));
  } catch (e) {
    fs.writeTextFile(appDirPath + "storage/Cache/Games/Data.json", JSON.stringify({ }));
  }

})();

</script>
