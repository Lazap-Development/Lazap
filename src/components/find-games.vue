<script>
const currentRpc = require("./modules/rpcOptions.js").currentRpc;
const selectOption = require("./modules/rpcOptions.js").selectOption;
const os = window.__TAURI__.os;
const invoke = window.__TAURI__.invoke;
const path = window.__TAURI__.path;
const Window = window.__TAURI__.window;
const tauri = window.__TAURI__.tauri;
const { sha256 } = require("./modules/sha256.js");
let timestamp = null;
let processes = new Map();
let loads = 0;
let __CACHE__ = [];

class Elements {
  static getGameElement(game, id) {
    const gameElement = document.createElement("div");

    gameElement.id = `game-div-${game.DisplayName.replaceAll(" ", "_")}`;
    gameElement.className +=
      id.startsWith("recent") && id.includes("Main")
        ? "mainPageGamebox"
        : "gamebox";
    gameElement.style.diplay = "table";

    return gameElement;
  }

  static async getGameBannerElement(game) {
    const appDirPath = await path.appDir();
    const GAME_BANNERS_BASE_PATH = `${appDirPath}cache/games/banners`;

    const gameBanner = document.createElement("img");
    gameBanner.classList.add("game_banner_img");

    let banner;
    const dirs = await invoke("read_dir", {
      dirPath: GAME_BANNERS_BASE_PATH,
    });
    const img = dirs.find(
      (x) => x === `${sha256(game.DisplayName.replaceAll(" ", "_"))}.png`
    );
    if (img) {
      banner = img
        ? tauri.convertFileSrc(
            appDirPath +
              `cache/games/banners/${JSON.stringify(img).slice(1, -1)}`
          )
        : false;
    } else if (game.Banner) {
      banner = game.Banner;
    } else {
      banner = false;
    }

    if (banner !== false) {
      gameBanner.setAttribute("src", banner);
      gameBanner.style.content = "none";
    }
    gameBanner.height = 500;
    gameBanner.width = 500;
    game.Banner = banner;

    return gameBanner;
  }
  static getGameDisplayElement(game) {
    // Set Game Display Name
    const gameText = document.createElement("span");
    if (game.DisplayName.length > 17) {
      gameText.innerHTML = game.DisplayName.slice(0, 17);
      gameText.innerHTML += "...";
    } else {
      gameText.innerHTML = game.DisplayName;
    }

    return gameText;
  }

  static async getMenuElement(name) {
    const menuIcon = document.createElement("div");
    menuIcon.classList.add("menu");
    menuIcon.id = "menu";

    menuIcon.addEventListener("click", () => {
      document.getElementById("gameMenu").style.display === "flex"
        ? (document.getElementById("gameMenu").style.display = "none")
        : (document.getElementById("gameMenu").style.display = "flex");

      document.getElementById("gameMenuTitle").innerHTML = name;
    });

    return menuIcon;
  }

  static getLauncherIconElement(LauncherName) {
    const gameLauncherIcon = document.createElement("img");
    const icon = require("./modules/icons").getLauncherIcon(LauncherName);

    gameLauncherIcon.classList.add("gamebox-icon");
    gameLauncherIcon.setAttribute("src", icon);

    return gameLauncherIcon;
  }
}

