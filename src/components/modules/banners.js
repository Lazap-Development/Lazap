const http = window.__TAURI__.http;
const invoke = window.__TAURI__.invoke;
const path = window.__TAURI__.path;
const tauri = window.__TAURI__.tauri;
const { sha256 } = require("../modules/sha256");

async function getBanners(games) {
  games = games.filter((x) => !["CustomGame"].includes(x.LauncherName));
  const bannerBasePath = (await path.appDir()) + "cache/games/banners";
  const readBanners = await invoke('read_dir', { dirPath: bannerBasePath })

  let alreadyProcessed = false;
  let existingProcessed = 0;

  for (let i = 0; i < games.length; i++) {
    if (
      readBanners.includes(
        `${sha256(games[i].DisplayName.replaceAll(" ", "_"))}.png`
      )
    ) {
      existingProcessed++;
      if (existingProcessed === games.length) {
        alreadyProcessed = true;
      }
    }
  }
  if (alreadyProcessed === true) {
    return console.log(
      "%c[BANNER] " + "Banners are already loaded. Skipping.",
      "color:blue"
    );
  }

  const arr = [];
  for (let i = 0; i < games.length; i++) {
    arr.push(
      (async () => {
        switch (games[i].LauncherName) {
          case "Steam": {
            return `https://cdn.akamai.steamstatic.com/steam/apps/${games[i].GameID}/library_600x900.jpg`;
          }
          case "RiotGames": {
            return "https://valorant-config.fr/wp-content/uploads/2020/05/7d604cf06abf5866f5f3a2fbd0deacf9-200x300.png";
          }
          case "Minecraft": {
            return "https://i.imgur.com/PJFx3U2.jpg";
          }
          case "FiveM": {
            return "https://logos-world.net/wp-content/uploads/2021/03/FiveM-Symbol.png";
          }
          case "Lunar": {
            return "https://www.lunarclient.com/assets/img/default-twitter-icon.webp";
          }
          case "Lutris": {
            if (
              games[i].DisplayName.replaceAll(" ", "_") === "Epic_Games_Store"
            )
              return "https://pcper.com/wp-content/uploads/2021/02/epic-games-store.png";
            if (
              games[i].DisplayName.replaceAll(" ", "_") ===
              "Rockstar_Games_Launcher"
            )
              return "https://cdn.player.one/sites/player.one/files/2019/08/26/rockstar-games.png";
            return;
          }
          case "XboxGames": {
            return games[i].Banner;
          }
          case "Osu": {
            return "https://cdn2.steamgriddb.com/file/sgdb-cdn/grid/a5d7420f9fdc41087377b4d58c5fe94b.png";
          }
          case "EpicGames": {
            const fetchEpicGame = await http.fetch(
              `https://api.rawg.io/api/games?key=f8854c401fed44b89f4e1e4faa56ccc8&search=${games[
                i
              ].DisplayName.replaceAll(
                " ",
                "-"
              )}&search_exact&search_precise&stores=11`,
              {
                method: "GET",
                mode: "no-cors",
                headers: {
                  Accept: "application/json",
                  "Content-Type": "application/json",
                },
              }
            );
            if (!fetchEpicGame.data.results[0]) break;
            return (
              fetchEpicGame.data.results[0].background_image.slice(0, 27) +
              "/crop/600/400" +
              fetchEpicGame.data.results[0].background_image.slice(27)
            );
          }
          case "Uplay": {
            const fetchUplay = await http.fetch(
              `https://api.rawg.io/api/games?key=f8854c401fed44b89f4e1e4faa56ccc8&search=${games[
                i
              ].DisplayName.replaceAll(" ", "-")}&search_exact&search_precise`,
              {
                method: "GET",
                mode: "no-cors",
                headers: {
                  Accept: "application/json",
                  "Content-Type": "application/json",
                },
              }
            );
            if (!fetchUplay.data.results[0]) break;
            return (
              fetchUplay.data.results[0].background_image.slice(0, 27) +
              "/crop/600/400" +
              fetchUplay.data.results[0].background_image.slice(27)
            );
          }
          case "Rockstar": {
            return `https://media-rockstargames-com.akamaized.net/rockstargames-newsite/img/global/games/fob/640/${games[i].bannerId}.jpg`;
          }
        }
      })()
    );
  }

  cacheBanners(
    games,
    arr.filter((x) => x)
  );
  return arr;
}

async function cacheBanners(data, res) {
  const bannerBasePath = (await path.appDir()) + "cache/games/banners";

  if (data?.length === 0) {
    return console.log("%c[BANNER] " + "No games to process", "color:blue");
  }

  let fetchProcessed = 0;

  if (res.length === 0) {
    return console.log("%c[BANNER] " + "No banners to load.", "color:blue");
  }

  res
    .filter(async (x) => (await x)?.startsWith("http"))
    .forEach(async (x, i) => {
      await http
        .fetch(await x, {
          method: "GET",
          headers: {
            "Content-Type": "text/plain; charset=utf-8",
          },
          responseType: 3,
        })
        .then(async (response) => {
          if (response.status === 404 && data[i].LauncherName === "Lutris")
            return;
          await invoke("write_binary_file", {
            filePath:
              bannerBasePath +
              `/${sha256(data[i].DisplayName.replaceAll(" ", "_"))}.png`,
            fileContent: response.data,
          });
          document
            .getElementById(
              `game-div-${data[i].DisplayName.replaceAll(" ", "_")}`
            )
            ?.firstElementChild?.setAttribute(
              "src",
              tauri.convertFileSrc(
                bannerBasePath +
                  `/${sha256(data[i].DisplayName.replaceAll(" ", "_"))}.png`
              )
            );
        })
        .catch((e) => console.error(e));

      fetchProcessed++;
      if (fetchProcessed === data.length) {
        return console.log(
          "%c[BANNER] " + "Just finished processing banners.",
          "color:blue"
        );
      }
    });
}

export { getBanners };
