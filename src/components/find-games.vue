<script>
// TODO benchmark everything to see what's causing so much delay DONE
// Classes
class GameElement {
  constructor(data, listID, jsondata, settings, bannerdirarr) {
    this.data = data;
    this.jsondata = jsondata; // Not to be used in events, only during initialisation(getHTMLElement)
    this.settings = settings; // Not to be used in events, only during initialisation(getHTMLElement)
    this.bannerdirarr = bannerdirarr;
    this.listID = listID;
    this.process = null;
  }

  async getHTMLElement() {
    const gameElement = this.getElement();

    if (this.listID.startsWith("recent") && this.listID.includes("Main")) {
      await this.getBannerElementv2(gameElement);
      return gameElement;
    }

    const banner = await this.getBannerElement();
    gameElement.appendChild(banner);
    // eslint-disable-next-line no-self-assign
    this.data.Banner = this.data.Banner;
    const { enableLauncherIcons } = this.settings;
    if (enableLauncherIcons) gameElement.prepend(this.getLauncherIconElement());

    const gameBottom = document.createElement("div");
    gameBottom.classList.add("gamebox-bottom");
    gameElement.appendChild(gameBottom);
    gameBottom.appendChild(this.getTextElement());
    gameBottom.appendChild(await this.getStarElement());
    gameBottom.appendChild(this.getMenuElement());

    return gameElement;
  }

  getElement() {
    const element = document.createElement("div");
    element.id = `game-div-${this.data.display_name.replaceAll(" ", "_")}`;
    element.className +=
      this.listID.startsWith("recent") && this.listID.includes("Main")
        ? "mainPageGamebox"
        : "gamebox";
    element.style.display = "table";
    return element;
  }

  async getBannerElement() {
    const element = document.createElement("img");

    const image = require("../assets/img/default-game-banner.png");
    element.setAttribute("src", image);

    element.classList.add("game_banner_img");

    let banner;
    let block = false;
    element.addEventListener("error", () => {
      block = true;
      if (this.data.launcher_name !== "CustomGame" && banner)
        console.warn(
          `${this.data.display_name}'s banner did not load successfully`
        );
      const image = require("../assets/img/default-game-banner.png");
      element.setAttribute("src", image);
    });
    element.addEventListener("load", async () => {
      if (block) return;
      // Load banner from getBanner() after default banner has loaded
      banner = await this.getBanner();
      element.setAttribute("src", banner);
      element.style.content = "none";
      this.cacheBanner(banner);
      block = true;
    });
    element.addEventListener("click", async () => {
      this.handleLaunch();
      storage.addLaunch(this.data.game_id, this.data.launcher_name);
    });

    return element;
  }

  async getBannerElementv2(element) {
    const image = require("../assets/img/default-game-banner.png");
    element.style.background = `url(${image})`;
    element.style.backgroundSize = "cover";

    let banner = await this.getBanner();
    element.style.background = `url(${banner})`;
    element.style.backgroundSize = "cover";

    this.cacheBanner(banner);

    element.addEventListener("click", () => {
      this.handleLaunch();
      storage.addLaunch(this.data.game_id, this.data.launcher_name);
    });

    return element;
  }

  getTextElement() {
    const element = document.createElement("span");

    if (this.data.display_name.length > 17) {
      element.innerText = `${this.data.display_name.slice(0, 17)}...`;
    } else {
      element.innerText = this.data.display_name;
    }
    return element;
  }

  getMenuElement() {
    const element = document.createElement("div");
    element.classList.add("menu");
    element.id = "menu";

    element.addEventListener("click", () => {
      const gamemenu = document.getElementById("gameMenu");
      gamemenu.style.display =
        gamemenu.style.display === "flex" ? "none" : "flex";
      document.getElementById("gameMenuTitle").innerHTML =
        this.data.display_name;
    });

    return element;
  }