async function createProcess_windows(
  Command,
  { GameID, DisplayName, LauncherName },
  force = false
) {
  if (processes.get(GameID) && !force) return "RUNNING_ALREADY";
  VisibilityState({ LauncherName, DisplayName });

  const instance = invoke("launch_game", {
    exec: Command,
    args: "",
  }).then(() => {
    VisibilityState({ LauncherName, DisplayName });
    processes.delete(GameID);
  });
  processes.set(GameID, instance);

  return instance;
}
async function createProcess_linux(
  Command,
  Args,
  { GameID, LauncherName, DisplayName },
  force = false
) {
  if (processes.get(GameID) && !force) return "RUNNING_ALREADY";
  VisibilityState({ LauncherName, DisplayName });

  const instance = invoke("launch_game", {
    exec: Command,
    args: Args,
  }).then(() => {
    VisibilityState({ LauncherName, DisplayName });
    processes.delete(GameID);
  });
  processes.set(GameID, instance);

  return instance;
}
async function VisibilityState({ LauncherName, DisplayName }) {
  try {
    const LauncherData = JSON.parse(
      await invoke("read_file", {
        filePath: (await path.appDir()) + "LauncherData.json",
      })
    );
    if (LauncherData.trayMinLaunch === true) {
      if ((await Window.appWindow.isVisible()) === true) {
        Window.appWindow.hide();
        if (timestamp === null) timestamp = Date.now();

        try {
          await invoke(`set_activity`, {
            state: `Launcher: ${LauncherName}`,
            details: DisplayName,
            largeImage: LauncherName.toLowerCase(),
            largeText: "Lazap",
            smallImage: "lazap_icon",
            smallText: "Lazap",
            timestamp: timestamp === null ? Date.now() : timestamp,
          });
        } catch (error) {
          console.error(error);
        }
      } else {
        Window.appWindow.show();
        timestamp = Date.now();

        const { state, details, largeImage, largeText, smallImage, smallText } =
          selectOption(currentRpc);
        try {
          await invoke(`set_activity`, {
            state,
            details,
            largeImage,
            largeText,
            smallImage,
            smallText,
            timestamp: timestamp === null ? Date.now() : timestamp,
          });
        } catch (error) {
          console.error(error);
        }
      }
    }
  } catch (e) {
    return console.error(e);
  }
}

async function setGames(games, source) {
  const appDirPath = await path.appDir();
  const GAMES_DATA_BASE_PATH = appDirPath + "cache/games/data.json";
  const data = JSON.parse(
    await invoke("read_file", {
      filePath: GAMES_DATA_BASE_PATH,
    }).catch(() => "[]")
  );

  data.forEach((d) => {
    Object.keys(d).forEach((x) => {
      if ([undefined, null].includes(d[x])) delete d[x];
    });
  });
  games.forEach((d) => {
    Object.keys(d).forEach((y) => {
      if ([undefined, null].includes(d[y])) delete d[y];
    });
  });

  if (source === "add-launch") {
    await invoke("write_file", {
      filePath: GAMES_DATA_BASE_PATH,
      fileContent: JSON.stringify(games),
    });
  } else if (source === "toggle-favourite") {
    await invoke("write_file", {
      filePath: GAMES_DATA_BASE_PATH,
      fileContent: JSON.stringify(games),
    });
  } else if (source === "all-games") {
    if (data.length > 0) {
      for (let i = 0; i < games.length; i++) {
        const game = data.find(
          (x) =>
            x.GameID === games[i].GameID &&
            x.LauncherName === games[i].LauncherName
        );
        if (!game) {
          data.push(games[i]);
        } else if (
          Object.keys(games[i]).length <
          Object.keys(
            data.find(
              (x) =>
                x.GameID === games[i].GameID &&
                x.LauncherName === games[i].LauncherName
            )
          ).length
        ) {
          let obj = {
            ...data.find(
              (x) =>
                x.GameID === games[i].GameID &&
                x.LauncherName === games[i].LauncherName
            ),
          };
          Object.keys(
            data.find(
              (x) =>
                x.GameID === games[i].GameID &&
                x.LauncherName === games[i].LauncherName
            )
          )
            .filter((x) => !Object.keys(games[i]).includes(x))
            .forEach(
              (x) =>
                (obj[x] = data.find(
                  (x) =>
                    x.GameID === games[i].GameID &&
                    x.LauncherName === games[i].LauncherName
                )[x])
            );
          data.splice(
            data.findIndex(
              (x) =>
                x.GameID === games[i].GameID &&
                x.LauncherName === games[i].LauncherName
            ),
            1,
            obj
          );
        } else if (
          Object.keys(games[i]).length >
          Object.keys(
            data.find(
              (x) =>
                x.GameID === games[i].GameID &&
                x.LauncherName === games[i].LauncherName
            )
          ).length
        ) {
          data[
            data.findIndex(
              (x) =>
                x.GameID === games[i].GameID &&
                x.LauncherName === games[i].LauncherName
            )
          ] = games[i];
        }
      }
    } else {
      return await invoke("write_file", {
        filePath: GAMES_DATA_BASE_PATH,
        fileContent: JSON.stringify(games),
      });
    }
    await invoke("write_file", {
      filePath: GAMES_DATA_BASE_PATH,
      fileContent: JSON.stringify(data),
    });
  }
}

