<template>
  <find-games ref="findGamesMod"></find-games>

  <settings-comp></settings-comp>
  <titlebar-comp></titlebar-comp>

  <div class="bg" id="bg">
    <leftbar-comp></leftbar-comp>

    <div class="homebox" id="home">
      <div class="centerchildren">
        <div class="children fadeInUp">
          <img
            class="head-pic"
            src="./assets/img/main_banner.png"
            id="head-pic"
          />
        </div>
        <div class="children fadeInDown">
          <div class="jump-back">
            <p>Recently Launched</p>
            <div
              id="recentGamesListMainPage"
              class="fadeInDown mainPageGamesList"
            >
              <div class="placeholderGames"></div>
              <div class="placeholderGames"></div>
              <div class="placeholderGames"></div>
              <div class="placeholderGames"></div>
              <div class="placeholderGames"></div>
            </div>
          </div>
        </div>
      </div>
      <div class="children fadeInLeft x2">
        <div class="rightbar">
          <p>System Specification</p>
          <div id="sysInfo" class="sysInfo">
            <div id="system_host">
              <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 448 512">
                <path
                  fill="currentColor"
                  d="M224 256A128 128 0 1 0 224 0a128 128 0 1 0 0 256zm-45.7 48C79.8 304 0 383.8 0 482.3C0 498.7 13.3 512 29.7 512H418.3c16.4 0 29.7-13.3 29.7-29.7C448 383.8 368.2 304 269.7 304H178.3z"
                />
              </svg>
            </div>
            <div id="system_name">
              <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 576 512">
                <path
                  fill="currentColor"
                  d="M64 0C28.7 0 0 28.7 0 64V352c0 35.3 28.7 64 64 64H240l-10.7 32H160c-17.7 0-32 14.3-32 32s14.3 32 32 32H416c17.7 0 32-14.3 32-32s-14.3-32-32-32H346.7L336 416H512c35.3 0 64-28.7 64-64V64c0-35.3-28.7-64-64-64H64zM512 64V288H64V64H512z"
                />
              </svg>
            </div>
            <div id="system_kernel">
              <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 576 512">
                <path
                  fill="currentColor"
                  d="M64 0C28.7 0 0 28.7 0 64V352c0 35.3 28.7 64 64 64H240l-10.7 32H160c-17.7 0-32 14.3-32 32s14.3 32 32 32H416c17.7 0 32-14.3 32-32s-14.3-32-32-32H346.7L336 416H512c35.3 0 64-28.7 64-64V64c0-35.3-28.7-64-64-64H64zM512 64V288H64V64H512z"
                />
              </svg>
            </div>
            <div id="disk">
              <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 512 512">
                <path
                  fill="currentColor"
                  d="M0 96C0 60.7 28.7 32 64 32H448c35.3 0 64 28.7 64 64V280.4c-17-15.2-39.4-24.4-64-24.4H64c-24.6 0-47 9.2-64 24.4V96zM64 288H448c35.3 0 64 28.7 64 64v64c0 35.3-28.7 64-64 64H64c-35.3 0-64-28.7-64-64V352c0-35.3 28.7-64 64-64zM320 416a32 32 0 1 0 0-64 32 32 0 1 0 0 64zm128-32a32 32 0 1 0 -64 0 32 32 0 1 0 64 0z"
                />
              </svg>
            </div>
            <div id="memory">
              <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 576 512">
                <path
                  fill="currentColor"
                  d="M64 64C28.7 64 0 92.7 0 128v7.4c0 6.8 4.4 12.6 10.1 16.3C23.3 160.3 32 175.1 32 192s-8.7 31.7-21.9 40.3C4.4 236 0 241.8 0 248.6V320H576V248.6c0-6.8-4.4-12.6-10.1-16.3C552.7 223.7 544 208.9 544 192s8.7-31.7 21.9-40.3c5.7-3.7 10.1-9.5 10.1-16.3V128c0-35.3-28.7-64-64-64H64zM576 352H0v64c0 17.7 14.3 32 32 32H80V416c0-8.8 7.2-16 16-16s16 7.2 16 16v32h96V416c0-8.8 7.2-16 16-16s16 7.2 16 16v32h96V416c0-8.8 7.2-16 16-16s16 7.2 16 16v32h96V416c0-8.8 7.2-16 16-16s16 7.2 16 16v32h48c17.7 0 32-14.3 32-32V352zM192 160v64c0 17.7-14.3 32-32 32s-32-14.3-32-32V160c0-17.7 14.3-32 32-32s32 14.3 32 32zm128 0v64c0 17.7-14.3 32-32 32s-32-14.3-32-32V160c0-17.7 14.3-32 32-32s32 14.3 32 32zm128 0v64c0 17.7-14.3 32-32 32s-32-14.3-32-32V160c0-17.7 14.3-32 32-32s32 14.3 32 32z"
                />
              </svg>
            </div>
            <div id="cpu">
              <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 512 512">
                <path
                  fill="currentColor"
                  d="M176 24c0-13.3-10.7-24-24-24s-24 10.7-24 24V64c-35.3 0-64 28.7-64 64H24c-13.3 0-24 10.7-24 24s10.7 24 24 24H64v56H24c-13.3 0-24 10.7-24 24s10.7 24 24 24H64v56H24c-13.3 0-24 10.7-24 24s10.7 24 24 24H64c0 35.3 28.7 64 64 64v40c0 13.3 10.7 24 24 24s24-10.7 24-24V448h56v40c0 13.3 10.7 24 24 24s24-10.7 24-24V448h56v40c0 13.3 10.7 24 24 24s24-10.7 24-24V448c35.3 0 64-28.7 64-64h40c13.3 0 24-10.7 24-24s-10.7-24-24-24H448V280h40c13.3 0 24-10.7 24-24s-10.7-24-24-24H448V176h40c13.3 0 24-10.7 24-24s-10.7-24-24-24H448c0-35.3-28.7-64-64-64V24c0-13.3-10.7-24-24-24s-24 10.7-24 24V64H280V24c0-13.3-10.7-24-24-24s-24 10.7-24 24V64H176V24zM160 128H352c17.7 0 32 14.3 32 32V352c0 17.7-14.3 32-32 32H160c-17.7 0-32-14.3-32-32V160c0-17.7 14.3-32 32-32zm192 32H160V352H352V160z"
                />
              </svg>
            </div>
          </div>
        </div>

        <player-comp></player-comp>
      </div>
    </div>

    <div class="secondorybox" id="recent">
      <p>Recently Played</p>
      <div id="recentGamesList" class="fadeInDown gamesList"></div>
      <h2 class="fade" id="recentGamesPlaceholder">
        You haven't launched any games recently
      </h2>
    </div>

    <allgames-comp></allgames-comp>

    <div class="secondorybox" id="favs">
      <p>Favourite Games</p>

      <div class="search-bar">
        <input type="text" placeholder="Search" id="favsInput" />
      </div>
      <div id="favGamesList" class="fadeInDown gamesList"></div>
      <h2 class="fade" id="favGamesPlaceholder">
        You currently have no game marked as favorite
      </h2>
    </div>

    <monitor-comp></monitor-comp>

    <div class="secondorybox" id="activity">
      <p>Overclock</p>
      <h1 class="fade">Coming Soon...</h1>
    </div>

    <div class="secondorybox" id="friends">
      <p>Benchmark</p>
      <h1 class="fade">Coming Soon...</h1>
    </div>

    <div class="gameMenu" id="gameMenu">
      <div class="gameMenuTitle" id="gameMenuTitle"></div>
      <!-- <div class="gameMenuBody"> -->
      <!-- <input
          type="text"
          class="launchOptions"
          id="launchOptions"
          placeholder="Launch Flags..."
        /> -->
      <button class="gameMenuBtn" id="removeGame">Remove Game</button>
      <!-- </div> -->
    </div>
  </div>