  async getStarElement() {
    const element = document.createElement("div");
    element.classList.add("star");
    element.id = "star";

    // Check whether the game is starred or not
    const isFav = this.jsondata?.favourite || false;
    if (isFav) {
      element.classList.add("star-fill");
      element.style.filter =
        "invert(77%) sepia(68%) saturate(616%) hue-rotate(358deg) brightness(100%) contrast(104%)";
    } else {
      element.classList.remove("star-fill");
      element.style.filter =
        "invert(100%) sepia(0%) saturate(1489%) hue-rotate(35deg) brightness(116%) contrast(100%)";
    }

    // Handle onclick
    element.addEventListener("click", async () => {
      const isfavourite = await storage.togglefavourite(
        this.data.game_id,
        this.data.launcher_name
      );
      const ele = Array.from(
        document.getElementById(
          `game-div-${this.data.display_name.replaceAll(" ", "_")}`
        ).children
      )
        .find((x) => x.classList.contains("gamebox-bottom"))
        .children.namedItem("star");

      ele.style.filter = isfavourite
        ? "invert(77%) sepia(68%) saturate(616%) hue-rotate(358deg) brightness(100%) contrast(104%)"
        : "invert(100%) sepia(0%) saturate(1489%) hue-rotate(35deg) brightness(116%) contrast(100%)";
      if (isfavourite) {
        ele.classList.add("star-fill");
        ele.classList.add("shake");
        setTimeout(() => ele.classList.remove("shake"), 500);
      } else {
        ele.classList.remove("star-fill");
      }
    });
    return element;
  }

  getLauncherIconElement() {
    const element = document.createElement("img");
    const image = require(`../assets/icons/${this.data.launcher_name}.png`);
    element.setAttribute("src", image);
    element.classList.add("gamebox-icon");

    return element;
  }

