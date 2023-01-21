<template>
  <settings-comp></settings-comp>
  <titlebar-comp></titlebar-comp>

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
import Settings from "./components/Settings.vue";
import Titlebar from "./components/Titlebar.vue";
import { selOption } from "./components/modules/rpcOptions";

export default {
  name: "App",
  components: {
    "settings-comp": Settings,
    "titlebar-comp": Titlebar,
  },
  methods: {
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
    (async () => {
      const fs = window.__TAURI__.fs;
      const path = window.__TAURI__.path;
      const dialog = window.__TAURI__.dialog;
      const invoke = window.__TAURI__.invoke;

      const home = document.getElementById("home");
      const recent = document.getElementById("recent");
      const games = document.getElementById("games");
      const favs = document.getElementById("favs");
      const friends = document.getElementById("friends");
      const messages = document.getElementById("messages");
      const activity = document.getElementById("activity");
      const gameMenu = document.getElementById("gameMenu");
      const searchbars = document.querySelectorAll(
        'div.search-bar > input[type="text"]'
      );

      let timestamp = null;

      let sysInfoInvoke = JSON.parse(await invoke("get_sys_info"));
      if (sysInfoInvoke.cpu.length > 22) {
        sysInfoInvoke.cpu = sysInfoInvoke.cpu.slice(0, 22) + "...";
      }
      document.getElementById("sysInfo").innerHTML = `
  <div> <span style="margin-right: 4px;color:#A3BE8C;">  </span> ${sysInfoInvoke.system_host}</div>
  <div> <span style="margin-right: 4px;color:#EBCB8B;">  </span>  ${sysInfoInvoke.system_name}</div>
  <div> <span style="margin-right: 4px;color:#EBCB8B;">  </span> ${sysInfoInvoke.system_kernel}</div>
  <div> <span style="margin-right: 4px;color:#5E81AC;">  </span> ${sysInfoInvoke.memory}</div>
  <div> <span style="margin-right: 4px;color:#5E81AC;">  </span> ${sysInfoInvoke.cpu}</div>`;

      await invoke("show_window");

      try {
        const data = JSON.parse(
          await invoke("read_file", {
            filePath: (await path.appDir()) + "cache/user/data.json",
          })
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
          await invoke("read_file", {
            filePath: (await path.appDir()) + "LauncherData.json",
          })
        );
        if (!accentColor) accentColor = "#7934FA";
        updateAccentColor(accentColor);
      } catch (error) {
        console.error(error);
      }

      try {
        const { enable_rpc } = JSON.parse(
          await invoke("read_file", {
            filePath: (await path.appDir()) + "LauncherData.json",
          })
        );
        if (!enable_rpc) throw console.log("RPC must not be enabled.");
        invoke("disable_rpc", { enable: true });
        setActivity("home");

        document.getElementById("rpc").innerHTML = "Connected to Discord";
      } catch (error) {
        invoke("disable_rpc", { enable: false });
        document.getElementById("rpc").innerHTML = "Disconnected";
      }

      const allGames = await require("./components/launchers/find-games.js")
        .getInstalledGames()
        .catch((err) => {
          return console.error(err);
        });
      const gamesdata = await require("./components/launchers/find-games.js")
        .getGames()
        .catch((err) => {
          return console.error(err);
        });

      await require("./components/launchers/find-games.js")
        .loadGames("recentGamesListMainPage", allGames, gamesdata)
        .catch((err) => {
          return console.error(err);
        });
      await require("./components/launchers/find-games.js")
        .loadGames("recentGamesList", allGames, gamesdata)
        .catch((err) => {
          return console.error(err);
        });
      await require("./components/launchers/find-games.js")
        .loadGames("allGamesList", allGames)
        .catch((err) => {
          return console.error(err);
        });
      await require("./components/launchers/find-games.js")
        .loadGames("favGamesList", allGames, gamesdata)
        .catch((err) => {
          return console.error(err);
        });

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

          await require("./components/launchers/find-games.js")
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

          await require("./components/launchers/find-games.js")
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

          await require("./components/launchers/find-games.js")
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

          await require("./components/launchers/find-games.js")
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
          await invoke("write_file", {
            filePath: (await path.appDir()) + "cache/user/data.json",
            fileContent: JSON.stringify({ username: change.target.value }),
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
              require("./components/launchers/find-games.js").Elements.createGameElement(
                scheme,
                "allGamesList"
              );
              let data = JSON.parse(
                await invoke("read_file", {
                  filePath: (await path.appDir()) + "cache/games/data.json",
                })
              );
              data.push(scheme);
              await invoke("write_file", {
                filePath: (await path.appDir()) + "cache/games/data.json",
                fileContent: JSON.stringify(data),
              });
            } catch (e) {
              require("./components/launchers/find-games.js").Elements.createGameElement(
                scheme,
                "allGamesList"
              );
              let data = JSON.parse(
                await invoke("read_file", {
                  filePath: (await path.appDir()) + "cache/games/data.json",
                })
              );
              data.push(scheme);
              await invoke("write_file", {
                filePath: (await path.appDir()) + "cache/games/data.json",
                fileContent: JSON.stringify(data),
              });
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
            await invoke("read_file", {
              filePath: (await path.appDir()) + "cache/games/data.json",
            })
          );
          data = data.filter(
            (a) =>
              a.DisplayName !=
              document.getElementById("removeGame").parentNode.parentNode
                .firstChild.innerHTML
          );
          await invoke("write_file", {
            filePath: (await path.appDir()) + "cache/games/data.json",
            fileContent: JSON.stringify(data),
          });
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
    })();
  },
};
</script>

<style>
:root {
  --svgcolor: #656565;
  --back: rgb(121, 52, 250);
}

::selection {
  color: inherit;
  background: #2e3038;
  border-radius: 10px;
}

@font-face {
  font-family: Nunito;
  src: url("./assets/fonts/Nunito-Medium.ttf") format("truetype");
}

@font-face {
  font-family: Nunito-Bold;
  src: url("./assets/fonts/Nunito-SemiBold.ttf") format("truetype");
}

body {
  zoom: 0.895;
  background: #181a1f;
  overflow: hidden;
  font-family: Nunito;
}

.bg {
  display: flex;
  justify-content: center;
  align-items: center;
  flex-direction: column;
  height: 100vh;
  margin: -20px 0 50px;
}

.mx-1 {
  margin-right: 0.25rem !important;
  margin-left: 0.25rem;
}

.homebox {
  position: absolute;

  display: flex;
  flex-direction: column;
  flex-wrap: wrap;

  height: calc(100% - 65px);
  width: calc(100% - 300px);
  margin-bottom: 5px;
  margin-left: 285px;
}

.secondorybox {
  position: absolute;

  display: none;
  flex-direction: column;
  flex-wrap: wrap;

  background-color: #1c1d22;
  border-radius: 10px;

  height: calc(100% - 72px);
  width: calc(100% - 310px);
  margin-bottom: 5px;
  margin-left: 285px;
  cursor: default;
}

.secondorybox p {
  color: rgb(197, 197, 197);
  margin-left: 30px;
  margin-top: 30px;
  font-size: 20px;
  float: left;
}

.secondorybox .addGameBtn {
  position: absolute;
  width: 15px;
  height: 15px;
  margin-top: 36px;
  margin-left: 138px;
  transition: 0.1s all linear;
  color: rgb(255, 255, 255);
}

.secondorybox .addGameBtn:hover {
  --svgcolor: var(--back);
  cursor: pointer;
}

.secondorybox .addGamePopUp {
  position: absolute;
  width: 380px;
  height: 200px;
  margin-left: 170px;
  margin-top: 20px;
  background-color: #242424;
  animation: gradient 20s infinite;
  display: none;

  justify-content: center;
  align-items: center;

  border-radius: 10px 20px 20px 20px;
  z-index: 1;
}

.secondorybox .addGamePopUp .addGameBannerSection .banner {
  position: absolute;
  width: 150px;
  height: 150px;
  opacity: 0;
}

.secondorybox .addGamePopUp .addGameBannerSection .banner:hover {
  cursor: pointer;
}

.secondorybox .addGamePopUp .addGameBannerSection .addGameCustomBannerOutput {
  border-radius: 10px;
  width: 150px;
  height: 150px;
  image-rendering: auto;
  border: none;
  outline: none;
  object-fit: cover;
  margin-left: auto;
  margin-right: 25px;
  box-shadow: 0 3px 16px -7px rgb(17 18 24 / 70%);
  background-position: center center;
  background-size: cover;
  text-align: center;
  background-repeat: no-repeat;
  padding: 0;
  background-color: #313131;
  transition: all 0.1s ease-in-out;
  z-index: 0;
}

.secondorybox .addGamePopUp .addGameBannerSection:hover {
  border-radius: 10px;
  transition: all 0.1s ease-in-out;
  scale: 1.05;
  cursor: pointer;
}

.secondorybox .addGamePopUp .addGameBannerSection p {
  position: absolute;
  margin-top: 64px;
  margin-left: 25px;
  font-size: 12px;
  z-index: 2;
  width: 100px;
  text-align: center;
  pointer-events: none;
}

.secondorybox .addGamePopUp .addGameBannerSection label {
  position: absolute;
  cursor: pointer;
  width: 30px;
  height: 150px;
}

.secondorybox .mainSection {
  display: flex;
  flex-direction: column;
  margin-top: -25px;
}

.secondorybox .section {
  position: relative;
  align-self: flex-start;
  padding-bottom: -30px;
}

.secondorybox .section .title {
  position: absolute;

  align-self: flex-start;
  margin-top: 20px;
  margin-left: 22px;
  font-size: 14px;

  color: rgb(212, 212, 212);
  font-family: Nunito-Bold;
  cursor: default;
}

.secondorybox .section .inputGameName {
  width: 160px;
  height: 20px;
  margin-left: 20px;
  outline: none;
  border: none;
  align-self: flex-start;
  margin-right: 20px;
  margin-top: 44px;

  padding: 5px;
  background-color: #323232;
  color: rgb(180, 180, 180);
  border-radius: 8px;
  font-family: Nunito-Bold;
}

.secondorybox .section .addGameLocation {
  width: 170px;
  height: 40px;
  margin-left: 20px;
  outline: none;
  border: none;
  align-self: flex-start;
  margin-right: 20px;
  margin-top: 14px;
  background-color: #3e3e3e;
  color: rgb(194, 194, 194);
  padding: 10px;
  border-radius: 12px;
  font-family: Nunito-Bold;
}

.secondorybox .section .addGameLocation:hover {
  background-color: #393939;
}

.secondorybox .section .addGameFinalBtn {
  width: 170px;
  height: 30px;
  margin-left: 20px;
  outline: none;
  border: none;
  align-self: flex-start;
  margin-right: 20px;
  margin-top: 10px;
  background-color: #7f3db5;
  color: rgb(216, 216, 216);
  border-radius: 12px;
  font-family: Nunito-Bold;
}

.secondorybox .section .addGameFinalBtn:hover {
  cursor: pointer;
}

.children {
  display: flex;
  height: 100%;
  width: 71%;
  overflow: hidden;
  margin: 5px;

  flex: 1;
  justify-content: center;
  align-items: center;
}

.children:last-child {
  position: relative;
  width: 27%;
  flex: 0 0 calc(100% - 10px);
}

.head-pic {
  width: 200%;
  height: 100%;
  overflow: unset;
  position: relative;
  object-fit: cover;
  flex-shrink: 10;
  min-width: calc(100% - 100px);
  min-height: 100%;
  border-radius: 10px;
  image-rendering: auto;
}

.jump-back {
  background-color: #1c1d22;
  width: 100%;
  height: 100%;

  border-radius: 10px;
}

.jump-back p {
  color: rgb(197, 197, 197);
  margin-left: 18px;
  margin-top: 18px;
  font-size: 18px;
  font-family: Nunito-Bold;
}

.rightbar {
  background-color: #1c1d22;
  width: 100%;
  height: 100%;
  border-radius: 10px;
  cursor: default;
}

.rightbar p {
  color: rgb(197, 197, 197);
  margin-left: 18px;
  margin-top: 18px;
  font-size: 18px;
  font-family: Nunito-Bold;
}

.rightbar .sysInfo {
  margin-top: 30px;
  display: flex;
  flex-direction: column;
  margin-left: 34px;
  margin-right: auto;
  width: 280px;
  font-size: 17px;
  color: rgb(138, 138, 138);
}

.rightbar .sysInfo div {
  margin-bottom: 4px;
}

.rightbar .stickers {
  display: flex;
  justify-content: center;
  margin-top: 290px;
}

.rightbar .stickers img {
  width: 100px;
}

.leftbar {
  background-color: #1c1d22;
  border-radius: 15px;
  position: absolute;
  height: calc(100% - 71px);
  max-height: 100%;
  width: 270px;
  top: 25px;
  margin-top: 30px;
  margin-right: 100%;
  margin-left: 300px;
  text-align: center;
  box-shadow: 0 3px 16px -7px rgb(17 18 24 / 70%);
}

.user-pfp {
  position: relative;
  margin-top: 50px;
  width: 88px;
  height: 88px;
  margin-left: auto;
  margin-right: auto;
  transition: all 0.2s ease-in-out;
}

.user-pfp:hover {
  cursor: pointer;
  transition: all 0.2s ease-in-out;
  border-radius: 14px;
  scale: 1.1;
}

.user-pfp input {
  display: none;
}

.user-pfp img {
  border-radius: 18px;
  image-rendering: auto;
  width: 88px;
  height: 88px;
  object-fit: cover;
  box-shadow: 0 3px 16px -7px rgb(17 18 24 / 70%);
}

.user-pfp label {
  position: absolute;
  cursor: pointer;
  height: 70px;
  width: 70px;
}

.username {
  margin-top: 8px;
  margin-bottom: 26px;
  text-align: center;
  font-size: 22px;
  font-family: Nunito-Bold;
  color: #b9b9b9;
  background-color: transparent;
  border: transparent;
  text-decoration: none;
}

.side-tabs {
  margin-left: 20px;
  position: absolute;
  height: 66%;
  overflow-y: auto;
}

.side-tab {
  background-color: rgba(30, 30, 34, 0.5);
  box-shadow: -1px 3px 8px -1px rgba(0, 0, 0, 0.2);
  height: 40px;
  width: 230px;
  border-radius: 10px;
  display: flex;
  margin-bottom: 10px;
  z-index: 1;
  transition: color 0.15s ease-out;
}

.side-tab:before {
  background: rgb(30, 30, 34);
  background: linear-gradient(
    30deg,
    rgba(30, 30, 34, 1) 0%,
    rgba(30, 30, 34, 1) 30%,
    var(--back) 100%
  );
  content: "";
  height: 40px;
  width: 230px;
  border-radius: 10px;
  display: flex;
  margin-bottom: 10px;
  position: absolute;
  z-index: -1;
  opacity: 0;
  transition: opacity 0.15s ease-out;
}

.side-tab:hover:before {
  opacity: 1;
}

.side-tab:hover {
  cursor: default;
  color: #fff;
}

.side-tab img {
  filter: invert(100%) sepia(6%) saturate(0%) hue-rotate(115deg)
    brightness(108%) contrast(108%);
  margin-left: 25px;
  margin-top: 8px;
  margin-right: 5px;
  display: inline-block;
}

.side-tab .side-tab-text {
  color: #dadada;
  font-size: 18px;
  margin-left: 16px;
  align-self: center;
  margin-top: 4px;
  cursor: default;
}

.category-name {
  color: #7a7a7a;
  margin-left: 10px;
  margin-bottom: 4px;
  font-size: 15px;
}

.category-name:hover {
  cursor: default;
}

.indicator {
  transition: 0.3s;
  position: absolute;
  margin-top: 5px;
  margin-left: 2px;

  background-color: var(--back);
  height: 32px;
  width: 5px;
  border-radius: 50px;
  display: inline;
}

.anim-indicatorscaleY {
  animation: indicatorscaleY 0.2s linear;
}

.gamesList {
  position: absolute;
  overflow-y: scroll;
  width: 97%;
  height: 81%;
  margin-top: 100px;
  margin-left: 18px;
  display: flex;
  flex-wrap: wrap;
  align-content: flex-start;
  align-items: center;
  justify-content: flex-start;
}

.gamebox {
  position: relative;
  margin-bottom: 30px;
  text-align: center;
  display: inline-block;

  margin-right: 10px;
  margin-left: 20px;
  height: 280px;
  width: 200px;
  opacity: 0.8;
}

.gamebox:hover {
  box-shadow: 0 2px 10px rgba(0, 0, 0, 0.4);
  opacity: 1;
  cursor: pointer;
}

.gamebox img {
  height: 280px;
  width: 200px;
  border-radius: 12px;
  image-rendering: auto;
  object-fit: cover;
  mask-image: -webkit-gradient(
    linear,
    left 70%,
    left bottom,
    from(rgba(0, 0, 0, 1)),
    to(rgba(0, 0, 0, 0))
  );
  content: url("./assets/img/default-game-banner.png");
}

.gamebox .gamebox-bottom {
  cursor: default;
  position: relative;
  margin-top: -24%;
  width: 100%;
  height: 15%;
  position: absolute;
  background-color: #292929;
  border-radius: 0px 0px 10px 10px;
}

.gamebox .gamebox-bottom span {
  font-family: Nunito-Bold;
  position: absolute;
  margin-top: 14px;
  margin-left: 10px;
  font-size: 12px;
  color: #dadada;
  display: block;
}

.gamebox .gamebox-bottom .menu {
  position: absolute;

  margin-left: 165px;
  margin-top: 13px;
  display: block;
  content: url("./assets/svg/menu.svg");
  height: 18px;
  width: 18px;
  cursor: pointer;
  cursor: default;
}

.mainPageGamesList {
  position: absolute;
  width: 100%;
  height: 100%;
  display: flex;
  align-content: center;
  justify-content: center;
  align-items: center;

  margin-top: -44px;
}

.mainPageGamebox {
  position: relative;

  text-align: center;
  display: inline-block;
  border-radius: 14px;
  transition: all 0.25s cubic-bezier(0.165, 0.84, 0.44, 1);
  height: 62%;
  width: 16.8%;
  box-shadow: 0 2px 10px rgba(0, 0, 0, 0.1);
  opacity: 0.8;
  border: solid var(--back) 4px;
}

.mainPageGamebox:nth-last-child(-n + 4) {
  margin-left: 2%;
}

.mainPageGamebox:hover {
  box-shadow: 0 2px 10px rgba(0, 0, 0, 0.5);
  transform: scale(1.03);
  cursor: pointer;
  opacity: 1;
}

.mainPageGamebox img {
  object-fit: cover;
  object-position: top;
  display: block;
  height: 100%;
  width: 100%;
  border-radius: 10px;
}

.mainPageGamebox span {
  position: absolute;
  margin-top: -28px;
  margin-left: 10px;
  font-size: 14px;
  color: #d3d3d3;
  display: block;
}

.star {
  position: absolute;

  margin-left: 135px;
  margin-top: 10px;
  display: block;
  content: url("./assets/svg/star-empty.svg");
  height: 20px;
  width: 20px;
}

.star-fill {
  content: url("./assets/svg/star-solid.svg") !important;
}

.star:hover {
  display: block;
  content: url("./assets/svg/star-solid.svg") !important;
  cursor: pointer;
}

.fade {
  animation: fadeIn 0.5s;
}

.fadeInUp {
  animation-duration: 0.5s;
  animation-fill-mode: both;
  backface-visibility: hidden;
  opacity: 0;
  animation-name: fadeInUp;
}

.fadeInDown {
  animation-duration: 0.5s;
  animation-fill-mode: both;
  backface-visibility: hidden;
  opacity: 0;
  animation-name: fadeInDown;
}

.fadeInLeft {
  animation-duration: 0.5s;
  animation-fill-mode: both;
  backface-visibility: hidden;
  opacity: 0;
  animation-name: fadeInLeft;
}

.shake {
  animation-duration: 0.5s;
  animation-name: shake;
}

.d-flex {
  display: flex !important;
}

.justify-content-start {
  justify-content: flex-start !important;
}

.justify-content-center {
  justify-content: center !important;
}

.pb-2 {
  padding-bottom: 0.4rem !important;
}

.pb-5 {
  padding-bottom: 2rem !important;
}

.notFound {
  position: relative;
  text-align: center;
  margin-top: 25%;
  color: rgb(197, 197, 197);

  font-size: 22px;
}

::-webkit-scrollbar {
  width: 6px;
  background: rgba(121, 121, 121, 0);
  height: 6px;
}

::-webkit-scrollbar:hover {
  width: 8px;
  height: 8px;
}

::-webkit-scrollbar-thumb {
  background: #63636396;
  border-radius: 8px;
}

::-webkit-scrollbar-thumb:hover {
  background: #83838396;
  height: 30px;
  border-radius: 10px;
  width: 120px !important;
}

::-webkit-scrollbar-track-piece {
  display: none;
}

::-webkit-scrollbar-corner {
  display: none;
}

:not(input):not(textarea),
:not(input):not(textarea)::after,
:not(input):not(textarea)::before {
  -webkit-user-select: none;
  user-select: none;
  -webkit-user-drag: none;
  user-select: none;
  -moz-user-select: none;
  -ms-user-select: none;
}

input,
button,
textarea,
img,
:focus {
  outline: none;
}

.gameMenu {
  display: none;
  position: absolute;
  right: 20px;
  height: 90%;
  background: rgba(35, 35, 37, 1);
  border-bottom-right-radius: 10px;
  border-top-right-radius: 10px;
  border-top-left-radius: 20px;
  border-bottom-left-radius: 20px;
  animation: gameMenuAnimation 0.2s both ease-out;
  justify-content: center;
}

.gameMenu .gameMenuBtn {
  font-family: Nunito;
  color: rgb(201, 201, 201);
  border-radius: 12px;
  font-size: 18px;

  background-color: #3e3e3e;
  opacity: 0.9;
  width: 13.78rem;
  height: 50px;
  border: none;
  text-decoration: none;
  text-shadow: none;
  padding: 0;
}

.gameMenu .gameMenuBtns .gameMenuBtn:first-child {
  margin-top: 100px;
}

.gameMenu .gameMenuTitle {
  position: absolute;
  font-size: 20px;
  color: #cacaca;
  text-align: center;
  margin-top: 40px;
  display: block;
}

.gameMenu .gameMenuBtn:hover {
  background-color: #373737;
}

.subSetting {
  display: flex;
  justify-content: space-between;
  align-items: center;
}


.leftbar-overlay {
  position: absolute;
  display: flex;
  height: -webkit-fill-available;
  width: -webkit-fill-available;
  border-radius: inherit;
  opacity: 0;
  visibility: hidden;
  z-index: 1;
  background: inherit;
}

.alert-box {
  position: absolute;
  display: flex;
  height: 100px;
  width: 300px;
  border-radius: 10px;
  box-shadow: 0 0 5px #9521f3, 0 0 10px #ae00ff, 0 0 15px #f704f7;
  background-color: #1a1a1aaf;
  opacity: 1;
  margin-left: calc(50% - 150px);
  margin-top: -120px;
  background-color: none;
  backdrop-filter: blur(6px);
  z-index: 3;
}

.alert-box-container {
  margin-top: 10px;
  margin-left: 12px;
  height: -webkit-fill-available;
  width: -webkit-fill-available;
  display: inline-flex;
}

.alert-box-title {
  display: block;
  margin-top: 3px;
  margin-left: 6px;
  margin-block-end: 0px;
  color: rgb(197, 197, 197);
  font-size: 20px;
}

.alert-box-exclamation {
  height: 25px;
  width: 25px;
  display: flex;
  filter: invert(12%) sepia(78%) saturate(4656%) hue-rotate(1deg)
    brightness(108%) contrast(128%);
}

.alert-box-cross {
  margin-top: 2px;
  color: white;
  font-size: 24px;
  cursor: pointer;
  transition: 0.3s;
  position: absolute;
  margin-left: calc(100% - 39px);
}

.alert-box-cross:hover {
  filter: invert(21%) sepia(70%) saturate(3776%) hue-rotate(255deg)
    brightness(97%) contrast(101%);
}

.alert-box-divider {
  align-self: baseline;
  height: 0px;
  width: calc(100% - 14px);
  margin-top: 11%;
  margin-left: -80px;
}

.alert-box-message {
  color: rgb(197, 197, 197);
  align-self: flex-end;
  font-size: 13px;
  margin-block-end: 0;
  margin-block-start: 0;
  position: fixed;
  height: 45px;
  width: 275px;
  text-align: center;
}

.search-bar {
  margin-left: auto;
  margin-right: 110px;
  margin-top: -52px;
  height: 40px;
  display: flex;
  width: 100%;
  max-width: 350px;
  padding-left: 16px;
  border-radius: 4px;
  display: inline;
}

.search-bar input {
  width: 100%;
  height: 100%;
  border: none;
  background-color: #191a1d;
  border-radius: 6px;
  font-family: Nunito;
  font-size: 15px;
  font-weight: 500;
  padding: 0 20px 0 40px;
  background-image: url("data:image/svg+xml;charset=UTF-8,%3csvg xmlns='http://www.w3.org/2000/svg' viewBox='0 0 56.966 56.966' fill='%23717790c7'%3e%3cpath d='M55.146 51.887L41.588 37.786A22.926 22.926 0 0046.984 23c0-12.682-10.318-23-23-23s-23 10.318-23 23 10.318 23 23 23c4.761 0 9.298-1.436 13.177-4.162l13.661 14.208c.571.593 1.339.92 2.162.92.779 0 1.518-.297 2.079-.837a3.004 3.004 0 00.083-4.242zM23.984 6c9.374 0 17 7.626 17 17s-7.626 17-17 17-17-7.626-17-17 7.626-17 17-17z'/%3e%3c/svg%3e");
  background-size: 14px;
  background-repeat: no-repeat;
  background-position: 16px 48%;
  color: #f9fafb;
}

.search-bar input::selection {
  color: var(--back);
  background: rgb(255, 255, 255);
}

.search-bar input::placeholder {
  font-family: Nunito;
  color: rgb(113 119 144 / 78%);
  font-size: 15px;
  font-weight: 500;
}

.search-bar input:focus {
  border: 3px solid var(--back);
  animation-name: searchbox;
  animation-duration: 0.3s;
  animation-fill-mode: both;
}

.switch {
  --line: #505162;
  --dot: #f7f8ff;
  --circle: #9ea0be;
  --duration: 0.3s;
  --text: #9ea0be;
  cursor: pointer;
}

.switch input {
  display: none;
}

.switch input + div {
  margin-top: -41px;
  margin-left: 280px;
  position: relative;
}

.switch input + div:before,
.switch input + div:after {
  --s: 1;
  content: "";
  position: absolute;
  height: 4px;
  top: 10px;
  width: 24px;
  background: var(--line);
  transform: scaleX(var(--s));
  transition: transform var(--duration) ease;
}

.switch input + div:before {
  --s: 0;
  left: 0;
  transform-origin: 0 50%;
  border-radius: 2px 0 0 2px;
}

.switch input + div:after {
  left: 28px;
  transform-origin: 100% 50%;
  border-radius: 0 2px 2px 0;
}

.switch input + div span {
  padding-left: 56px;
  line-height: 24px;
  color: var(--text);
}

.switch input + div span:before {
  --x: 0;
  --b: var(--circle);
  --s: 4px;
  content: "";
  position: absolute;
  left: 0;
  top: 1px;
  width: 20px;
  height: 20px;
  border-radius: 50%;
  box-shadow: inset 0 0 0 var(--s) var(--b);
  transform: translateX(var(--x));
  transition: box-shadow var(--duration) ease, transform var(--duration) ease;
}

.switch input + div span:not(:empty) {
  padding-left: 64px;
}

.switch input:checked + div:before {
  --s: 1;
}

.switch input:checked + div:after {
  --s: 0;
}

.switch input:checked + div span:before {
  --x: 28px;
  --s: 12px;
  --b: var(--dot);
}

.color {
  display: flex;
}

.repeatButton {
  height: 40px;
  width: 40px;
  filter: invert(100%) sepia(6%) saturate(0%) hue-rotate(115deg)
    brightness(108%) contrast(108%);
}

.repeatButton:hover {
  cursor: pointer;
}

.btnInput {
  display: flex;
  flex-direction: row-reverse;
  justify-content: space-evenly;
  align-items: center;
  margin-left: 146px;
}

@keyframes fadeOutUp {
  from {
    transform: translate3d(0, 0, 0);
  }

  to {
    transform: translate3d(0, -15px, 0);
    opacity: 1;
  }
}

@keyframes fadeInUp {
  from {
    transform: translate3d(0, -10px, 0);
  }

  to {
    transform: translate3d(0, 0, 0);
    opacity: 1;
  }
}

@keyframes fadeInDown {
  from {
    transform: translate3d(0, 10px, 0);
  }

  to {
    transform: translate3d(0, 0, 0);
    opacity: 1;
  }
}

@keyframes fadeInLeft {
  0% {
    transform: translate3d(10px, 0, 0);
  }

  100% {
    transform: translate3d(0, 0, 0);
    opacity: 1;
  }
}

@keyframes fadeIn {
  0% {
    opacity: 0;
  }

  100% {
    opacity: 1;
  }
}

@keyframes settings-spin {
  0% {
    -webkit-transform: rotate(0deg);
    transform: rotate(0deg);
  }

  10% {
    -webkit-transform: rotate(18deg);
    transform: rotate(18deg);
  }

  20% {
    -webkit-transform: rotate(36deg);
    transform: rotate(36deg);
  }

  80% {
    -webkit-transform: rotate(324deg);
    transform: rotate(324deg);
  }

  90% {
    -webkit-transform: rotate(342deg);
    transform: rotate(342deg);
  }

  100% {
    -webkit-transform: rotate(360deg);
    transform: rotate(360deg);
  }
}

@keyframes indicatorscaleY {
  0% {
    transform: scaleY(0);
  }

  100% {
    transform: scaleY(1);
  }
}

@keyframes shake {
  10%,
  90% {
    transform: translate3d(-1px, 0, 0);
  }

  20%,
  80% {
    transform: translate3d(2px, 0, 0);
  }

  30%,
  50%,
  70% {
    transform: translate3d(-2px, 0, 0);
  }

  40%,
  60% {
    transform: translate3d(2px, 0, 0);
  }
}

@keyframes gradient {
  0% {
    background-position: 0% 50%;
  }

  50% {
    background-position: 100% 50%;
  }

  100% {
    background-position: 0% 50%;
  }
}

@keyframes searchbox {
  0% {
    width: 0%;
  }

  100% {
    width: 100%;
  }
}

@keyframes settingsBtnAnimation {
  0% {
    border-bottom-width: 0px;
    border-bottom-style: none;
  }

  100% {
    border-bottom: var(--back);
    border-bottom-width: 3px;
    border-bottom-style: solid;
  }
}

@keyframes gameMenuAnimation {
  0% {
    width: 0px;
    opacity: 0.5;
  }

  100% {
    width: 30rem;
    opacity: 1;
  }
}

@keyframes closeOverlay {
  0% {
    height: 100%;
  }

  100% {
    height: 0%;
  }
}
</style>
