const shell = window.__TAURI__.shell;
const os = window.__TAURI__.os;
const http = window.__TAURI__.http;
const invoke = window.__TAURI__.invoke;

async function getInstalledGames() {
  if ((await os.platform()) === "win32") {
    const { stdout, stderr } = await new shell.Command("cmd", [
      "/C",
      "C:/Windows/System32/WindowsPowerShell/v1.0/powershell.exe",
      "get-appxpackage",
    ]).execute();
    if (stderr) {
      console.log(stderr);
      return [];
    }
    let apps = await parseRawToJSON(stdout).filter(
      (x) =>
        x.IsFramework !== "True" &&
        x.SignatureKind === "Store" &&
        x.PublisherId !== "8wekyb3d8bbwe" &&
        !x.PackageFullName.startsWith("MicrosoftWindows")
    );
    apps = (await Promise.all(verifyGames(apps))).filter(async (x) => {
      return typeof (await x) === "object";
    });
    apps = apps.filter((x) => x?.Banner);
    return (await Promise.all(apps.map((x) => parseGmeObject(x)))).filter(
      (x) => typeof x === "object"
    );
  } else {
    return [];
  }
}

function parseRawToJSON(res) {
  return res
    .split("\r\n\r\n")
    .slice(1)
    .map((x) => {
      return x
        .split("\r\n")
        .filter((y) => !y.startsWith(" "))
        .map((y) =>
          y
            .split(":", 3)
            .map(
              (z, i, arr) =>
                `${
                  arr.length === 3 && i !== 0 ? (i === 2 ? "" : '"') : '"'
                }${z.trim()}${
                  arr.length === 3 && i !== 0 ? (i === 2 ? '"' : "") : '"'
                }`
            )
            .join(":")
        )
        .join(",\n")
        .replaceAll("\\", "/");
    })
    .filter((x) => x.length > 20)
    .map((x) => {
      return JSON.parse(`{${x}}`);
    });
}

async function parseGmeObject(obj) {
  const { Banner, InstallLocation } = obj;
  const Location = InstallLocation;
  const manifest = (
    await invoke("read_file", {filePath: Location + "/AppxManifest.xml"})
  ).split("\r\n");
  const Executable = manifest
    .find((x) => x.trim().startsWith("<Application "))
    .split(/<[/]{0,1}Application[>]{0,1}/)[1]
    .match(/Executable="[a-z0-9\s-]{1,}\.exe"/i);
  if (!Executable) return;

  return {
    Executable: Executable[0].split('"')[1],
    Location,
    DisplayName: obj.Name,
    GameID: obj.PackageFullName,
    LaunchID: `start shell:AppsFolder\\${obj.PackageFamilyName}!${
      manifest
        .find((x) => x.trim().startsWith("<Application "))
        .split(/<[/]{0,1}Application[>]{0,1}/)[1]
        .match(/Id="[a-z]{1,}"/i)[0]
    }`.replace('!Id="App"', "!App"),
    Size: null,
    Banner,
    LauncherName: "XboxGames",
  };
}

function verifyGames(jsons) {
  const arr = [];
  for (let i = 0; i < jsons.length; i++) {
    const json = jsons[i];
    arr.push(
      (async () => {
        json.Name = json.Name.split(".")[1];
        const response = await http
          .fetch(
            `https://www.microsoft.com/en-in/search/shop/games?q=${encodeURIComponent(
              json.Name
            )}&devicetype=pc`,
            { responseType: 2 }
          )
          .catch(() => "");
        if (response === "") return;
        const dom = JSON.parse(
          await invoke("parse", {
            value: response.data,
          })
        );

        const list = dom.children
          .find((x) => x.name === "html")
          .children.find((x) => x.name === "body")
          .children.find((x) =>
            x.attributes
              ? x.attributes["data-grid"] === "container pad-12x stack-2"
              : false
          )
          ?.children.find((x) => x.name === "section")
          .children[0].children.find((x) =>
            x.id ? x.id.includes("productplacementlist") : false
          )?.children;
        if (list.find((x) => x.name === "p")) return false;

        const results = list
          .find((x) => !x.attributes)
          .children.find((x) => x.name === "div" && !x.attributes).children;

        json.Name = results[0].children[0].children
          .find((x) => x.id?.startsWith("coreui"))
          .children.find((x) => x.name === "h3").children[0];

        json.Banner = results[0].children[0].children
          .find((x) => !x.id?.startsWith("coreui"))
          .children[0].children.find((x) => x.name === "img")
          .attributes["data-src"].replaceAll("amp;", "")
          .replace(/w=162/, "w=600")
          .replace(/h=300/, "h=900");
        return json;
      })()
    );
  }
  return arr;
}

export { getInstalledGames };
