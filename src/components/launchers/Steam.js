const os = window.__TAURI__.os;
const fs = window.__TAURI__.fs;
const path = window.__TAURI__.path;
const shell = window.__TAURI__.shell;
const invoke = window.__TAURI__.invoke;

async function getSteamLocation() {
  let launcher_location;
  let registry_res;
  if ((await os.platform()) === "win32") {
    const output = await new shell.Command("cmd", [
      "/C",
      "Reg",
      "Query",
      `HKEY_LOCAL_MACHINE\\SOFTWARE\\${
        (await os.arch()) === "x86_64" ? "WOW6432Node\\" : ""
      }Valve\\Steam`,
      "/v",
      "InstallPath",
    ]).execute();
    if (!output.stdout) return [];
    else {
      registry_res = output.stdout;
      const steamDir = registry_res
        .split("REG_SZ")[1]
        .split("\r\n\r\n")[0]
        .trim();
      let text;
      try {
        text = await invoke("read_file", {
          filePath: steamDir + `\\steamapps\\libraryfolders.vdf`
        })
      } catch (e) {
        return [];
      }

      const VDF = require("../modules/parseVDF");
      const parsed = VDF.parse(text);
      const toArray = Object.entries(parsed.libraryfolders).filter(
        (x) => x[1].path
      );

      launcher_location = toArray.map((item) => {
        return item[1].path;
      });
    }
  } else if ((await os.platform()) === "linux") {
    const homedir = await path.homeDir();
    try {
      await invoke("read_file", {
        filePath: homedir + `.steam/steam/steamapps/libraryfolders.vdf`
      })
    } catch (err) {
      console.error(err)
      return [];
    }

    const text = await invoke("read_file", {
      filePath: homedir + `.steam/steam/steamapps/libraryfolders.vdf`
    })
    if (text.length === 0) return (launcher_location = []);
    const VDF = require("../modules/parseVDF");
    const parsed = VDF.parse(text);
    const toArray = Object.entries(parsed.libraryfolders);
    launcher_location = toArray.map((item) => {
      return item[1].path;
    });
  }

  for (const path of launcher_location) {
    try {
      await fs.readDir(path);
    } catch (err) {
      launcher_location = launcher_location.filter((e) => e !== path);
    }
  }
  return launcher_location;
}

async function getInstalledGames() {
  const path = await getSteamLocation();
  if (path?.length === 0) return [];

  let allGames = [];

  for (const location of path) {
    let acf_basePath;
    if ((await os.platform()) === "win32") {
      acf_basePath = `${location}\\steamapps`;
    } else if ((await os.platform()) === "linux") {
      acf_basePath = `${location}/steamapps`;
    }

    if (!(await fs.readDir(acf_basePath))) return [];

    const readDirOfBasePath = await fs.readDir(acf_basePath);
    const acf_files = await Promise.all(
      readDirOfBasePath
        .filter((x) => x.name.split(".")[1] === "acf")
        .map(async (x) => {
          return parseGameObject(
            acf_to_json(
              await fs.readTextFile(
                `${acf_basePath}/${JSON.stringify(x.name).replace(
                  /['"]+/g,
                  ""
                )}`
              )
            )
          );
        })
    );

    allGames.push(acf_files);

    const result = allGames.flat().reduce((unique, o) => {
      if (!unique.some((obj) => obj.DisplayName === o.DisplayName)) {
        unique.push(o);
      }
      return unique;
    }, []);
    allGames = result;
  }
  return allGames;
}

async function parseGameObject(acf_object = {}) {
  const {
    name: DisplayName,
    appid: GameID,
    LauncherExe: Executable,
    LauncherPath: Location,
    BytesDownloaded: Size,
  } = await acf_object;

  return {
    DisplayName,
    GameID,
    Executable,
    Location,
    Size: parseInt(Size),
    LauncherName: "Steam",
  };
}

function acf_to_json(acf_content = "") {
  if (acf_content.length === 0) return;
  return JSON.parse(
    acf_content
      .split("\n")
      .slice(1)
      .map((x, i, arr) => {
        if (x.length === 0) return;
        if (x.trim().includes("\t\t")) {
          return (
            x.trim().replace("\t\t", ":") +
            (["{", "}"].includes(arr[i + 1]?.trim().slice(0, 1)) ? "" : ",")
          );
        }
        return x.split('"').length > 1
          ? x.trim() + ":"
          : x +
              (x.trim() === "{" ||
              !arr[i + 1] ||
              ["{", "}"].includes(arr[i + 1]?.trim().slice(0, 1))
                ? ""
                : ",");
      })
      .join("\n")
  );
}

export { getInstalledGames, parseGameObject, acf_to_json };
