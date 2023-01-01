<template>
  <div class="settings-background" id="settings-backblur"></div>
  <div class="settings fadeInDown" id="settings-popup">
    <div>
      <h1>Settings</h1>
      <div class="choser" id="choser">
        <button id="generalbtn">General</button>
        <button id="appearancebtn">Appearance</button>
        <button id="winerunnersbtn">Wine Runners</button>
      </div>
      <div class="general-settings" id="general-settings">
        <div>
          <p>Discord Rich Presence</p>
          <label class="switch">
            <input type="checkbox" id="setting-enableRPC" />
            <div>
              <span></span>
            </div>
          </label>
        </div>

        <div>
          <p>Launch on Startup</p>
          <label class="switch">
            <input
              type="checkbox"
              id="setting-launchOnStartup"
              disabled="readonly"
            />
            <div>
              <span></span>
            </div>
          </label>
        </div>

        <div>
          <p>Skip Login Menu</p>
          <label class="switch">
            <input type="checkbox" id="setting-skipLogin" disabled="readonly" />
            <div>
              <span></span>
            </div>
          </label>
        </div>

        <div>
          <p>Minimize to Tray on Launch</p>
          <label class="switch">
            <input type="checkbox" id="setting-trayMinLaunch" />
            <div>
              <span></span>
            </div>
          </label>
        </div>

        <div>
          <p>Minimize to Tray on Quit</p>
          <label class="switch">
            <input type="checkbox" id="setting-trayMinQuit" />
            <div>
              <span></span>
            </div>
          </label>
        </div>

        <div>
          <p>Check for Updates</p>
          <label class="switch">
            <input
              type="checkbox"
              id="setting-checkForUpdates"
              disabled="readonly"
            />
            <div>
              <span></span>
            </div>
          </label>
        </div>
      </div>
      <div class="appearance-settings" id="appearance-settings">
        <div class="subSetting">
          <p>Accent Color</p>
          <div class="btnInput">
            <label class="color">
              <input type="color" id="setting-accentColor" />
            </label>
            <img class="repeatButton" src="./assets/icons/reset.png" />
          </div>
        </div>
      </div>

      <div class="settings-footer">v0.6.1 (Tauri Release)</div>
    </div>
  </div>

  <div class="main-loading-overlay" id="main-loading-overlay">
    <div class="spinner-content">
      <img src="./assets/svg/spinner.svg" class="loading-spinner" />
      <p id="spinnerLoading">Loading...</p>
    </div>
  </div>

  <div data-tauri-drag-region class="titlebar">
    <div
      data-tauri-drag-region
      style="justify-content: space-between"
      class="titlebar-icons"
    >
      <div class="titlebar-options">
        <img
          class="titlebar-settings"
          src="./assets/svg/settings.svg"
          id="settings-btn"
        />
        <img
          @click="changeComponent('Login')"
          class="titlebar-account"
          src="./assets/svg/account.svg"
          id="account-btn"
        />
        
        <img class="titlebar-rpc" src="./assets/svg/discord.svg" id="rpcbtn" />
        <span id="rpc"></span>
      </div>
      <div style="margin-top: 0px" class="titlebar-icons">
        <div @click="min_window" class="titlebar-min mx-1"></div>
        <div @click="max_window" class="titlebar-max mx-1"></div>
        <div @click="close_window" class="titlebar-exit mx-1"></div>
      </div>
    </div>
  </div>

  <div class="bg" id="bg">
    <div class="outline"></div>
    <div class="homebox" id="home">
      <div class="children fadeInUp">
        <img
          class="head-pic"
          src="./assets/img/main-banner.png"
          id="head-pic"
        />
      </div>
      <div class="children fadeInDown">
        <div class="jump-back">
          <p>Recently Launched</p>
          <div
            id="recentGamesListMainPage"
            class="fadeInDown mainPageGamesList"
          ></div>
        </div>
      </div>
      <div class="children fadeInLeft">
        <div class="rightbar">
          <p>System Specification</p>
          <div id="sysInfo" class="sysInfo"></div>
        </div>
      </div>
    </div>

    <div class="secondorybox" id="recent">
      <p>Recently Played</p>
      <div id="recentGamesList" class="fadeInDown gamesList"></div>
    </div>

    <div class="secondorybox" id="games">
      <div class="addGamePopUp" id="addGamePopUp">
        <div class="mainSection fadeInDown">
          <div class="section">
            <div class="title">Game Name</div>
            <input
              maxlength="24"
              type="text"
              class="inputGameName"
              id="inputGameName"
            />
          </div>
          <div class="section">
            <button class="addGameLocation" id="addGameLocation">
              Locate Game
            </button>
          </div>
          <div class="section">
            <button class="addGameFinalBtn" id="addGameFinalBtn">
              Add Game
            </button>
          </div>
        </div>
        <div class="addGameBannerSection fadeInUp">
          <label for="addGameCustomBanner"></label>
          <input
            class="banner"
            id="addGameCustomBanner"
            type="file"
            accept="image/png"
            @change="(event) => loadCustomBanner(event)"
          />
          <p id="addGameCustomBannerTxt">Hover & Click to Select Banner</p>
          <div
            id="addGameCustomBannerOutput"
            class="addGameCustomBannerOutput"
          ></div>
        </div>
      </div>

      <div id="game-loading-overlay" class="game-loading-overlay">
        <div class="spinner-content">
          <img
            src="./assets/svg/spinner.svg"
            alt="Loading..."
            class="loading-spinner"
          />
        </div>
      </div>
      <p>All Games</p>
      <svg
        id="addGameBtn"
        class="addGameBtn"
        version="1.1"
        xmlns="http://www.w3.org/2000/svg"
        xmlns:xlink="http://www.w3.org/1999/xlink"
        x="0px"
        y="0px"
        width="459.325px"
        height="459.325px"
        viewBox="0 0 459.325 459.325"
        style="enable-background: new 0 0 459.325 459.325"
        xml:space="preserve"
      >
        <g>
          <path
            fill="var(--svgcolor)"
            d="M459.319,229.668c0,22.201-17.992,40.193-40.205,40.193H269.85v149.271c0,22.207-17.998,40.199-40.196,40.193
		c-11.101,0-21.149-4.492-28.416-11.763c-7.276-7.281-11.774-17.324-11.769-28.419l-0.006-149.288H40.181
		c-11.094,0-21.134-4.492-28.416-11.774c-7.264-7.264-11.759-17.312-11.759-28.413C0,207.471,17.992,189.475,40.202,189.475h149.267
		V40.202C189.469,17.998,207.471,0,229.671,0c22.192,0.006,40.178,17.986,40.19,40.187v149.288h149.282
		C441.339,189.487,459.308,207.471,459.319,229.668z"
          />
        </g>
      </svg>

      <div class="search-bar">
        <input type="text" placeholder="Search" id="gamesInput" />
      </div>
      <div id="allGamesList" class="fadeInDown gamesList"></div>
    </div>

    <div class="secondorybox" id="favs">
      <p>Favourite Games</p>
      <div class="search-bar">
        <input type="text" placeholder="Search" id="favsInput" />
      </div>
      <div id="favGamesList" class="fadeInDown gamesList"></div>
    </div>

    <div class="secondorybox" id="messages">
      <p>Messages</p>
    </div>

    <div class="secondorybox" id="activity">
      <p>Activity</p>
    </div>

    <div class="secondorybox" id="friends">
      <p>Friends</p>
    </div>

    <div class="gameMenu" id="gameMenu">
      <div class="gameMenuTitle" id="gameMenuTitle"></div>
      <div class="gameMenuBtns">
        <button class="gameMenuBtn" id="removeGame">Remove Game</button>
      </div>
    </div>

    <div class="leftbar">
      <div class="d-flex justify-content-center">
        <div>
          <div class="user-pfp">
            <label for="file"></label>
            <input
              id="file"
              type="file"
              accept="image/png"
              @change="(event) => loadPFP(event)"
            />
            <img
              src="./assets/svg/default-profile.svg"
              alt="Avatar"
              width="88"
              id="output"
            />
          </div>
          <input
            class="username"
            id="text"
            type="text"
            value="Lazap"
            spellcheck="false"
            maxlength="12"
          />
        </div>
      </div>

      <div class="side-tabs">
        <div class="d-flex justify-content-center pb-2">
          <div class="side-tab" id="home-btn">
            <div class="indicator" id="indicator"></div>
            <img src="./assets/svg/home.svg" height="25" width="25" />
            <div class="side-tab-text">Home</div>
          </div>
        </div>

        <div class="category-name d-flex justify-content-left">Games</div>
        <div class="d-flex justify-content-center">
          <div class="side-tab" id="recent-btn">
            <img
              id="recently-btn-img"
              src="./assets/svg/recent.svg"
              height="25"
              width="25"
            />
            <div class="side-tab-text">Recent</div>
          </div>
        </div>
        <div class="d-flex justify-content-center">
          <div class="side-tab" id="games-btn">
            <img
              id="games-btn-img"
              src="./assets/svg/games.svg"
              height="25"
              width="25"
            />
            <div class="side-tab-text">All Games</div>
          </div>
        </div>
        <div class="d-flex justify-content-center pb-2">
          <div class="side-tab" id="favs-btn">
            <img
              id="favs-btn-img"
              src="./assets/svg/favs.svg"
              height="25"
              width="25"
            />
            <div class="side-tab-text">Favourites</div>
          </div>
        </div>

        <div class="category-name d-flex justify-content-start">Friends</div>
        <div class="d-flex justify-content-center">
          <div class="side-tab" id="messages-btn">
            <img src="./assets/svg/messages.svg" height="25" width="25" />
            <div class="side-tab-text">Messages</div>
          </div>
        </div>
        <div class="d-flex justify-content-center">
          <div class="side-tab" id="activity-btn">
            <img src="./assets/svg/activity.svg" height="25" width="25" />
            <div class="side-tab-text">Activity</div>
          </div>
        </div>
        <div class="d-flex justify-content-center">
          <div class="side-tab" id="friends-btn">
            <img src="./assets/svg/friends.svg" height="25" width="25" />
            <div class="side-tab-text">All Friends</div>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<script scoped>
