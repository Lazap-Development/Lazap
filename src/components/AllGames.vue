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
</template>

<script>
const fs = window.__TAURI__.fs;
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
        await fs.write(
          (await path.appDir()) +
            `storage/cache/games/banners/newcustombanner.png`,
          reader.result
        );
        document.getElementById(
          "addGameCustomBannerOutput"
        ).style.backgroundImage =
          `url(` +
          tauri.convertFileSrc(
            (await path.appDir()) +
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
    let newGameLocation;

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
            await fs.removeFile(
              (await path.appDir()) + `cache/games/banners/newcustombanner.png`
            );
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
            await fs.renameFile(
              (await path.appDir()) +
                `storage/cache/games/banners/newcustombanner.png`,
              (await path.appDir()) +
                `storage/cache/games/banners/${require("./modules/sha256.js").sha256(
                  document
                    .getElementById("inputGameName")
                    .value.replaceAll(" ", "_")
                )}.png`
            );
            this.$root.$refs.findGamesMod.Elements.createGameElement(
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
            this.$root.$refs.findGamesMod.Elements.createGameElement(
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
  background-color: var(--back);
  color: rgb(216, 216, 216);
  border-radius: 12px;
  font-family: Nunito-Bold;
}

.secondorybox .section .addGameFinalBtn:hover {
  cursor: pointer;
}
</style>
