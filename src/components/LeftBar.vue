<template>
  <div class="leftbar">
    <div class="leftbar-top">
      <div class="user-pfp">
        <label for="file"></label>
        <input
          id="file"
          type="file"
          accept="image/png"
          @change="(event) => loadPFP(event)"
        />
        <img
          src="../assets/svg/default-profile.svg"
          alt="Avatar"
          width="88"
          id="output"
        />
      </div>
      <input
        autocomplete="off"
        class="username"
        id="text"
        type="text"
        value="Lazap"
        spellcheck="false"
        maxlength="20"
      />
    </div>

    <div class="side-tabs">
      <div class="d-flex justify-content-center pb-2">
        <div class="side-tab" id="home-btn">
          <div class="indicator" id="indicator"></div>
          <img src="../assets/svg/home.svg" height="25" width="25" />
          <div class="side-tab-text">Home</div>
        </div>
      </div>

      <div class="category-name d-flex justify-content-left">Games</div>
      <div class="d-flex justify-content-center">
        <div class="side-tab" id="recent-btn">
          <img
            id="recently-btn-img"
            src="../assets/svg/recent.svg"
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
            src="../assets/svg/games.svg"
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
            src="../assets/svg/favs.svg"
            height="25"
            width="25"
          />
          <div class="side-tab-text">Favourites</div>
        </div>
      </div>

      <div class="category-name d-flex justify-content-start">Tweaks</div>
      <div class="d-flex justify-content-center">
        <div class="side-tab" id="monitor-btn">
          <img src="../assets/svg/wave-square.svg" height="25" width="25" />
          <div class="side-tab-text">Monitor</div>
        </div>
      </div>
      <div class="d-flex justify-content-center">
        <div class="side-tab" id="overclock-btn">
          <img src="../assets/svg/overclock.svg" height="25" width="25" />
          <div class="side-tab-text">Overclock</div>
        </div>
      </div>
      <div class="d-flex justify-content-center">
        <div class="side-tab" id="benchmark-btn">
          <img src="../assets/svg/chart-line.svg" height="25" width="25" />
          <div class="side-tab-text">Benchmark</div>
        </div>
      </div>
    </div>
  </div>
</template>

<script>
import { selectOption } from "./misc/rpcOptions.js";

const invoke = window.__TAURI__.invoke;
const path = window.__TAURI__.path;