import { selOption } from "./components/modules/rpcOptions";

export default {
  name: "-",
  props: ["changeComponent"],
  methods: {
    min_window() {
      window.__TAURI__.window.appWindow.minimize();
    },
    max_window() {
      window.__TAURI__.window.appWindow.toggleMaximize();
    },
    close_window() {
      window.__TAURI__.window.appWindow.close();
    },
    async loadPFP(event) {
      var selectedFile = event.target.files[0];
      var reader = new FileReader();

      reader.onload = async function () {
        await window.__TAURI__.fs.writeBinaryFile(
          (await window.__TAURI__.path.appDir()) + `storage/cache/user/pfp.png`,
          reader.result
        );
        document.getElementById("output").src =
          window.__TAURI__.tauri.convertFileSrc(
            (await window.__TAURI__.path.appDir()) +
              `storage/cache/user/pfp.png`
          ) + `?${new Date().getSeconds()}`;
      };
      reader.readAsArrayBuffer(selectedFile);
    },
    async loadCustomBanner(event) {
      var selectedFile = event.target.files[0];
      var reader = new FileReader();

      reader.onload = async function () {
        await window.__TAURI__.fs.writeBinaryFile(
          (await window.__TAURI__.path.appDir()) +
            `storage/cache/games/banners/newcustombanner.png`,
          reader.result
        );
        document.getElementById(
          "addGameCustomBannerOutput"
        ).style.backgroundImage =
          `url(` +
          window.__TAURI__.tauri.convertFileSrc(
            (await window.__TAURI__.path.appDir()) +
              `storage/cache/games/banners/newcustombanner.png`
          ) +
          `?${new Date().getSeconds()}` +
          `)`;
      };
      document.getElementById("addGameCustomBannerTxt").style.opacity = "0";
      reader.readAsArrayBuffer(selectedFile);
    },
  },
  async mounted() {
    if ((await window.__TAURI__.os.platform()) === "win32") {
      document.getElementById("bg").style.height = "111.5vh";
    }

    (async () => {
      const home = document.getElementById("home");
      const recent = document.getElementById("recent");
      const games = document.getElementById("games");
      const favs = document.getElementById("favs");
      const friends = document.getElementById("friends");
      const messages = document.getElementById("messages");
      const activity = document.getElementById("activity");
      const gameMenu = document.getElementById("gameMenu");
      const settings = document.getElementById("settings-popup");
      const settingsbackblur = document.getElementById("settings-backblur");
      const searchbars = document.querySelectorAll(
        'div.search-bar > input[type="text"]'
      );
      const fs = window.__TAURI__.fs;
      const path = window.__TAURI__.path;
      const dialog = window.__TAURI__.dialog;
      const invoke = window.__TAURI__.invoke;
      let timestamp = null;

      /** START OF STORAGE CHECK **/
      const appDirPath = await path.appDir();
      let data = {};
      data.enableRPC = true;
      data.launchOnStartup = false;
      data.skipLogin = false;
      data.trayMinLaunch = true;
      data.trayMinQuit = false;
      data.checkForUpdates = false;
      data.accentColor = "#7934FA";
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
      fs.createDir(
        appDirPath + "storage/cache/user",
        { recursive: true },
        (err) => {
          if (err) throw err;
        }
      );
      fs.createDir(
        appDirPath + "storage/cache/games",
        { recursive: true },
        (err) => {
          if (err) throw err;
        }
      );
      fs.createDir(
        appDirPath + "storage/cache/games/banners",
        { recursive: true },
        (err) => {
          if (err) throw err;
        }
      );

      try {
        JSON.parse(
          await fs.readTextFile(appDirPath + "storage/LauncherData.json")
        );
      } catch (e) {
        fs.writeTextFile(appDirPath + "storage/LauncherData.json", CONSTANTS);
      }

      try {
        JSON.parse(
          await fs.readTextFile(
            appDirPath + "storage/cache/user/UserProfile.json"
          )
        );
      } catch (e) {
        fs.writeTextFile(
          appDirPath + "storage/cache/user/UserProfile.json",
          JSON.stringify({ username: await invoke("sysusername") })
        );
      }

      try {
        JSON.parse(
          await fs.readTextFile(appDirPath + "storage/cache/games/data.json")
        );
      } catch (e) {
        fs.writeTextFile(
          appDirPath + "storage/cache/games/data.json",
          JSON.stringify([])
        );
      }
      /** END OF STORAGE CHECK **/

      let sysInfoInvoke = JSON.parse(
        "{" + (await invoke("get_sys_info")).replaceAll(`'`, `"`) + "}"
      );
      if (sysInfoInvoke.cpu.length > 22) {
        sysInfoInvoke.cpu = sysInfoInvoke.cpu.slice(0, 22) + "...";
      }
      document.getElementById("sysInfo").innerHTML = `
  <div> <span style="margin-right: 4px;color:#A3BE8C;">  </span> ${sysInfoInvoke.system_host}</div>
  <div> <span style="margin-right: 4px;color:#EBCB8B;">  </span>  ${sysInfoInvoke.system_name}</div>
  <div> <span style="margin-right: 4px;color:#EBCB8B;">  </span> ${sysInfoInvoke.system_kernel}</div>
  <div> <span style="margin-right: 4px;color:#5E81AC;">  </span> ${sysInfoInvoke.memory}</div>
  <div> <span style="margin-right: 4px;color:#5E81AC;">  </span> ${sysInfoInvoke.cpu}</div>`;

      /** START RICH PRESENCE IF ENABLED **/
      await startRPC();

      try {
        const data = JSON.parse(
          await fs.readTextFile(
            (await path.appDir()) + "storage/cache/user/UserProfile.json",
            (err) => {
              if (err) throw err;
            }
          )
        );
        document.getElementById("text").value = data.username;

        await fs.readBinaryFile(
          (await path.appDir()) + "storage/cache/user/pfp.png"
        );
        document.getElementById("output").src =
          window.__TAURI__.tauri.convertFileSrc(
            (await path.appDir()) + "storage/cache/user/pfp.png"
          ) + `?${new Date().getSeconds()}`;
      } catch (err) {
        //console.log(err)
      }

      try {
        let { accentColor } = JSON.parse(
          await fs.readTextFile(
            (await path.appDir()) + "storage/LauncherData.json"
          )
        );
        if (!accentColor) accentColor = "#7934FA";
        updateAccentColor(accentColor);
      } catch (error) {
        console.error(error);
      }

      const allGames = await require("./components/launchers/find-games")
        .getInstalledGames()
        .catch((err) => {
          return console.error(err);
        });
      const gamesdata = await require("./components/launchers/find-games")
        .getGames()
        .catch((err) => {
          return console.error(err);
        });

      await require("./components/launchers/find-games")
        .loadGames("recentGamesListMainPage", allGames, gamesdata)
        .catch((err) => {
          return console.error(err);
        });
      await require("./components/launchers/find-games")
        .loadGames("recentGamesList", allGames, gamesdata)
        .catch((err) => {
          return console.error(err);
        });
      await require("./components/launchers/find-games")
        .loadGames("allGamesList", allGames)
        .catch((err) => {
          return console.error(err);
        });
      await require("./components/launchers/find-games")
        .loadGames("favGamesList", allGames, gamesdata)
        .catch((err) => {
          return console.error(err);
        });

      document.getElementById("main-loading-overlay").style.opacity = "0";
      document.getElementById("main-loading-overlay").style.visibility =
        "hidden";

      document
        .getElementById("home-btn")
        .addEventListener("click", async function () {
          this.appendChild(document.getElementById("indicator"));
          if (home.style.display !== "flex") {
            toggleIndicatorAnim();
          }

          home.style.display = "flex";
          recent.style.display = "none";
          games.style.display = "none";
          favs.style.display = "none";
          friends.style.display = "none";
          messages.style.display = "none";
          activity.style.display = "none";
          gameMenu.style.display = "none";

          await require("./components/launchers/find-games")
            .loadGames("recentGamesListMainPage")
            .catch((err) => {
              return console.error(err);
            });

          setActivity("home");
        });

      document
        .getElementById("recent-btn")
        .addEventListener("click", async function () {
          this.appendChild(document.getElementById("indicator"));
          if (recent.style.display !== "flex") {
            toggleIndicatorAnim();
          }

          home.style.display = "none";
          recent.style.display = "flex";
          games.style.display = "none";
          favs.style.display = "none";
          friends.style.display = "none";
          messages.style.display = "none";
          activity.style.display = "none";
          gameMenu.style.display = "none";

          await require("./components/launchers/find-games")
            .loadGames("recentGamesList")
            .catch((err) => {
              return console.error(err);
            });

          setActivity("recent");
        });

      document
        .getElementById("games-btn")
        .addEventListener("click", async function () {
          this.appendChild(document.getElementById("indicator"));

          if (games.style.display !== "flex") {
            setTimeout(
              () => document.getElementById("gamesInput").focus(),
              100
            );
            toggleIndicatorAnim();
          }

          home.style.display = "none";
          recent.style.display = "none";
          games.style.display = "flex";
          favs.style.display = "none";
          friends.style.display = "none";
          messages.style.display = "none";
          activity.style.display = "none";
          gameMenu.style.display = "none";

          await require("./components/launchers/find-games")
            .loadGames("allGamesList")
            .catch((err) => {
              return console.error(err);
            });

          setActivity("games");
        });

      document
        .getElementById("favs-btn")
        .addEventListener("click", async function () {
          this.appendChild(document.getElementById("indicator"));

          if (favs.style.display !== "flex") {
            setTimeout(() => document.getElementById("favsInput").focus(), 100);
            toggleIndicatorAnim();
          }

          home.style.display = "none";
          recent.style.display = "none";
          games.style.display = "none";
          favs.style.display = "flex";
          messages.style.display = "none";
          activity.style.display = "none";
          friends.style.display = "none";
          gameMenu.style.display = "none";

          await require("./components/launchers/find-games")
            .loadGames("favGamesList")
            .catch((err) => {
              return console.error(err);
            });

          setActivity("favourites");
        });

      document
        .getElementById("messages-btn")
        .addEventListener("click", async function () {
          this.appendChild(document.getElementById("indicator"));
          if (messages.style.display !== "flex") {
            toggleIndicatorAnim();
          }

          home.style.display = "none";
          recent.style.display = "none";
          games.style.display = "none";
          favs.style.display = "none";
          messages.style.display = "flex";
          activity.style.display = "none";
          friends.style.display = "none";
          gameMenu.style.display = "none";

          setActivity("messages");
        });

      document
        .getElementById("activity-btn")
        .addEventListener("click", async function () {
          this.appendChild(document.getElementById("indicator"));
          if (activity.style.display !== "flex") {
            toggleIndicatorAnim();
          }

          home.style.display = "none";
          recent.style.display = "none";
          games.style.display = "none";
          favs.style.display = "none";
          messages.style.display = "none";
          activity.style.display = "flex";
          friends.style.display = "none";
          gameMenu.style.display = "none";

          setActivity("activity");
        });

      document
        .getElementById("friends-btn")
        .addEventListener("click", async function () {
          this.appendChild(document.getElementById("indicator"));
          if (friends.style.display !== "flex") {
            toggleIndicatorAnim();
          }

          home.style.display = "none";
          recent.style.display = "none";
          games.style.display = "none";
          favs.style.display = "none";
          messages.style.display = "none";
          activity.style.display = "none";
          friends.style.display = "flex";
          gameMenu.style.display = "none";

          setActivity("friends");
        });

      document
        .getElementById("text")
        .addEventListener("change", async function (change) {
          this.appendChild(document.getElementById("indicator"));
          fs.writeTextFile(
            (await path.appDir()) + "storage/cache/user/UserProfile.json",
            JSON.stringify({ username: change.target.value }),
            (err) => {
              if (err) throw err;
            }
          );
        });

      settingsbackblur.addEventListener("click", () => {
        settings.style.display = "none";
        settingsbackblur.style.display = "none";
      });

      document
        .getElementById("settings-btn")
        .addEventListener("click", async () => {
          settingsbackblur.style.display = "flex";
          settings.style.display = "flex";

          const appDirPath = await path.appDir();
          const Data = JSON.parse(
            await fs.readTextFile(appDirPath + "storage/LauncherData.json")
          );
          document.querySelectorAll("input[id^=setting-]").forEach((input) => {
            if (input.id === "setting-accentColor")
              input.value = Data[input.id.split("-")[1]];
            else input.checked = Data[input.id.split("-")[1]] ? true : false;
          });
        });

      document.getElementById("generalbtn").addEventListener("click", () => {
        document.getElementById("general-settings").style.display = "flex";
        document.getElementById("appearance-settings").style.display = "none";
      });

      document.getElementById("appearancebtn").addEventListener("click", () => {
        document.getElementById("general-settings").style.display = "none";
        document.getElementById("appearance-settings").style.display = "flex";
      });

      document
        .getElementById("winerunnersbtn")
        .addEventListener("click", () => {
          document.getElementById("general-settings").style.display = "none";
          document.getElementById("appearance-settings").style.display = "flex";
        });

      document
        .querySelector(".repeatButton")
        .addEventListener("click", async () => {
          let LauncherData = JSON.parse(
            await fs.readTextFile(
              (await path.appDir()) + "storage/LauncherData.json"
            )
          );
          LauncherData["accentColor"] = "#7934FA";
          fs.writeTextFile(
            (await path.appDir()) + "storage/LauncherData.json",
            JSON.stringify(LauncherData)
          );
          document.querySelector("input[id=setting-accentColor]").value =
            LauncherData["accentColor"];
          updateAccentColor(LauncherData["accentColor"]);
        });

      document.querySelectorAll("input[id^=setting-]").forEach((input) => {
        if (input.id === "setting-accentColor") {
          input.addEventListener("input", async () => {
            const LauncherData = JSON.parse(
              await fs.readTextFile(
                (await path.appDir()) + "storage/LauncherData.json"
              )
            );
            LauncherData[input.id.split("-")[1]] = document.querySelector(
              `input[id=${input.id}]`
            ).value;
            fs.writeTextFile(
              (await path.appDir()) + "storage/LauncherData.json",
              JSON.stringify(LauncherData)
            );

            updateAccentColor(LauncherData.accentColor);
          });
          return;
        }
        input.addEventListener("change", async () => {
          const LauncherData = JSON.parse(
            await fs.readTextFile(
              (await path.appDir()) + "storage/LauncherData.json"
            )
          );
          LauncherData[input.id.split("-")[1]] = document.querySelector(
            `input[id=${input.id}]`
          ).checked;
          fs.writeTextFile(
            (await path.appDir()) + "storage/LauncherData.json",
            JSON.stringify(LauncherData)
          );

          if (input.id === "setting-enableRPC") startRPC();
        });
      });

      searchbars.item(0).addEventListener("keyup", () => {
        const query = searchbars.item(0).value;
        const allGames = document.querySelectorAll(
          '#allGamesList > div[id^="game-div"]'
        );
        allGames.forEach((game) => {
          if (
            game.id
              .split("-")
              .slice(2)
              .join("-")
              .match(new RegExp(`${query.replaceAll(" ", "_")}`, "gi"))
          ) {
            game.style.display = "block";
          } else if (query.length === 0) {
            game.style.display = "block";
          } else {
            game.style.display = "none";
          }
        });
      });
      searchbars.item(1).addEventListener("keyup", () => {
        const query = searchbars.item(1).value;
        const allGames = document.querySelectorAll(
          '#favGamesList > div[id^="game-div"]'
        );

        allGames.forEach((game) => {
          if (
            game.id
              .split("-")
              .slice(2)
              .join("-")
              .match(new RegExp(`${query.replaceAll(" ", "_")}`, "gi"))
          ) {
            game.style.display = "block";
          } else if (query.length === 0) {
            game.style.display = "block";
          } else {
            game.style.display = "none";
          }
        });
      });

      document.addEventListener("keydown", async function (e) {
        if (e.ctrlKey && e.key === "r") {
          window.location.reload();
        }
      });

      document
        .getElementById("addGameBtn")
        .addEventListener("click", async function () {
          if (
            window.getComputedStyle(document.getElementById("addGamePopUp"))
              .display === "flex"
          ) {
            document.getElementById(
              "addGameCustomBannerOutput"
            ).style.backgroundImage = "url()";
            document.getElementById("addGamePopUp").style.display = "none";
            document.getElementById("inputGameName").value = "";
            document.getElementById("addGameCustomBannerTxt").style.opacity =
              "1";
            try {
              await fs.removeFile(
                (await path.appDir()) +
                  `storage/cache/games/banners/newcustombanner.png`
              );
            } catch (e) {
              return e;
            }
          } else {
            document.getElementById("addGamePopUp").style.display = "flex";
            document.getElementById("inputGameName").focus();
          }
        });

      let newGameLocation;
      document
        .getElementById("addGameLocation")
        .addEventListener("click", async function () {
          const selected = await dialog.open({
            multiple: false,
            filters: [
              {
                name: "Windows Executable",
                extensions: ["exe"],
              },
            ],
          });
          if (selected !== null) {
            newGameLocation = selected;
          }
        });

      document
        .getElementById("addGameFinalBtn")
        .addEventListener("click", async function () {
          if (
            document.getElementById("inputGameName").value.trim().length > 0 &&
            newGameLocation
          ) {
            let scheme = {
              DisplayName: document.getElementById("inputGameName").value,
              LauncherName: "CustomGame",
              GameID: "CustomGame",
              Executable: newGameLocation.split("\\").slice(-1)[0],
              Location: newGameLocation.split("\\").slice(0, -1).join("\\"),
              Args: [],
            };

            try {
              await fs.renameFile(
                (await path.appDir()) +
                  `storage/cache/games/banners/newcustombanner.png`,
                (await path.appDir()) +
                  `storage/cache/games/banners/${require("./components/modules/sha256.js").sha256(
                    document
                      .getElementById("inputGameName")
                      .value.replaceAll(" ", "_")
                  )}.png`
              );
              require("./components/launchers/find-games").Elements.createGameElement(
                scheme,
                "allGamesList"
              );
              let data = JSON.parse(
                await fs.readTextFile(
                  (await path.appDir()) + "storage/cache/games/data.json"
                )
              );
              data.push(scheme);
              await fs.writeTextFile(
                (await path.appDir()) + "storage/cache/games/data.json",
                JSON.stringify(data)
              );
            } catch (e) {
              require("./components/launchers/find-games").Elements.createGameElement(
                scheme,
                "allGamesList"
              );
              let data = JSON.parse(
                await fs.readTextFile(
                  (await path.appDir()) + "storage/cache/games/data.json"
                )
              );
              data.push(scheme);
              await fs.writeTextFile(
                (await path.appDir()) + "storage/cache/games/data.json",
                JSON.stringify(data)
              );
            }
            document.getElementById(
              "addGameCustomBannerOutput"
            ).style.backgroundImage = "url()";
            document.getElementById("addGamePopUp").style.display = "none";
            document.getElementById("inputGameName").value = "";
            document.getElementById("addGameCustomBannerTxt").style.opacity =
              "1";
            newGameLocation = "";
          } else if (
            document.getElementById("inputGameName").value.trim().length > 0 &&
            !newGameLocation
          ) {
            return alert("You are missing the game location.");
          } else if (
            document.getElementById("inputGameName").value.length < 0 &&
            newGameLocation
          ) {
            return alert("You are missing the game name.");
          } else {
            return alert(
              "You are missing the game name, and the game location."
            );
          }
        });

      document
        .getElementById("removeGame")
        .addEventListener("click", async function () {
          let data = JSON.parse(
            await fs.readTextFile(
              (await path.appDir()) + "storage/cache/games/data.json"
            )
          );
          data = data.filter(
            (a) =>
              a.DisplayName !=
              document.getElementById("removeGame").parentNode.parentNode
                .firstChild.innerHTML
          );
          await fs.writeTextFile(
            (await path.appDir()) + "storage/cache/games/data.json",
            JSON.stringify(data)
          );
          document
            .getElementById(
              `game-div-${document
                .getElementById("removeGame")
                .parentNode.parentNode.firstChild.innerHTML.replaceAll(
                  " ",
                  "_"
                )}`
            )
            .remove();
          setTimeout(
            () =>
              (document.getElementById(
                "removeGame"
              ).parentNode.parentNode.style = ""),
            200
          );
        });

      function toggleIndicatorAnim() {
        let indicator = document.getElementById("indicator");
        indicator.classList.add("anim-indicatorscaleY");
        setTimeout(() => {
          indicator.classList.remove("anim-indicatorscaleY");
        }, 200);
      }
      function updateAccentColor(accentColor) {
        document.getElementById("indicator").style.backgroundColor =
          accentColor;
        document
          .querySelector(":root")
          .style.setProperty("--back", accentColor);
      }
      async function setActivity(tab) {
        const { state, details, largeImage, largeText, smallImage, smallText } =
          selOption(tab);
        if (timestamp === null) timestamp = Date.now();
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
      async function startRPC() {
        const { enableRPC } = JSON.parse(
          await fs.readTextFile(
            (await path.appDir()) + "storage/LauncherData.json"
          )
        );

        try {
          if (!enableRPC) throw Error("RPC must not be enabled");
          invoke("disable_rpc", { enable: true });
          setActivity("home");

          /** Initial RPC stuff */
          document.getElementById("rpc").innerHTML = "Connected to Discord";
        } catch (error) {
          invoke("disable_rpc", { enable: false });
          console.error(error);
          //document.getElementById("rpcbtn").style.display = "none";
          document.getElementById("rpc").innerHTML = "Disconnected";
        }
      }
    })();
  },
};
</script>

<style scoped>
@import "./css/main.css";
</style>
