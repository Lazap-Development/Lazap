<template>
  <div class="settings-background fadeInDownNoDelay" id="settings-backblur"></div>
  <div class="settings fadeInDown" id="settings-popup">
    <div>
      <h1>
        Settings
      </h1>

      <div>
        <p>Discord Rich Presence</p>
        <label class="switch">
          <input type="checkbox" id="setting-enableRPC">
          <div>
            <span></span>
          </div>
        </label>
      </div>

      <div>
        <p>Launch on Startup</p>
        <label class="switch">
          <input type="checkbox" id="setting-launchOnStartup" disabled="readonly">
          <div>
            <span></span>
          </div>
        </label>
      </div>

      <div>
        <p>Skip Login Menu</p>
        <label class="switch">
          <input type="checkbox" id="setting-skipLogin" disabled="readonly">
          <div>
            <span></span>
          </div>
        </label>
      </div>

      <div>
        <p>Minimize to Tray on Launch</p>
        <label class="switch">
          <input type="checkbox" id="setting-trayMinLaunch">
          <div>
            <span></span>
          </div>
        </label>
      </div>

      <div>
        <p>Minimize to Tray on Quit</p>
        <label class="switch">
          <input type="checkbox" id="setting-trayMinQuit">
          <div>
            <span></span>
          </div>
        </label>
      </div>

      <div>
        <p>Check for Updates</p>
        <label class="switch">
          <input type="checkbox" id="setting-checkForUpdates" disabled="readonly">
          <div>
            <span></span>
          </div>
        </label>
      </div>

      <div class="settings-footer">
        v0.4 (Tauri Release)
      </div>
    </div>
  </div>

  <div data-tauri-drag-region class="titlebar">
    <div data-tauri-drag-region style="justify-content: space-between;" class="titlebar-icons">
      <div class="titlebar-options">
        <img class="titlebar-settings" src="./assets/settings.svg" id="settings-btn">
        <img class="titlebar-account" src="./assets/account.svg" id="account-btn">
        <img class="titlebar-update" src="./assets/download.svg" id="update-btn">
      </div>
      <div data-tauri-drag-region style="margin-top: 0px;" class="titlebar-icons">
        <div @click="min_window" class="titlebar-min mx-1"></div>
        <div @click="max_window" class="titlebar-max mx-1"></div>
        <div @click="close_window" class="titlebar-exit mx-1"></div>
      </div>
    </div>
  </div>

  <div id="main-loading-overlay">
    <div class="spinner-content">
      <img src="./assets/spinner.svg" alt="Loading..." class="loading-spinner">
      <p class="fadeInDown">Loading...</p>
    </div>
  </div>

  <div class="bg" id="bg">
    <div class="homebox" id="home">
      <div class="children fadeInUp">
        <img class="head-pic" src="./assets/main-banner.png">
      </div>
      <div class="children fadeInDown">
        <div class="jump-back">
          <p>Recently Launched</p>
          <div id="recentGamesListMainPage" class="fadeInDown mainPageGamesList"></div>
        </div>
      </div>
      <div class="children fadeInLeft">
        <div class="online-friends">
          <p>Online Friends</p>
          <div class="CMS">Coming Soon...</div>
        </div>
      </div>
    </div>

    <div class="secondorybox" id="recent">
      <p>Recently Played</p>
      <div id="recentGamesList" class="fadeInDown gamesList">
      </div>
    </div>

    <div class="secondorybox" id="games">
      <div id="game-loading-overlay" class="game-loading-overlay">
        <div class="spinner-content">
          <img src="./assets/spinner.svg" alt="Loading..." class="loading-spinner">
          <p class="fadeInDown">Loading your games...</p>
        </div>
      </div>
      <p>All Games</p>
      <div class="search-bar">
        <input type="text" placeholder="Search" id="gamesInput">
      </div>
      <div id="allGamesList" class="fadeInDown gamesList">
      </div>
    </div>

    <div class="secondorybox" id="favs">
      <p>Favourite Games</p>
      <div class="search-bar">
        <input type="text" placeholder="Search" id="favsInput">
      </div>
      <div id="favGamesList" class="fadeInDown gamesList">
      </div>
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

    <div class="leftbar">
      <div class="d-flex justify-content-center">
        <div>
          <div class="user-pfp">
            <label for="file"></label>
            <input id="file" type="file" accept="image/png" @change="(event) => loadFile(event)" />
            <img src="./assets/default-profile.svg" alt="Avatar" width="85" height="85" id="output">
          </div>
          <input class="username" id="text" type="text" value="Lazap" spellcheck="false" maxlength="12" />
        </div>
      </div>

      <div class="side-tabs">
        <div class="d-flex justify-content-center pb-2">
          <div class="side-tab" id="home-btn">
            <div id="indicator"></div>
            <img src="./assets/home.svg" height="25" width="25">
            <div class="side-tab-text">
              Home
            </div>
          </div>
        </div>

        <div class="category-name d-flex justify-content-left">
          Games
        </div>
        <div class="d-flex justify-content-center">
          <div class="side-tab" id="recent-btn">
            <img id="recently-btn-img" src="./assets/recent.svg" height="25" width="25">
            <div class="side-tab-text">
              Recent
            </div>
          </div>
        </div>
        <div class="d-flex justify-content-center">
          <div class="side-tab" id="games-btn">
            <img id="games-btn-img" src="./assets/games.svg" height="25" width="25">
            <div class="side-tab-text">
              All Games
            </div>
          </div>
        </div>
        <div class="d-flex justify-content-center pb-2">
          <div class="side-tab" id="favs-btn">
            <img id="favs-btn-img" src="./assets/favs.svg" height="25" width="25">
            <div class="side-tab-text">
              Favourites
            </div>
          </div>
        </div>



        <div class="category-name d-flex justify-content-start">
          Friends
        </div>
        <div class="d-flex justify-content-center">
          <div class="side-tab" id="messages-btn">
            <img src="./assets/messages.svg" height="25" width="25">
            <div class="side-tab-text">
              Messages
            </div>
          </div>
        </div>
        <div class="d-flex justify-content-center">
          <div class="side-tab" id="activity-btn">
            <img src="./assets/activity.svg" height="25" width="25">
            <div class="side-tab-text">
              Activity
            </div>
          </div>
        </div>
        <div class="d-flex justify-content-center">
          <div class="side-tab" id="friends-btn">
            <img src="./assets/friends.svg" height="25" width="25">
            <div class="side-tab-text">
              All Friends
            </div>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<script>
