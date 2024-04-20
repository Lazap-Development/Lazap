<template>
  <div class="secondorybox" id="games">
    <div class="secondary-top">

      <div class="secondary-main">
        <p>All Games</p>
        <img id="addGameBtn" class="addGameBtn" src="../assets/svg/add.svg" />
        <div class="addGamePopUp" id="addGamePopUp">
          <div class="mainSection fadeInDown">
            <div class="section">
              <div class="title">Game Name</div>
              <input maxlength="24" type="text" class="inputGameName" id="inputGameName" />
            </div>
            <div class="section">
              <button class="addGameLocation" id="addGameLocation">
                Locate Game
              </button>
            </div>
            <div class="section">
              <button class="addGameFinalBtn" id="addGameFinalBtn">Add Game</button>
            </div>
          </div>
          <div class="addGameBannerSection fadeInUp">
            <label for="addGameCustomBanner"></label>
            <input class="banner" id="addGameCustomBanner" type="file" accept="image/png"
              @change="(event) => loadCustomBanner(event)" />
            <p id="addGameCustomBannerTxt">Hover & Click to Select Banner</p>
            <div id="addGameCustomBannerOutput" class="addGameCustomBannerOutput"></div>
          </div>
        </div>
      </div>


      <div class="search-bar">
        <input type="text" placeholder="Search" id="gamesInput" />
      </div>
    </div>
    <div id="allGamesList" class="fadeInDown gamesList"></div>
  </div>
</template>

<script>
const invoke = window.__TAURI__.invoke;
const dialog = window.__TAURI__.dialog;
const path = window.__TAURI__.path;
const tauri = window.__TAURI__.tauri;

export default {
  name: "allgames-comp",
  methods: {
    async loadCustomBanner(event) {
      let selectedFile = event.target.files[0];
      let reader = new FileReader();

      reader.onload = async function () {
        await invoke("write_binary_file", {
          filePath:
            (await path.appDir()) + `cache/games/banners/newcustombanner.png`,
          fileContent: [...new Uint8Array(reader.result)],
        });
        document.getElementById(
          "addGameCustomBannerOutput"
        ).style.backgroundImage = `url(${tauri.convertFileSrc(
          (await path.appDir()) + "cache/games/banners/newcustombanner.png"
        )}?${new Date().getSeconds()})`;
      };
      document.getElementById("addGameCustomBannerTxt").style.opacity = "0";
      reader.readAsArrayBuffer(selectedFile);
    },
  },
  async mounted() {
    const appDir = await path.appDir();
    let newGameLocation;
    let loadGames = this.$root.$refs.findGamesMod.loadGames;

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
          document.getElementById("addGameCustomBannerTxt").style.opacity = "1";
          try {
            await invoke("remove_file", {
              file_path: `${appDir}cache/games/banners/newcustombanner.png`,
            });
          } catch (e) {
            return e;
          }
        } else {
          document.getElementById("addGamePopUp").style.display = "flex";
          document.getElementById("inputGameName").focus();
        }
      });

    document
      .getElementById("addGameFinalBtn")
      .addEventListener("click", async function () {
        if (
          document.getElementById("inputGameName").value.trim().length > 0 &&
          newGameLocation
        ) {
          await invoke("add_custom_game", {
            location: newGameLocation,
            displayName: document.getElementById("inputGameName").value,
          });
          loadGames("allGamesList");
          document.getElementById(
            "addGameCustomBannerOutput"
          ).style.backgroundImage = "url()";
          document.getElementById("addGamePopUp").style.display = "none";
          document.getElementById("inputGameName").value = "";
          document.getElementById("addGameCustomBannerTxt").style.opacity = "1";
          newGameLocation = "";
        } else if (
          document.getElementById("inputGameName").value.trim().length > 0 &&
          !newGameLocation
        ) {
          return alert("You are missing the game location.");
        } else if (
          document.getElementById("inputGameName").value.length < 1 &&
          newGameLocation
        ) {
          return alert("You are missing the game name.");
        } else {
          return alert("You are missing the game name, and the game location.");
        }
      });

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
  },
};
</script>

<style>
.secondary-main {
  display: flex;
  position: relative
}

.secondorybox .addGameBtn {
  width: 15px;
  height: 15px;
  transition: 0.1s all linear;
  color: rgb(255, 255, 255);
  margin: auto 20px;
}

.secondorybox .addGameBtn:hover {
  cursor: pointer;
}


.secondorybox .addGamePopUp {
  backdrop-filter: blur(10px);
  -webkit-backdrop-filter: blur(10px);
  position: absolute;
  width: 380px;
  height: 200px;
  margin-left: 100px;
  margin-top: 60px;
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


.secondorybox .section .addGameFinalBtn {
  width: 170px;
  height: 30px;
  margin-left: 20px;
  outline: none;
  border: none;
  align-self: flex-start;
  margin-right: 20px;
  margin-top: 10px;
  background-color: rgba(var(--accent-color), 1);
  color: rgb(255, 255, 255);
  border-radius: 12px;
  font-family: Nunito-ExtraBold;
  font-weight: 900;
}

.secondorybox .section .addGameFinalBtn:hover {
  cursor: pointer;
}
</style>
