<script>
import image from "../assets/img/default-game-banner.png";

/*
  isTyping is very important as it is a fix for a bug where spam clicking makes the GameMenu title weird.
  It is intended to be a global variable, as keeping it inside getMenuElement() resets it, which is not intended to happen.
*/
let isTyping = false;
const typeSpeed = 50;

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
      element.setAttribute("src", image);
    });
    element.addEventListener("load", async () => {
      if (block) return;
      // Load banner from getBanner() after default banner has loaded
      if (this.data.banner_path) {
        banner = await tauri.convertFileSrc(await this.data.banner_path);
      }
      element.setAttribute("src", banner);
      element.style.content = "none";
      block = true;
    });
    element.addEventListener("click", async () => {
      await invoke("handle_launch", { data: this.data });
      storage.addLaunch(this.data.game_id, this.data.launcher_name);
    });

    return element;
  }

  async getBannerElementv2(element) {
    element.style.background = `url(${image})`;
    if (this.data.banner_path) {
      let banner = await tauri.convertFileSrc(await this.data.banner_path);
      element.style.background = `url(${banner}) no-repeat`;
    }
    element.style.backgroundSize = "cover";

    element.addEventListener("click", async () => {
      await invoke("handle_launch", { data: this.data });
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

    const gamemenu = document.getElementById("gameMenu");

    element.addEventListener("click", () => {
      //makes sure the user can't spam click.
      if (!isTyping) {
        isTyping = true;
        gamemenu.style.display = "flex";
        const gameMenuTitle = document.getElementById("gameMenuTitle");
        gameMenuTitle.innerHTML = " ";
        const display_name = this.data.display_name;
        for (let i = 0; i < display_name.length; i++) {
          setTimeout(function () {
            gameMenuTitle.innerHTML += display_name.charAt(i);
            if (i === display_name.length - 1) {
              isTyping = false; // Reset the flag when typing is finished
            }
          }, typeSpeed * i);
        }
      }
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
    element.setAttribute(
      "src",
      `src/assets/icons/${this.data.launcher_name}.png`
    );
    element.classList.add("gamebox-icon");

    return element;
  }
}

class Storage {
  constructor() {
    (async () => {
      this.appDataDir = await window.__TAURI__.path.appDir();
      this.gamesDataJSON = `${this.appDataDir}cache/games/data.json`;
      this.bannersDir = `${this.appDataDir}cache/games/banners`;
      this.settingsJSON = `${this.appDataDir}LauncherData.json`;
    })();
  }
  async setGamesData(games, source) {
    const file_data = await this.getGamesData();
    file_data.forEach((d) => {
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
        if (file_data.length > 0) {
          if (file_data.length != games.length) {
            const newData = games.filter((game) => {
              return !file_data.some(
                (existingGame) =>
                  existingGame.launcher_id === game.launcher_id &&
                  existingGame.display_name === game.display_name
              );
            });

            const removedData = file_data.filter((existingGame) => {
              return !games.some(
                (game) =>
                  existingGame.launcher_id === game.launcher_id &&
                  existingGame.display_name === game.display_name
              );
            });

            const mergedData = file_data
              .filter((existingGame) => !removedData.includes(existingGame))
              .concat(newData);

            await invoke("write_file", {
              filePath: this.gamesDataJSON,
              fileContent: JSON.stringify(mergedData),
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

      // Save all games into JSON
      storage.setGamesData(games, "getInstalledGames");

      fetches--;
      return games;
    },
    // Gets all games installed and then filters out games which are required for the specific list
    async getGames(listID, allgames) {
      const games = allgames || (await this.getInstalledGames());
      if (listID === "recentGamesListMainPage") {
        let recentGamesList = document.getElementById(
          "recentGamesListMainPage"
        );
        let childCount = recentGamesList.childElementCount;
        for (let i = 0; i < childCount - 5; i++) {
          recentGamesList.removeChild(recentGamesList.firstChild);
        }

        let data = await storage.getGamesData();
        data = data
          .filter((x) => typeof x.launches === "number" && x.launches != 0)
          .sort((a, b) => b.launches - a.launches)
          .slice(0, 5);

        return data;
      } else if (listID === "recentGamesList") {
        let data = await storage.getGamesData();
        data = data
          .filter((x) => typeof x.launches === "number" && x.launches != 0)
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