import listeners from './components/listeners.vue'
import storage from './components/storage.vue'

export default {
  async created() {
    const os = window.__TAURI__.os;
    console.info(`OS: ${await os.platform()} | Arch: ${await os.arch()}`);

    if (await os.platform() === "win32") {
      document.getElementById('bg').style.height = '111.5vh';
      document.getElementById('main-loading-overlay').style.height = '111.5vh';
      document.getElementById('main-loading-overlay').style.width = '111.7vw';
    }
  },
  listeners,
  storage,
  methods: {
    min_window() {
      window.__TAURI__.window.appWindow.minimize()
    },
    max_window() {
      window.__TAURI__.window.appWindow.toggleMaximize()
    },
    close_window() {
      window.__TAURI__.window.appWindow.close()
    },
    async loadFile(event) {
      var selectedFile = event.target.files[0];
      var reader = new FileReader();

      reader.onload = async function () {
        await window.__TAURI__.fs.writeBinaryFile(await window.__TAURI__.path.appDir() + `storage/Cache/User/pfp.png`, reader.result);
        document.getElementById("output").src = window.__TAURI__.tauri.convertFileSrc(await window.__TAURI__.path.appDir() + `storage/Cache/User/pfp.png`) + `?${new Date().getSeconds()}`;
      };
      reader.readAsArrayBuffer(selectedFile);
    }
  }
};
</script>


 <style>
 @import './css/default.css';
 </style>
