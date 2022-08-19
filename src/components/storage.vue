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
  data.checkForUpdates = false;
  data.accentColor = "rgb(121, 52, 250)"
  let CONSTANTS = JSON.stringify(data);

  fs.createDir(appDirPath, { recursive: true }, (err) => {
    if (err) throw err;
  });
  fs.createDir(appDirPath + "storage", { recursive: true }, (err) => {
    if (err) throw err;
  });
  fs.createDir(appDirPath + "storage/cache", { recursive: true }, (err) => {
    if (err) throw err;
  });
  fs.createDir(appDirPath + "storage/cache/user", { recursive: true }, (err) => {
    if (err) throw err;
  })
  fs.createDir(appDirPath + "storage/cache/games", { recursive: true }, (err) => {
    if (err) throw err;
  });
  fs.createDir(appDirPath + "storage/cache/games/banners", { recursive: true }, (err) => {
    if (err) throw err;
  })

  try {
    JSON.parse(await fs.readTextFile(appDirPath + "storage/LauncherData.json"));
  } catch (e) {
    fs.writeTextFile(appDirPath + "storage/LauncherData.json", CONSTANTS);
  }

  try {
    JSON.parse(await fs.readTextFile(appDirPath + "storage/cache/user/UserProfile.json"));
  } catch (e) {
    fs.writeTextFile(appDirPath + "storage/cache/user/UserProfile.json", JSON.stringify({ username: "Lazap" }));
  }

  try {
    JSON.parse(await fs.readTextFile(appDirPath + "storage/cache/games/data.json"));
  } catch (e) {
    fs.writeTextFile(appDirPath + "storage/cache/games/data.json", JSON.stringify([]));
  }

})();

</script>
