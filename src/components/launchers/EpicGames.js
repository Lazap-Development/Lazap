const os = window.__TAURI__.os;
const invoke = window.__TAURI__.invoke;

async function getInstalledGames() {
  if ((await os.platform()) === "win32") {
    if (!(await isLauncherInstalled())) return [];
    const read = await invoke("read_dir", {
      dirPath: "C:\\ProgramData\\Epic\\EpicGamesLauncher\\Data\\Manifests",
    });
    const games = read
      .filter((x) => x.split(".")[1]?.toLowerCase() === "item")
      .map(async (x) =>
        JSON.parse(
          await invoke("read_file", {
            filePath: `C:\\ProgramData\\Epic\\EpicGamesLauncher\\Data\\Manifests\\${JSON.stringify(
              x
            ).replace(/['"]+/g, "")}`,
          })
        )
      );

    return await Promise.all(games.map((x) => parseGameObject(x)));
  }
  return [];
}

async function isLauncherInstalled(
  path = "C:\\ProgramData\\Epic\\EpicGamesLauncher\\Data\\Manifests"
) {
  try {
    return await invoke("d_f_exists", { path: path });
  } catch (err) {
    return false;
  }
}

async function parseGameObject(rawObj = {}) {
  const {
    LaunchExecutable: Executable,
    InstallLocation: Location,
    DisplayName,
    AppName: GameID,
    InstallSize: Size,
    LaunchCommand,
    CatalogNamespace,
    CatalogItemId,
    AppName,
  } = await rawObj;

  return {
    Executable,
    Location,
    DisplayName,
    GameID,
    LaunchID: `${CatalogNamespace}:${CatalogItemId}:${AppName}`,
    Size,
    LaunchCommand,
    LauncherName: "EpicGames",
  };
}

export { getInstalledGames, parseGameObject };