  async getBanner() {
    const bannersDir = this.bannerdirarr;
    const dispsha256 = await invoke("sha256", {
      content: this.data.display_name
        .replaceAll(" ", "_")
        .replace(/[\u{0080}-\u{FFFF}/]/gu, ""),
    });

    if (bannersDir.includes(`${dispsha256}.png`)) {
      return await tauri.convertFileSrc(
        `${storage.bannersDir}/${dispsha256}.png`
      );
    } else {
      switch (this.data.launcher_name) {
        case "Steam": {
          return `https://cdn.akamai.steamstatic.com/steam/apps/${this.data.game_id}/library_600x900.jpg`;
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
          return "https://pbs.twimg.com/profile_images/1608698913476812801/uLTLhANK_400x400.jpg";
        }
        case "Lutris": {
          if (
            this.data.display_name.replaceAll(" ", "_") === "Epic_Games_Store"
          )
            return "https://pcper.com/wp-content/uploads/2021/02/epic-games-store.png";
          if (
            this.data.display_name.replaceAll(" ", "_") ===
            "Rockstar_Games_Launcher"
          )
            return "https://cdn.player.one/sites/player.one/files/2019/08/26/rockstar-games.png";
          return;
        }
        case "XboxGames": {
          return this.data.Banner;
        }
        case "Osu": {
          return "https://cdn2.steamgriddb.com/file/sgdb-cdn/grid/a5d7420f9fdc41087377b4d58c5fe94b.png";
        }
        case "EpicGames": {
          const fetchEpicGame = await http.fetch(
            `https://api.rawg.io/api/games?key=f8854c401fed44b89f4e1e4faa56ccc8&search=${this.data.display_name.replaceAll(
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
            `https://api.rawg.io/api/games?key=f8854c401fed44b89f4e1e4faa56ccc8&search=${this.data.display_name.replaceAll(
              " ",
              "-"
            )}&search_exact&search_precise`,
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
          return `https://media-rockstargames-com.akamaized.net/rockstargames-newsite/img/global/games/fob/640/${this.data.bannerId}.jpg`;
        }
      }
    }
  }

  async cacheBanner(banner) {
    if (this.data.launcher_name === "CustomGame" || !banner) return;
    const bannersDir = await storage.readBannersDir();
    const dispsha256 = await invoke("sha256", {
      content: this.data.display_name
        .replaceAll(" ", "_")
        .replace(/[\u{0080}-\u{FFFF}/]/gu, ""),
    });

    if (bannersDir.includes(`${dispsha256}.png`)) {
      return await tauri.convertFileSrc(
        `${storage.bannersDir}/${dispsha256}.png`
      );
    } else {
      http
        .fetch(banner, {
          method: "GET",
          headers: {
            "Content-Type": "text/plain; charset=utf-8",
          },
          responseType: 3,
        })
        .then(async (response) => {
          if (response.status === 404 && this.data.launcher_name === "Lutris")
            return;

          await invoke("write_binary_file", {
            filePath: `${storage.bannersDir}/${dispsha256}.png`,
            fileContent: response.data,
          });
          const banner = document.getElementById(
            `game-div-${this.data.display_name.replaceAll(" ", "_")}`
          )?.firstElementChild;
          banner?.setAttribute(
            "src",
            tauri.convertFileSrc(`${storage.bannersDir}/${dispsha256}.png`)
          );
          banner.style = "content: none;";
          banner.addEventListener("error", () => (banner.style = ""));
        })
        .catch((e) => console.error(e));
    }
  }

  async handleLaunch() {
    let res;
    if (os === "win32") {
      switch (this.data.launcher_name) {
        case "EpicGames": {
          res = this.createProcess(
            `/C start com.epicgames.launcher://apps/${encodeURIComponent(
              this.data.launch_id
            )}?action=launch&silent=true`
          );
          break;
        }
        case "Steam": {
          res = this.createProcess(
            `/C start steam://rungame_id/${this.data.game_id}`
          );
          break;
        }
        case "Uplay": {
          res = this.createProcess(
            `/C start uplay://launch/${this.data.game_id}/0`
          );
          break;
        }
        case "Minecraft": {
          res = this.createProcess(
            `/C powershell start "${this.data.location}\\${this.data.executable}"`
          );
          break;
        }
        case "Lunar": {
          res = this.createProcess(
            `/C powershell start "${this.data.location}\\${this.data.executable}"`
          );
          break;
        }
        default: {
          res = this.createProcess(
            `/C powershell start "${this.data.location}\\${this.data.executable}"`
          );
          break;
        }
      }
    } else if (os === "linux") {
      switch (this.data.launcher_name) {
        case "Steam": {
          res = this.createProcess(
            "steam",
            `steam://rungame_id/${this.data.game_id} -silent`
          );
          break;
        }
        case "Minecraft": {
          res = this.createProcess("minecraft-launcher");
          break;
        }
        case "Lunar": {
          res = this.createProcess("lunarclient");
          break;
        }
        case "Lutris": {
          res = this.createProcess(
            "lutris",
            `lutris:rungame_id/${this.data.launch_id}`
          );
          break;
        }
        default: {
          res = this.createProcess(
            `"${this.data.location}/${this.data.executable}"`,
            this.data.args,
          );
          break;
        }
      }
    }
    return res;
  }

  async createProcess(exec, args = "") {
    // TODO: add back "ALREADY_RUNNING"
    VisibilityState({
      launcher_name: this.data.launcher_name,
      display_name: this.data.display_name,
    });
    invoke("launch_game", { exec, args }).then(() => {
      VisibilityState({
        launcher_name: this.data.launcher_name,
        display_name: this.data.display_name,
      });
    });

    async function VisibilityState() {
      try {
        const { tray_min_launch } = await storage.getSettings();
        if (tray_min_launch === true) {
          if ((await Window.appWindow.isVisible()) === true) {
            Window.appWindow.hide();
            if (!timestamp) timestamp = Date.now();
            try {
              await invoke(`set_rpc_activity`, {
                state: `Launcher: ${this.data.launcher_name}`,
                details: this.data.display_name,
                largeImage: this.data.launcher_name.toLowerCase(),
                largeText: "Lazap",
                smallImage: "lazap_icon",
                smallText: "Lazap",
                timestamp: timestamp,
              });
            } catch (error) {
              console.error(error);
            }
          } else {
            Window.appWindow.show();
            const rpc = require("./modules/rpcOptions");
            const { details, largeText, smallImage, smallText } =
              rpc.selectOption(rpc.currentRpc);
            if (timestamp === null) timestamp = Date.now();
            try {
              await invoke(`set_rpc_activity`, {
                details,
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
  }
}

class Storage {
  constructor() {
    (async () => {
      this.appDataDir = await window.__TAURI__.path.appDir();
      this.gamesDataJSON = `${this.appDataDir}/cache/games/data.json`;
      this.bannersDir = `${this.appDataDir}/cache/games/banners`;
      this.settingsJSON = `${this.appDataDir}/LauncherData.json`;
    })();
  }
  async setGamesData(games, source) {
    const data = await this.getGamesData();

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

    try {
      if (source === "getInstalledGames") {
        if (data.length > 0) {
          if (data.length != games.length) {
            await invoke("write_file", {
              filePath: this.gamesDataJSON,
              fileContent: JSON.stringify(
                data.filter((x) =>
                  games.find(
                    (y) =>
                      y.launcher_name === x.launcher_name &&
                      y.game_id === x.game_id
                  )
                )
              ),
            });
          }
        } else {
          await invoke("write_file", {
            filePath: this.gamesDataJSON,
            fileContent: JSON.stringify(games),
          });
        }
      } else if (["togglefavourite", "addLaunch"].includes(source)) {
        await invoke("write_file", {
          filePath: this.gamesDataJSON,
          fileContent: JSON.stringify(games),
        });
      }
    } catch (e) {
      console.error(e);
    }
  }
  async getGamesData() {
    try {
      return JSON.parse(
        await invoke("read_file", { filePath: this.gamesDataJSON })
      );
    } catch (e) {
      console.error(e);
    }
  }
  async readBannersDir() {
    try {
      return await invoke("read_dir", { dirPath: this.bannersDir });
    } catch (e) {
      console.error(e);
    }
  }
  async togglefavourite(game_id, launcher_name) {
    const data = await this.getGamesData();

    // Invert favourite field of the game and save the data
    const game = data.find(
      (x) => x.game_id === game_id && x.launcher_name === launcher_name
    );
    game.favourite = !game.favourite;
    this.setGamesData(data, "togglefavourite");

    // Check if game exists in favourites game list, if so delete the element
    const list = document.getElementById("favGamesList");
    const element = list.children.namedItem(
      `game-div-${game.display_name.replaceAll(" ", "_")}`
    );
    if (game.favourite === false && element) {
      element.classList.add("fadeOutUpNoDelay");
      setTimeout(() => {
        list.removeChild(element);
        if (document.getElementById("favGamesList").childNodes.length == 0) {
          document.getElementById("favGamesPlaceholder").style.display =
            "block";
        }
      }, 200);
    }

    return game.favourite;
  }
  async getSettings() {
    try {
      return JSON.parse(
        await invoke("read_file", { filePath: this.settingsJSON })
      );
    } catch (e) {
      console.error(e);
    }
  }
  async addLaunch(game_id, launcher_name) {
    const data = await this.getGamesData();
    if (!data) return;
    const game = data.find(
      (x) => x.game_id === game_id && x.launcher_name === launcher_name
    );
    game.lastlaunch = Date.now();
    game.launches = typeof game.launches === "number" ? game.launches + 1 : 1;
    this.setGamesData(data, "addLaunch");
  }
}

// Imports
const invoke = window.__TAURI__.invoke;
const tauri = window.__TAURI__.tauri;
const http = window.__TAURI__.http;

const Window = window.__TAURI__.window;
// Objects
const storage = new Storage();
// Variables
let fetches = 0;
const loads = {};
let lists = ["allGamesList", "recentGamesList", "favGamesList"];
let timestamp;
let os;

export default {
  name: "find-games",
  async mounted() {
    lists = lists.map((x) => document.getElementById(x));
    os = await window.__TAURI__.os.platform();
  },
  methods: {
    // Fetches all installed games in the disk according to the Launchers provided
    async getInstalledGames() {
      // Prevents multiple simultaneous running of the function
      if (fetches > 1) {
        return [];
      }
      fetches++;

      let games = await invoke("fetch_installed_games");
      // Adds Custom Games to the array if requested
      //if (Launchers.includes('CustomGames')) {
      //	const data = await storage.getGamesData();
      //	games.push(...data.filter(x => x.launcher_name === 'CustomGame'));
      //}
        
      // Save all games into JSON
      storage.setGamesData(games, "getInstalledGames");

      fetches--;
      return games;
    },
    // Gets all games installed and then filters out games which are required for the specific list
    async getGames(listID, allgames) {
      const games = allgames || (await this.getInstalledGames());
      if (listID === "recentGamesListMainPage") {
        let data = await storage.getGamesData();
        data = data
          .filter((x) => typeof x.launches === "number")
          .sort((a, b) => b.launches - a.launches)
          .slice(0, 5);

        // Add placeholder elements for the rest of the remaining spaces
        if (document.getElementsByClassName("placeholderGames").length < 1) {
          for (let i = 0; i < 5 - games.length; i++) {
            let element = document.createElement("div");
            element.classList.add("placeholderGames");
            document.getElementById("recentGamesListMainPage").append(element);
          }
        }

        return data;
      } else if (listID === "recentGamesList") {
        let data = await storage.getGamesData();
        data = data
          .filter((x) => typeof x.launches === "number")
          .sort((a, b) => b.launches - a.launches);
        return data;
      } else if (listID === "allGamesList") {
        return games
          .map((x) => x.display_name)
          .sort()
          .map((x) => games.find((y) => y.display_name === x));
      } else if (listID === "favGamesList") {
        let data = await storage.getGamesData();
        data = data
          .filter(
            (x) => typeof x.favourite === "boolean" && x.favourite === true
          )
          .map((x) => x.display_name)
          .sort()
          .map((x) => data.find((y) => y.display_name === x));
        return data;
      }
    },
    async createGameElement(data, listID, jsondata, settings, bannersdirarr) {
      const element = await new GameElement(
        data,
        listID,
        jsondata,
        settings,
        bannersdirarr
      ).getHTMLElement();
      return element;
    },
    async loadGames(listID, games) {
      const gamesdata = await storage.getGamesData();
      const bannerdirarr = await storage.readBannersDir();
      const settings = await storage.getSettings();
      const allgames = await this.getGames(listID, games);
      const list = document.getElementById(listID);
      const elements = [];

      if (loads[listID] === true) {
        return;
      } else {
        loads[listID] = true;
      }

      for (let i = 0; i < allgames.length; i++) {
        const element =
          list.children.namedItem(
            `game-div-${allgames[i].display_name.replaceAll(" ", "_")}`
          ) ??
          (await new GameElement(
            allgames[i],
            listID,
            gamesdata.find(
              (x) =>
                x.game_id === allgames[i].game_id &&
                x.launcher_name === allgames[i].launcher_name
            ),
            settings,
            bannerdirarr
          ).getHTMLElement());
        elements.push(element);
      }

      if (listID === "recentGamesListMainPage" && allgames.length < 5) {
        for (let i = 0; i < 5 - allgames.length; i++) {
          let element = document.createElement("div");
          element.classList.add("placeholderGames");
          elements.push(element);
        }
      }

      list.replaceChildren(...elements);
      delete loads[listID];
    },
  },
};
</script>