</template>

<script>
import Settings from "./components/Settings.vue";
import Titlebar from "./components/Titlebar.vue";
import AllGames from "./components/AllGames.vue";
import LeftBar from "./components/LeftBar.vue";
import findGames from "./components/find-games.vue";
import MonitorTab from "./components/MonitorTab.vue";
import MusicPlayer from "./components/MusicPlayer.vue";

const path = window.__TAURI__.path;
const invoke = window.__TAURI__.invoke;
const event = window.__TAURI__.event;

export default {
  name: "App",
  components: {
    "settings-comp": Settings,
    "titlebar-comp": Titlebar,
    "allgames-comp": AllGames,
    "leftbar-comp": LeftBar,
    "find-games": findGames,
    "monitor-comp": MonitorTab,
    "player-comp": MusicPlayer,
  },
  async mounted() {
    (async () => {
      disableContextMenu();
      const findGamesModule = this.$root.$refs.findGamesMod;

      const searchbars = document.querySelectorAll(
        'div.search-bar > input[type="text"]'
      );

      const sysInfoInvoke = JSON.parse(await invoke("get_sys_info"));
      Object.keys(sysInfoInvoke).forEach((elementId) => {
        let data = sysInfoInvoke[elementId];
        if (elementId === "cpu" && data.length > 22) {
          data = data.slice(0, 22) + "...";
        }
        document
          .getElementById(elementId)
          .insertAdjacentText("beforeend", data);
      });

      await findGamesModule
        .loadGames("recentGamesListMainPage")
        .catch((err) => {
          return console.error(err);
        });

      try {
        let { accentColor, backgroundColor, primaryColor, enable_blur } =
          JSON.parse(
            await invoke("read_file", {
              filePath: (await path.appDir()) + "LauncherData.json",
            })
          );
        if (!enable_blur) {
          document.getElementById;
          document.querySelectorAll("body").forEach(function (element) {
            var computedStyle = window.getComputedStyle(element);
            var backgroundColor = computedStyle.backgroundColor;

            if (
              backgroundColor &&
              backgroundColor !== "rgba(0, 0, 0, 0)" &&
              backgroundColor !== "transparent"
            ) {
              if (backgroundColor.includes("rgba")) {
                var rgbaComponents = backgroundColor.match(/\d+/g);
                var opacity = rgbaComponents[3].trim();

                if (opacity < 1) {
                  element.style.backgroundColor =
                    "rgba(" +
                    rgbaComponents[0] +
                    "," +
                    rgbaComponents[1] +
                    "," +
                    rgbaComponents[2] +
                    ", 1.0)";
                }
              } else if (backgroundColor.includes("rgb")) {
                element.style.backgroundColor = backgroundColor
                  .replace("rgb", "rgba")
                  .replace(")", ", 1.0)");
              }
            }
          });
        }
        if (!accentColor) accentColor = "#7934FA";
        // document.getElementById("indicator").style.backgroundColor =
        //   accent-color;
        if (backgroundColor)
          document
            .querySelector(":root")
            .style.setProperty("--all-color-back", backgroundColor);
        if (primaryColor)
          document
            .querySelector(":root")
            .style.setProperty("--all-color-primary", primaryColor);
        document
          .querySelector(":root")
          .style.setProperty("--accent-color", accentColor);
      } catch (error) {
        console.error(error);
      }

      event.listen("tauri://update-available", async () => {
        try {
          const data = JSON.parse(
            await invoke("read_file", {
              filePath: (await path.appDir()) + "LauncherData.json",
            })
          );
          if (data.check_for_updates === true) {
            document.getElementById("update-btn").style.display = "block";
          }
        } catch (e) {
          console.log(e);
        }
      });
      window.setInterval(checkForUpdate, 600_000);
      checkForUpdate();

      await invoke("show_window");

      await findGamesModule.loadGames("allGamesList").catch((err) => {
        return console.error(err);
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
        .getElementById("removeGame")
        .addEventListener("click", async function () {
          let data = JSON.parse(
            await invoke("read_file", {
              filePath: (await path.appDir()) + "cache/games/data.json",
            })
          );
          data = data.filter(
            (a) =>
              a.display_name !=
              document.getElementById("removeGame").parentNode.firstChild
                .innerHTML
          );
          await invoke("write_file", {
            filePath: (await path.appDir()) + "cache/games/data.json",
            fileContent: JSON.stringify(data),
          });
          document
            .getElementById(
              `game-div-${document
                .getElementById("removeGame")
                .parentNode.firstChild.innerHTML.replaceAll(" ", "_")}`
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
    })();
    function checkForUpdate() {
      window.__TAURI__.updater
        .checkUpdate()
        .then((res) => {
          console.log(res);
        })
        .catch(console.log);
    }
    function disableContextMenu() {
      if (window.location.hostname !== "tauri.localhost") {
        return;
      }

      document.addEventListener(
        "contextmenu",
        (e) => {
          e.preventDefault();
          return false;
        },
        { capture: true }
      );

      document.addEventListener(
        "selectstart",
        (e) => {
          e.preventDefault();
          return false;
        },
        { capture: true }
      );
    }
  },
};
</script>

<style>
:root {
  --accent-color: 121, 52, 250;
  --all-color-back: 20, 14, 36;
  --all-color-front: 42, 16, 81;
  --all-color-primary: 12, 11, 14;
}

::selection {
  color: inherit !important;
  background: rgba(var(--accent-color), 0.7) !important;
  border-radius: 10px;
}

p {
  cursor: default;
}

@font-face {
  font-family: Nunito;
  src: url("./assets/fonts/Nunito-Medium.ttf") format("truetype");
}

@font-face {
  font-family: Nunito-Bold;
  src: url("./assets/fonts/Nunito-SemiBold.ttf") format("truetype");
}

@font-face {
  font-family: Nunito-ExtraBold;
  src: url("./assets/fonts/Nunito-Bold.ttf") format("truetype");
}

html,
body {
  zoom: 0.945;
  background-color: rgba(var(--all-color-back), 0.1);
  font-family: Nunito;
  height: 100%;
  overflow: hidden;
  border-radius: 15px;
  margin: 0px;
}

#app {
  height: calc(100% - 70px);
}

.bg {
  display: flex;
  justify-content: center;
  align-items: flex-start;
  flex-direction: row;
  margin-top: 100px;
  height: 100%;
  margin: 12px;
  margin-top: 10px;
}

.mx-1 {
  margin-right: 0.25rem !important;
  margin-left: 0.25rem;
}

.homebox {
  display: flex;
  flex-direction: row;

  height: 100%;
  width: 100%;
}

.secondorybox {
  display: none;

  background-color: rgba(var(--all-color-primary), 0.7);
  border-radius: 20px;

  height: 100%;
  width: 100%;
  margin-left: 14px;
  cursor: default;
}

.secondorybox p {
  color: rgb(197, 197, 197);
  margin-left: 30px;
  margin-top: 30px;
  font-size: 20px;
  float: left;
  font-family: Nunito-ExtraBold;
}

.secondorybox h1 {
  position: absolute;
  left: 50%;
  top: 45%;
  font-weight: normal;
  color: rgba(164, 164, 164, 0.3);
  font-size: 34px;
  text-align: center;
}

.secondorybox h2 {
  position: absolute;
  left: 50%;
  top: 45%;
  font-weight: normal;
  color: rgba(98, 98, 98, 0.5);
  width: 310px;
  text-align: center;
  font-size: 26px;
}

.secondorybox .addGamePopUp {
  backdrop-filter: blur(10px);
  -webkit-backdrop-filter: blur(10px);
  position: absolute;
  width: 380px;
  height: 200px;
  margin-left: 170px;
  margin-top: 20px;
  background-color: rgba(var(--all-color-front), 0.7);
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
  background-color: rgba(var(--all-color-primary), 0.7);
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
  background-color: rgba(var(--all-color-primary), 0.7);
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
  background-color: rgba(var(--all-color-primary), 0.7);
  color: rgb(194, 194, 194);
  padding: 10px;
  border-radius: 12px;
  font-family: Nunito-Bold;
}

.secondorybox .section .addGameLocation:hover {
  scale: 1.06;
}

.centerchildren {
  display: flex;
  flex-direction: column;
  width: 100% !important;
}

.children {
  height: 100%;
  width: 100%;
  overflow: hidden;
  margin-left: 14px;
  display: flex;
}

.children:first-child {
  margin-bottom: 15px !important;
}

.head-pic {
  width: 100%;
  height: 100%;
  position: relative;
  object-fit: cover;
  border-radius: 20px;
  image-rendering: auto;
}

.jump-back {
  background-color: rgba(var(--all-color-primary), 0.7);
  width: 100%;
  height: 100% !important;
  border-radius: 20px;
}

.jump-back p {
  color: rgb(197, 197, 197);
  margin-left: 18px;
  margin-top: 18px;
  font-size: 18px;
  font-family: Nunito-ExtraBold;
  position: absolute;
}

.x2 {
  display: flex;
  flex-direction: column;
  margin-left: 27px;
  width: 40%;
}

.rightbar {
  background-color: rgba(var(--all-color-primary), 0.7);
  height: 40%;
  border-radius: 20px;
  cursor: default;
}

.rightbar:first-child {
  height: 60%;
  margin-bottom: 15px;
}

.rightbar p {
  color: rgb(197, 197, 197);
  margin-left: 18px;
  margin-top: 18px;
  font-size: 18px;
  font-family: Nunito-ExtraBold;
}

.rightbar .sysInfo {
  margin-top: 30px;
  display: flex;
  flex-direction: column;
  margin-left: 34px;
  margin-right: auto;
  width: 300px;
  font-size: 18px;
  color: rgb(197, 197, 197);
  font-family: Nunito-Bold;
}

.rightbar .sysInfo div {
  margin-bottom: 4px;
}

.rightbar .sysInfo svg {
  width: 16px;
  margin-right: 10px;
  transform: translate(0px, 3px);
  color: rgb(var(--accent-color));
}

.rightbar .stickers {
  display: flex;
  justify-content: center;
  margin-top: 290px;
}

.rightbar .stickers img {
  width: 100px;
}

.gamesList {
  position: absolute;
  overflow-y: scroll;
  width: 80%;
  height: 72%;
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
  border-radius: 0 0 10px 10px;
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

.gamebox .gamebox-icon {
  position: absolute;
  width: 32px;
  height: 32px;
  z-index: 4000;
  border-radius: 50px/50px;
  mask-image: none;
  display: block;
  top: 0;
  right: 0;
  left: inherit;
  margin: 5px;
}

.gamebox .game_banner_img {
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
  margin-top: -21%;
  width: 100%;
  height: 15%;
  position: absolute;
  background-color: rgba(var(--all-color-front), 1);
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
  margin: 20px;
  margin-left: 4%;
  align-items: center;
}

.placeholderGames {
  position: relative;

  text-align: center;
  display: inline-block;
  border-radius: 14px;
  transition: all 0.25s cubic-bezier(0.165, 0.74, 0.44, 1);
  height: 61%;
  width: 17%;
  box-shadow: 0 2px 10px rgba(0, 0, 0, 0.1);
  opacity: 0.5;
  background-color: rgba(var(--accent-color), 0.7);
  mask-image: -webkit-gradient(
    linear,
    right 90%,
    left top,
    from(rgba(0, 0, 0, 1)),
    to(rgba(0, 0, 0, 0))
  );
}

.placeholderGames:nth-last-child(-n + 4) {
  margin-left: 1.6%;
}

.mainPageGamebox {
  position: relative;
  image-rendering: auto;
  text-align: center;
  display: inline-block;
  border-radius: 14px;
  transition: all 0.25s cubic-bezier(0.165, 0.74, 0.44, 1);
  width: 16%;
  height: 61%;
  box-shadow: 0 2px 10px rgba(0, 0, 0, 0.1);
  opacity: 0.7;
  border: solid rgba(var(--accent-color), 1) 6px;
  background-position: 50% 40% !important;
  object-fit: cover;
}

.mainPageGamebox:nth-last-child(-n + 4) {
  margin-left: 1.6%;
}

.mainPageGamebox:hover {
  box-shadow: 0 2px 10px rgba(0, 0, 0, 0.5);
  cursor: pointer;
  opacity: 0.9;
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
  animation: fadeIn 0.3s;
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
  backdrop-filter: blur(10px);
  -webkit-backdrop-filter: blur(10px);
  right: 10px;
  height: 90%;
  background: linear-gradient(
    30deg,
    rgba(var(--all-color-primary), 0.7) 0%,
    rgba(var(--all-color-primary), 0.7) 30%,
    rgba(var(--accent-color), 0.7) 100%
  );
  border-bottom-right-radius: 10px;
  border-top-right-radius: 10px;
  border-top-left-radius: 20px;
  border-bottom-left-radius: 20px;
  animation: gameMenuAnimation 0.2s both ease-out;
  flex-direction: column;
  align-items: center;
}

.gameMenu .gameMenuBtn {
  font-family: Nunito;
  color: rgb(201, 201, 201);
  border-radius: 12px;
  font-size: 18px;

  background-color: rgba(var(--all-color-primary), 0.7);
  opacity: 0.9;
  width: 13.78rem;
  height: 50px;
  border: none;
  text-decoration: none;
  text-shadow: none;
  padding: 0;
  display: flex;
  justify-content: center;
  align-items: center;
}

.gameMenu .gameMenuBody {
  margin: 40px;
  display: flex;
  justify-content: center;
  align-items: center;
  flex-direction: column;
  width: 100%;
}

.gameMenu .gameMenuBody input[type="text"] {
  padding-left: 10px;
  padding-right: 10px;
  height: 30px;
  outline: none;
  border: none;
  background-color: rgba(var(--all-color-primary), 0.7);
  height: 40px;
  color: rgb(201, 201, 201);
  border-radius: 12px;
  font-size: 16px;
  margin-bottom: 10px;
}

.gameMenu .gameMenuBody input[type="text"]::placeholder {
  color: #ffffff5b;
}

.gameMenu .gameMenuBody .gameMenuBtn:first-child {
  margin-top: 100px;
}

.gameMenu .gameMenuTitle {
  font-size: 20px;
  color: #cacaca;
  text-align: center;
  margin-top: 40px;
  display: block;
  margin-bottom: 50px;
}

.gameMenu .gameMenuBtn:hover {
  scale: 1.05;
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
  -webkit-backdrop-filter: blur(6px);
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
  margin-top: 25px;
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
  background-color: rgba(var(--all-color-front), 0.7);
  border-radius: 10px;
  font-family: Nunito;
  font-size: 16px;
  font-weight: 500;
  padding: 0 20px 0 40px;
  background-image: url("data:image/svg+xml;charset=UTF-8,%3csvg xmlns='http://www.w3.org/2000/svg' viewBox='0 0 56.966 56.966' fill='%23717790c7'%3e%3cpath d='M55.146 51.887L41.588 37.786A22.926 22.926 0 0046.984 23c0-12.682-10.318-23-23-23s-23 10.318-23 23 10.318 23 23 23c4.761 0 9.298-1.436 13.177-4.162l13.661 14.208c.571.593 1.339.92 2.162.92.779 0 1.518-.297 2.079-.837a3.004 3.004 0 00.083-4.242zM23.984 6c9.374 0 17 7.626 17 17s-7.626 17-17 17-17-7.626-17-17 7.626-17 17-17z'/%3e%3c/svg%3e");
  background-size: 14px;
  background-repeat: no-repeat;
  background-position: 16px 48%;
  color: #f9fafb;
  border: 4px solid rgba(255, 255, 255, 0);
}

.search-bar input::selection {
  color: rgba(var(--all-color-back), 0.7);
  background: rgb(255, 255, 255);
}

.search-bar input::placeholder {
  font-family: Nunito;
  color: rgb(113 119 144 / 78%);
  font-size: 16px;
  font-family: Nunito-ExtraBold;
}

.search-bar input:focus {
  border: 4px solid rgba(var(--accent-color), 1);
  animation-name: searchbox;
  animation-duration: 0.3s;
  animation-fill-mode: both;
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
    opacity: 0;
  }

  100% {
    opacity: 1;
    width: 100%;
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
</style>
