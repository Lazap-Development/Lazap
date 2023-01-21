<template>
  <div class="settings-background" id="settings-backblur"></div>
  <div class="settings" id="settings-popup">
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
            <input type="checkbox" id="setting-enable_rpc" />
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
              id="setting-launch_on_startup"
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
            <input
              type="checkbox"
              id="setting-skip_login"
              disabled="readonly"
            />
            <div>
              <span></span>
            </div>
          </label>
        </div>

        <div>
          <p>Minimize to Tray on Launch</p>
          <label class="switch">
            <input type="checkbox" id="setting-tray_min_launch" />
            <div>
              <span></span>
            </div>
          </label>
        </div>

        <div>
          <p>Minimize to Tray on Quit</p>
          <label class="switch">
            <input type="checkbox" id="setting-tray_min_quit" />
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
              id="setting-check_for_updates"
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
            <img class="repeatButton" src="../assets/icons/reset.png" />
          </div>
        </div>
      </div>

      <div class="settings-footer">v0.6.2 (Tauri Release)</div>
    </div>
  </div>
</template>

<script>
export default {
  name: "settings-comp",
  async mounted() {
    const settings = document.getElementById("settings-popup");
    const settingsbackblur = document.getElementById("settings-backblur");
    const path = window.__TAURI__.path;
    const invoke = window.__TAURI__.invoke;

    settingsbackblur.addEventListener("click", () => {
      settings.style.display = "none";
      settingsbackblur.style.display = "none";
    });

    document
      .getElementById("settings-btn")
      .addEventListener("click", async () => {
        settingsbackblur.style.display = "flex";
        settings.style.display = "flex";

        const Data = JSON.parse(
          await invoke("read_file", {
            filePath: (await path.appDir()) + "LauncherData.json",
          })
        );
        console.log(Data);
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

    document.getElementById("winerunnersbtn").addEventListener("click", () => {
      document.getElementById("general-settings").style.display = "none";
      document.getElementById("appearance-settings").style.display = "flex";
    });

    document
      .querySelector(".repeatButton")
      .addEventListener("click", async () => {
        let LauncherData = JSON.parse(
          await invoke("read_file", {
            filePath: (await path.appDir()) + "LauncherData.json",
          })
        );
        LauncherData["accentColor"] = "#7934FA";
        await invoke("write_file", {
          filePath: (await path.appDir()) + "LauncherData.json",
          fileContent: JSON.stringify(LauncherData),
        });
        document.querySelector("input[id=setting-accentColor]").value =
          LauncherData["accentColor"];
        updateAccentColor(LauncherData["accentColor"]);
      });

    document.querySelectorAll("input[id^=setting-]").forEach((input) => {
      if (input.id === "setting-accentColor") {
        input.addEventListener("input", async () => {
          const LauncherData = JSON.parse(
            await invoke("read_file", {
              filePath: (await path.appDir()) + "LauncherData.json",
            })
          );
          LauncherData[input.id.split("-")[1]] = document.querySelector(
            `input[id=${input.id}]`
          ).value;
          await invoke("write_file", {
            filePath: (await path.appDir()) + "LauncherData.json",
            fileContent: JSON.stringify(LauncherData),
          });

          updateAccentColor(LauncherData.accentColor);
        });
        return;
      }
      input.addEventListener("change", async () => {
        const LauncherData = JSON.parse(
          await invoke("read_file", {
            filePath: (await path.appDir()) + "LauncherData.json",
          })
        );
        LauncherData[input.id.split("-")[1]] = document.querySelector(
          `input[id=${input.id}]`
        ).checked;
        await invoke("write_file", {
          filePath: (await path.appDir()) + "LauncherData.json",
          fileContent: JSON.stringify(LauncherData),
        });

        if (input.id === "setting-enable_rpc") {
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
        }
      });
    });

    function updateAccentColor(accentColor) {
      document.getElementById("indicator").style.backgroundColor = accentColor;
      document.querySelector(":root").style.setProperty("--back", accentColor);
    }

    async function setActivity(tab) {
      let timestamp = null;
      const { state, details, largeImage, largeText, smallImage, smallText } =
        require("./modules/rpcOptions").selectOption(tab);
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
  },
};
</script>

<style>
.settings-background {
  position: absolute;
  width: 100%;
  height: 100%;
  z-index: 1999;
  background-color: #0f10136e;
  backdrop-filter: blur(10px);
  display: none;
  margin-top: -8px;
  margin-left: -8px;
  justify-content: center;
  align-content: center;
}

.settings {
  position: absolute;
  height: 500px;
  width: 600px;
  display: none;
  background: #181a1f;
  z-index: 2000;
  border-radius: 20px;
  text-shadow: 2px 4px 3px rgba(0, 0, 0, 0.3);
  box-shadow: 0 3px 16px -7px rgb(17 18 24 / 100%);

  top: 0;
  bottom: 0;
  left: 0;
  right: 0;

  margin: auto;
  justify-content: center;
}

.settings h1 {
  font-size: 26px;
  text-align: center;
  font-family: Nunito-Bold;
  font-weight: normal;
  color: rgb(201, 201, 201);
  margin-top: 45px;
  margin-bottom: 20px;
}

.settings p {
  font-size: 18px;
  position: relative;

  font-family: Nunito;
  color: rgb(201, 201, 201);

  text-decoration: none;
  text-shadow: none;
  cursor: default;
  z-index: -15;
}

.settings-info {
  position: absolute;
  color: rgb(141, 141, 141);

  font-size: 18px;
  margin-top: 8px;
}

.settings-footer {
  color: rgba(133, 133, 133, 0.446);
  position: absolute;
  bottom: 0;
  left: 0;
  margin-bottom: 14px;
  margin-left: 20px;
}

.choser {
  display: flex;
  justify-content: center;
  align-items: center;
  margin-bottom: 20px;
}

.choser button {
  font-family: Nunito;
  color: rgb(201, 201, 201);
  border-radius: 12px;
  text-decoration: none;
  text-shadow: none;
  font-size: 18px;

  background: #22242b;
  width: 180px;
  height: 40px;
  border: none;
  outline: none;
  padding: 0;
}

.choser button:hover {
  animation: settingsBtnAnimation 0.2s forwards;
  cursor: pointer;
}

.appearance-settings,
.general-settings {
  display: flex;
  flex-direction: column;
  align-items: center;
  margin-left: auto;
}

.appearance-settings {
  display: none;
}
</style>
