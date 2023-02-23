<template>
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
          <button class="addGameFinalBtn" id="addGameFinalBtn">Add Game</button>
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
    <img id="addGameBtn" class="addGameBtn" src="../assets/svg/add.svg" />

    <div class="search-bar">
      <input type="text" placeholder="Search" id="gamesInput" />
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
        ).style.backgroundImage =
          `url(` +
          tauri.convertFileSrc(
            (await path.appDir()) + `cache/games/banners/newcustombanner.png`
          ) +
          `?${new Date().getSeconds()}` +
          `)`;
      };
      document.getElementById("addGameCustomBannerTxt").style.opacity = "0";
      reader.readAsArrayBuffer(selectedFile);
    },
  },
  async mounted() {
    let newGameLocation;
    let createGameElement = this.$root.$refs.findGamesMod.createGameElement;

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
              file_path:
                (await path.appDir()) +
                `cache/games/banners/newcustombanner.png`,
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
          let scheme = {
            DisplayName: document.getElementById("inputGameName").value,
            LauncherName: "CustomGame",
            GameID: "CustomGame",
            Executable: newGameLocation.split("\\").slice(-1)[0],
            Location: newGameLocation.split("\\").slice(0, -1).join("\\"),
            Args: [],
          };

          try {
            await invoke("rename_file", {
              from:
                (await path.appDir()) +
                `cache/games/banners/newcustombanner.png`,
              to:
                (await path.appDir()) +
                `cache/games/banners/${await invoke("sha256", {
                  content: document
                    .getElementById("inputGameName")
                    .value.replaceAll(" ", "_"),
                })}.png`,
            });
            createGameElement(scheme, "allGamesList");
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
            createGameElement(scheme, "allGamesList");
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
          document.getElementById("addGameCustomBannerTxt").style.opacity = "1";
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
.secondorybox .section .addGameFinalBtn {
  width: 170px;
  height: 30px;
  margin-left: 20px;
  outline: none;
  border: none;
  align-self: flex-start;
  margin-right: 20px;
  margin-top: 10px;
  background-color: var(--accentColor);
  color: rgb(216, 216, 216);
  border-radius: 12px;
  font-family: Nunito-Bold;
}

.secondorybox .section .addGameFinalBtn:hover {
  cursor: pointer;
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
  cursor: pointer;
}
</style>