export default {
  name: "leftbar-comp",
  methods: {
    async loadPFP(event) {
      let selectedFile = event.target.files[0];
      if (selectedFile) {
        let reader = new FileReader();

        reader.onload = async function () {
          await invoke("write_binary_file", {
            filePath: (await path.appDir()) + `cache/user/pfp.png`,
            fileContent: [...new Uint8Array(reader.result)],
          });
          document.getElementById("output").src =
            window.__TAURI__.tauri.convertFileSrc(
              (await path.appDir()) + `cache/user/pfp.png`
            ) + `?${new Date().getSeconds()}`;
        };
        reader.readAsArrayBuffer(selectedFile);
      }
    },
  },
  async mounted() {
    let timestamp = null;
    const findGamesModule = this.$root.$refs.findGamesMod;

    const data = JSON.parse(
      await invoke("read_file", {
        filePath: (await path.appDir()) + "cache/user/data.json",
      })
    );
    document.getElementById("text").value = data.username;

    if (
      await invoke("d_f_exists", {
        path: (await path.appDir()) + "cache/user/pfp.png",
      })
    ) {
      document.getElementById("output").src =
        window.__TAURI__.tauri.convertFileSrc(
          (await path.appDir()) + "cache/user/pfp.png"
        ) + `?${new Date().getSeconds()}`;
    }

    const buttonConfigs = [
      {
        displayId: "home",
        gamesListId: "recentGamesListMainPage",
        focusId: null,
        placeholderId: null,
      },
      {
        displayId: "recent",
        gamesListId: "recentGamesList",
        focusId: null,
        placeholderId: "recentGamesPlaceholder",
      },
      {
        displayId: "games",
        gamesListId: "allGamesList",
        focusId: "gamesInput",
        placeholderId: null,
      },
      {
        displayId: "favs",
        gamesListId: "favGamesList",
        focusId: "favsInput",
        placeholderId: "favGamesPlaceholder",
      },
      {
        displayId: "monitor",
        gamesListId: null,
        focusId: null,
        placeholderId: null,
      },
      {
        displayId: "overclock",
        gamesListId: null,
        focusId: null,
        placeholderId: null,
      },
      {
        displayId: "benchmark",
        gamesListId: null,
        focusId: null,
        placeholderId: null,
      },
    ];

    buttonConfigs.forEach(
      ({ displayId, gamesListId, focusId, placeholderId }) => {
        document
          .getElementById(displayId + "-btn")
          .addEventListener("click", async function () {
            this.appendChild(document.getElementById("indicator"));
            if (document.getElementById(displayId).style.display !== "flex") {
              if (focusId) {
                setTimeout(() => document.getElementById(focusId).focus(), 100);
              }
              toggleIndicatorAnim();
            }

            switchDisplay(displayId);

            if (gamesListId) {
              await findGamesModule.loadGames(gamesListId).catch(console.error);

              if (
                placeholderId &&
                document.getElementById(gamesListId).childNodes.length > 0
              ) {
                document.getElementById(placeholderId).style.display = "none";
              }
            }

            setActivity(displayId);
          });
      }
    );

    function switchDisplay(activeSection) {
      buttonConfigs
        .map((item) => ({ displayId: item.displayId }))
        .forEach((section) => {
          document.getElementById(section.displayId).style.display =
            section.displayId === activeSection ? "flex" : "none";
        });
    }

    async function setActivity(tab) {
      const { details, largeText, smallImage, smallText } = selectOption(tab);
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
    function toggleIndicatorAnim() {
      let indicator = document.getElementById("indicator");
      indicator.classList.add("anim-indicatorscaleY");
      setTimeout(() => {
        indicator.classList.remove("anim-indicatorscaleY");
      }, 200);
    }
  },
};
</script>

<style>
.leftbar {
  background-color: rgba(var(--all-color-primary), 0.7);
  border-radius: 20px;
  width: 280px;
  height: 100%;
  text-align: center;
  box-shadow: 0 3px 16px -7px rgb(17 18 24 / 70%);
}

.leftbar-top {
  display: flex;
  align-items: center;
  flex-direction: column;
  height: 200px;
  justify-content: center;
  margin-top: 15px;
}

.user-pfp {
  width: 88px;
  height: 88px;
  transition: all 0.2s ease-in-out;
  border-radius: 50%;
  border: 5px solid rgba(var(--accent-color), 1);
  box-shadow: 0 3px 16px -7px rgb(17 18 24 / 70%);
  overflow: hidden;
}

.user-pfp:hover {
  cursor: pointer;
  border-radius: 30%;
}

.user-pfp input {
  display: none;
}

.user-pfp img {
  image-rendering: auto;
  width: 100%;
  height: 100%;
  width: 88px;
  height: 88px;
  object-fit: cover;
  background-color: rgb(var(--accent-color));
}

.user-pfp label {
  position: absolute;
  cursor: pointer;
  width: 88px;
  height: 88px;
}

.username {
  margin-top: 8px;
  text-align: center;
  font-size: 22px;
  font-family: Nunito-Bold;
  color: #b9b9b9;
  background-color: transparent;
  border: transparent;
  text-decoration: none;
}

/* .side-tabs {
  height: 420px;
  width: 100%;
  overflow-y: auto;
  overflow-x: hidden;
} */

.side-tab {
  box-shadow: -1px 3px 8px -1px rgba(0, 0, 0, 0.2);
  height: 40px;
  width: 235px;
  border-radius: 10px;
  display: flex;
  margin-bottom: 10px;
  z-index: 1;
  transition: color 0.15s ease-out;
}

.side-tab:before {
  background: linear-gradient(
    30deg,
    rgba(var(--all-color-primary), 0.3) 0%,
    rgba(var(--all-color-primary), 0.3) 30%,
    rgba(var(--accent-color), 0.7) 100%
  );
  content: "";
  height: 40px;
  width: 235px;
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
  font-family: Nunito-Bold;
}

.category-name {
  color: #7a7a7a;
  margin-left: 28px;
  margin-bottom: 4px;
  font-size: 15px;
  cursor: default !important;
}

.indicator {
  transition: 0.3s;
  position: absolute;
  margin-top: 5px;
  margin-left: 2px;

  background-color: rgba(var(--accent-color), 0.7);
  height: 32px;
  width: 5px;
  border-radius: 50px;
  display: inline;
}

.anim-indicatorscaleY {
  animation: indicatorscaleY 0.2s linear;
}
</style>