export default {
  name: "find-games",
  methods: {
    async createGameElement(game, id, list, prev) {
      if (game.LauncherName === "CustomGame" && !prev) {
        prev = __CACHE__;
        prev.push(game);
        const filtered = (
          await this.filterAndSort(prev, "allGamesList", list)
        ).reverse();
        prev =
          filtered[
            filtered.findIndex((x) =>
              Object.keys(game).every((y) => game[y] === x[y])
            ) - 1
          ];
      }
      list = document.getElementById(id);
      const gameElement = Elements.getGameElement(game, id);
      if (
        prev &&
        !list.children.namedItem(
          `game-div-${game.DisplayName.replaceAll(" ", "_")}`
        )
      ) {
        list.children
          .namedItem(`game-div-${prev.DisplayName.replaceAll(" ", "_")}`)
          .insertAdjacentElement("beforebegin", gameElement);
      } else if (
        list.children.namedItem(
          `game-div-${game.DisplayName.replaceAll(" ", "_")}`
        )
      ) {
        return;
      } else if (!prev) {
        list.appendChild(gameElement);
      }

      const gameBanner = await Elements.getGameBannerElement(game);
      gameElement.appendChild(gameBanner);
      // eslint-disable-next-line no-self-assign
      game.Banner = game.Banner;

      gameBanner.addEventListener("click", () => {
        this.handleLaunch(game);
      });

      if (id.startsWith("recent") && id.includes("Main")) return game;

      const { enableLauncherIcons } = JSON.parse(
        await invoke("read_file", {
          filePath: (await path.appDir()) + "LauncherData.json",
        })
      );
      if (enableLauncherIcons)
        gameElement.prepend(Elements.getLauncherIconElement(game.LauncherName));

      const gameBottom = document.createElement("div");
      gameBottom.classList.add("gamebox-bottom");
      gameElement.appendChild(gameBottom);

      const gameText = Elements.getGameDisplayElement(game);
      gameBottom.appendChild(gameText);

      const starIcon = await this.getStarElement(game, gameElement);
      gameBottom.appendChild(starIcon);

      const gameMenu = await Elements.getMenuElement(game.DisplayName);
      gameBottom.appendChild(gameMenu);

      return game;
    },
    async filterAndSort(games, type, list, stored) {
      list = list ?? document.getElementById(type);

      // Check if the list already has the games loaded and prevent further execution
      if (list.children.length === games.length && list.children.length !== 0) {
        if (
          games.every(
            (x, i) =>
              list.children.item(i).id ===
              `game-div-${x.DisplayName.replaceAll(" ", "_")}`
          )
        )
          return [];
      }

      // Filter out new games and delete old games
      games = games.filter(
        (x) =>
          !require("./others/blacklist.json")[0].includes(
            x.GameID
          ) /* && !list.children.namedItem(`game-div-${x.DisplayName.replaceAll(' ', '_')}`)*/
      );
      for (let i = 0; i < list.length; i++) {
        if (
          !games
            .map((x) => x.GameID.replaceAll(" ", "_"))
            .includes(list.children[i].id.slice(9))
        ) {
          list.removeChild(list.children[i]);
          continue;
        }
      }

      if (type === "allGamesList") {
        games = games
          .map((x) => x.DisplayName)
          .sort()
          .map((x) => games[games.findIndex((y) => y.DisplayName === x)]);
      } else if (
        ["recentGamesListMainPage", "recentGamesList"].includes(type)
      ) {
        let final = [];

        for (let i = 0; i < games.length; i++) {
          const game =
            stored?.find(
              (x) =>
                x.GameID === games[i].GameID &&
                x.LauncherName === games[i].LauncherName
            ) ?? (await this.getGames(games[i].GameID, games[i].LauncherName));
          if (
            typeof game?.LastLaunch === "number" &&
            typeof game?.Launches === "number" &&
            game?.Launches > 0
          )
            final.push(game);
        }
        final
          .sort((a, b) => {
            return b.Launches - a.Launches;
          })
          .sort((a, b) => {
            if (b.Launches - a.Launches === 0) {
              return b.LastLaunch - a.LastLaunch;
            } else {
              return 0;
            }
          });
        games = final.slice(
          0,
          type.includes("MainPage") ? 5 - list.children.length : final.length
        );
      } else if (type === "favGamesList") {
        let final = [];
        for (let i = 0; i < games.length; i++) {
          const game =
            stored?.find(
              (x) =>
                x.GameID === games[i].GameID &&
                x.LauncherName === games[i].LauncherName
            ) ?? (await this.getGames(games[i].GameID, games[i].LauncherName));
          if (typeof game?.Favourite === "boolean" && game.Favourite === true)
            final.push(game);
        }
        games = final
          .map((x) => x.DisplayName)
          .sort()
          .map((x) => final[final.findIndex((y) => y.DisplayName === x)]);
      } else {
        return [];
      }
      __CACHE__ = games;
      return games;
    },
    async loadGames(id, data, stored) {
      const games = data ?? (await this.getInstalledGames());
      const list = document.getElementById(id);

      (await this.filterAndSort(games, id, list, stored))
        .reverse()
        .forEach(async (game, i, arr) =>
          this.createGameElement(game, id, list, arr[i - 1])
        );
      if (games.length > 0 && id === "allGamesList") {
        setGames(games, "all-games");
      }

      if (!data) {
        this.loadGames(id, [
          ...games,
          ...(await this.getInstalledGames(["XboxGames.js"])),
        ]);
      }

      return games;
    },
    async getInstalledGames(
      launchers = [
        "CustomGames",
        "EpicGames.js",
        "Lutris.js",
        "Minecraft.js",
        "RiotGames.js",
        "RockstarGames.js",
        "Steam.js",
        "Uplay.js",
      ]
    ) {
      if (loads >= 1) {
        launchers = launchers.filter((x) => x !== "XboxGames.js");
      }
      if (loads >= 2) {
        return [];
      }
      loads++;

      // Fetch all games
      const games = (
        await Promise.all(
          launchers
            .filter((x) => x.endsWith(".js"))
            .map((x) => require(`./launchers/${x}`)?.getInstalledGames())
        )
      ).flat();

      loads--;
      if (launchers.includes("CustomGames")) {
        try {
          let readDataJSON = JSON.parse(
            await invoke("read_file", {
              filePath: (await path.appDir()) + "cache/games/data.json",
            })
          );
          readDataJSON
            .filter((a) => a.LauncherName == "CustomGame")
            .forEach((x) => {
              games.push(x);
            });
          return games;
        } catch (e) {
          return games;
        }
      } else {
        return games;
      }
    },
    async getGames(GameID, LauncherName) {
      const data = JSON.parse(
        await invoke("read_file", {
          filePath: (await path.appDir()) + "cache/games/data.json",
        })
      );

      if (GameID && LauncherName) {
        return data.find(
          (x) => x.GameID === GameID && x.LauncherName === LauncherName
        );
      } else {
        return data;
      }
    },
    async toggleFavourite(GameID, LauncherName) {
      const data = await this.getGames();
      if (!data) return;
      const game = data.find(
        (x) => x.GameID === GameID && x.LauncherName === LauncherName
      );
      game.Favourite = !game.Favourite;
      if (
        game.Favourite === false &&
        document
          .getElementById("favGamesList")
          .children.namedItem(
            `game-div-${game.DisplayName.replaceAll(" ", "_")}`
          )
      ) {
        const element = document
          .getElementById("favGamesList")
          .children.namedItem(
            `game-div-${game.DisplayName.replaceAll(" ", "_")}`
          );
        element.classList.add("fadeOutUpNoDelay");
        setTimeout(
          () => document.getElementById("favGamesList").removeChild(element),
          500
        );
      }

      setGames(data, "toggle-favourite");

      if (game.Favourite === false) {
        return "empty";
      } else {
        return "solid";
      }
    },
    async getStarElement(game) {
      const starIcon = document.createElement("div");
      starIcon.classList.add("star");
      starIcon.id = "star";

      const isFavourite = JSON.parse(
        await invoke("read_file", {
          filePath: (await path.appDir()) + "cache/games/data.json",
        })
      ).find(
        (y) =>
          y.GameID === game.GameID &&
          y.LauncherName === game.LauncherName &&
          y.Favourite
      );
      if (isFavourite) {
        starIcon.classList.add("star-fill");
        starIcon.style.filter =
          "invert(77%) sepia(68%) saturate(616%) hue-rotate(358deg) brightness(100%) contrast(104%)";
      } else {
        starIcon.classList.remove("star-fill");
        starIcon.style.filter =
          "invert(100%) sepia(0%) saturate(1489%) hue-rotate(35deg) brightness(116%) contrast(100%)";
      }

      starIcon.addEventListener("click", async () => {
        const solidOrEmpty = await this.toggleFavourite(
          game.GameID,
          game.LauncherName
        );
        starIcon.style.filter =
          solidOrEmpty === "solid"
            ? "invert(77%) sepia(68%) saturate(616%) hue-rotate(358deg) brightness(100%) contrast(104%)"
            : "invert(100%) sepia(0%) saturate(1489%) hue-rotate(35deg) brightness(116%) contrast(100%)";

        if (solidOrEmpty === "solid") {
          starIcon.classList.add("star-fill");
          starIcon.classList.add("shake");
          setTimeout(() => {
            starIcon.classList.remove("shake");
          }, 500);
        } else {
          starIcon.classList.remove("star-fill");
        }
      });

      return starIcon;
    },
    async addLaunch(GameID, LauncherName) {
      const data = await this.getGames();
      if (!data) return;
      const game = data.find(
        (x) => x.GameID === GameID && x.LauncherName === LauncherName
      );
      game.LastLaunch = Date.now();
      game.Launches = typeof game.Launches === "number" ? game.Launches + 1 : 1;
      setGames(data, "add-launch");
      if (
        !document
          .getElementById("recentGamesList")
          .children.namedItem(
            `game-div-${game.DisplayName.replaceAll(" ", "_")}`
          )
      ) {
        // eslint-disable-next-line no-undef
        recentGamesList.replaceChildren([]);
        this.loadGames("recentGamesList", null, data);
        if (
          document.getElementById("recentGamesListMainPage").children.length < 5
        ) {
          // eslint-disable-next-line no-undef
          recentGamesListMainPage.replaceChildren([]);
          this.loadGames("recentGamesListMainPage", null, data);
        }
      }
    },
    async handleLaunch(game) {
      let res;
      if ((await os.platform()) === "win32") {
        switch (game.LauncherName) {
          case "EpicGames": {
            res = createProcess_windows(
              `/C start com.epicgames.launcher://apps/${encodeURIComponent(
                game.LaunchID
              )}?action=launch&silent=true`,
              game
            );
            break;
          }
          case "Steam": {
            res = createProcess_windows(
              `/C start steam://rungameid/${game.GameID}`,
              game
            );
            break;
          }
          case "Uplay": {
            res = createProcess_windows(
              `/C start uplay://launch/${game.GameID}/0`,
              game
            );
            break;
          }
          case "Minecraft": {
            res = createProcess_windows(
              `/C powershell start "${game.Location}\\${game.Executable}"`,
              game
            );
            break;
          }
          case "Lunar": {
            res = createProcess_windows(
              `/C powershell start "${game.Location}\\${game.Executable}"`,
              game
            );
            break;
          }
          default: {
            console.log(game.Location, game.Executable);
            res = createProcess_windows(
              `/C powershell start "${game.Location}\\${game.Executable}"`,
              game
            );
            break;
          }
        }
      } else if ((await os.platform()) === "linux") {
        switch (game.LauncherName) {
          case "Steam": {
            res = createProcess_linux(
              "steam",
              `steam://rungameid/${game.GameID} -silent`,
              game
            );
            break;
          }
          case "Minecraft": {
            res = createProcess_linux("minecraft-launcher", "", game);
            break;
          }
          case "Lunar": {
            res = createProcess_linux("lunarclient", "", game);
            break;
          }
          case "Lutris": {
            res = createProcess_linux(
              "lutris",
              `lutris:rungameid/${game.LaunchID}`,
              game
            );
            break;
          }
          default: {
            res = createProcess_linux(
              `"${game.Location}/${game.Executable}"`,
              game.Args,
              game
            );
            break;
          }
        }
      }

      if (res === "RUNNING_ALREADY") return;

      this.addLaunch(game.GameID, game.LauncherName);
    },
  },
};
</script>
