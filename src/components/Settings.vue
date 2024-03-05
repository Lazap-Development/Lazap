<template>
  <div class="settings-background" id="settings-backblur"></div>
  <div class="settings" id="settings-popup">
    <div>
      <h1>Settings</h1>
      <div class="choser" id="choser">
        <button id="generalbtn">General</button>
        <button id="appearancebtn">Appearance</button>
        <button id="addonsbtn">Addons</button>
      </div>
      <div class="general-settings" id="general-settings">
        <div class="setting">
          <p>Launch on Startup</p>
          <div class="btnInput">
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
        </div>

        <div class="setting">
          <p>Skip Login Menu</p>
          <div class="btnInput">
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
        </div>

        <div class="setting">
          <p>Minimize to Tray on Launch</p>
          <div class="btnInput">
            <label class="switch">
              <input type="checkbox" id="setting-tray_min_launch" />
              <div>
                <span></span>
              </div>
            </label>
          </div>
        </div>

        <div class="setting">
          <p>Minimize to Tray on Quit</p>
          <div class="btnInput">
            <label class="switch">
              <input type="checkbox" id="setting-tray_min_quit" />
              <div>
                <span></span>
              </div>
            </label>
          </div>
        </div>

        <div class="setting">
          <p>Check for Updates</p>
          <div class="btnInput">
            <label class="switch">
              <input type="checkbox" id="setting-check_for_updates" />
              <div>
                <span></span>
              </div>
            </label>
          </div>
        </div>
      </div>

      <div class="appearance-settings" id="appearance-settings">
        <div class="setting themes">
          <label class="color">
            <div
              class="theme-box"
              id="theme-box-default"
              style="background-color: #000"
            ></div>
            <div
              class="theme-box"
              id="theme-box-crimson"
              style="background-color: #660708"
            ></div>
            <div
              class="theme-box"
              id="theme-box-midnight"
              style="background-color: #240046"
            ></div>
          </label>
        </div>
        <div class="setting">
          <p>Primary Color</p>
          <div class="btnInput">
            <label class="color">
              <input type="color" id="setting-primaryColor" />
            </label>
            <img class="repeatButton" src="../assets/icons/reset.png" />
          </div>
        </div>
        <div class="setting">
          <p>Background Color</p>
          <div class="btnInput">
            <label class="color">
              <input type="color" id="setting-backgroundColor" />
            </label>
            <img class="repeatButton" src="../assets/icons/reset.png" />
          </div>
        </div>
        <div class="setting">
          <p>Front Color</p>
          <div class="btnInput">
            <label class="color">
              <input type="color" id="setting-frontColor" />
            </label>
            <img class="repeatButton" src="../assets/icons/reset.png" />
          </div>
        </div>
        <div class="setting">
          <p>Accent Color</p>
          <div class="btnInput">
            <label class="color">
              <input type="color" id="setting-accentColor" />
            </label>
            <img class="repeatButton" src="../assets/icons/reset.png" />
          </div>
        </div>
        <div class="setting">
          <p>Enable Launcher Icons</p>
          <div class="btnInput">
            <label class="switch">
              <input type="checkbox" id="setting-enableLauncherIcons" />
              <div>
                <span></span>
              </div>
            </label>
          </div>
        </div>
      </div>

      <div class="general-settings" id="addons-settings" style="display: none">
        <div class="setting">
          <p>Discord Rich Presence</p>
          <div class="btnInput">
            <label class="switch">
              <input type="checkbox" id="setting-enable_rpc" />
              <div>
                <span></span>
              </div>
            </label>
          </div>
        </div>

        <div class="setting">
          <p>Spotify Connection</p>
          <div class="btnInput">
            <label class="switch">
              <input type="checkbox" id="setting-enable_spotify" />
              <div>
                <span></span>
              </div>
            </label>
          </div>
        </div>

        <div class="setting">
          <p>YouTube Music Connection</p>
          <div class="btnInput">
            <label class="switch">
              <input
                type="checkbox"
                id="setting-enable_ytm"
                disabled="readonly"
              />
              <div>
                <span></span>
              </div>
            </label>
          </div>
        </div>

        <div class="setting">
          <p>Lazap Overlay [BETA]</p>
          <div class="btnInput">
            <label class="switch">
              <input type="checkbox" id="setting-enable_overlay" />
              <div>
                <span></span>
              </div>
            </label>
          </div>
        </div>
      </div>

      <div class="settings-footer">Release v0.8.0</div>
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
        document.querySelectorAll("input[id^=setting-]").forEach((input) => {
          if (input.id.startsWith("setting") && input.id.endsWith("Color"))
            input.value = Data[input.id.split("-")[1]];
          else input.checked = Data[input.id.split("-")[1]] ? true : false;
        });
      });

    document.getElementById("generalbtn").addEventListener("click", () => {
      document.getElementById("general-settings").style.display = "flex";
      document.getElementById("appearance-settings").style.display = "none";
      document.getElementById("addons-settings").style.display = "none";
    });

    document.getElementById("appearancebtn").addEventListener("click", () => {
      document.getElementById("general-settings").style.display = "none";
      document.getElementById("appearance-settings").style.display = "flex";
      document.getElementById("addons-settings").style.display = "none";
    });

    document.getElementById("addonsbtn").addEventListener("click", () => {
      document.getElementById("general-settings").style.display = "none";
      document.getElementById("appearance-settings").style.display = "none";
      document.getElementById("addons-settings").style.display = "flex";
    });

    let LauncherData = JSON.parse(
      await invoke("read_file", {
        filePath: (await path.appDir()) + "LauncherData.json",
      })
    );

    const repeats = document.getElementsByClassName("repeatButton");
    const defaults = {
      primaryColor: "12, 11, 14",
      backgroundColor: "20, 14, 36",
      frontColor: "42, 16, 81",
      accentColor: "121, 52, 250",
    };
    for (let i = 0; i < repeats.length; i++) {
      const id = repeats
        .item(i)
        .parentElement.children.item(0)
        .children.item(0)
        .id.split("-")[1];
      repeats.item(i).addEventListener("click", async () => {
        LauncherData[id] = defaults[id];
        await invoke("write_file", {
          filePath: (await path.appDir()) + "LauncherData.json",
          fileContent: JSON.stringify(LauncherData),
        });
        document.getElementById(`setting-${id}`).value = LauncherData[id];
        updateColor(id, LauncherData[id]);
      });
    }

    document.querySelectorAll("input[id^=setting-]").forEach((input) => {
      if (input.id === "setting-accentColor") {
        input.addEventListener("input", async () => {
          let hexValue = document.querySelector(`input[id=${input.id}]`).value;
          LauncherData[input.id.split("-")[1]] = `${parseInt(
            hexValue.substr(1, 2),
            16
          )}, ${parseInt(hexValue.substr(3, 2), 16)},${parseInt(
            hexValue.substr(5, 2),
            16
          )}`;

          await invoke("write_file", {
            filePath: (await path.appDir()) + "LauncherData.json",
            fileContent: JSON.stringify(LauncherData),
          });

          updateColor("accentColor", LauncherData.accentColor);
        });
        return;
      } else if (input.id === "setting-backgroundColor") {
        input.addEventListener("input", async () => {
          let hexValue = document.querySelector(`input[id=${input.id}]`).value;
          LauncherData[input.id.split("-")[1]] = `${parseInt(
            hexValue.substr(1, 2),
            16
          )}, ${parseInt(hexValue.substr(3, 2), 16)},${parseInt(
            hexValue.substr(5, 2),
            16
          )}`;

          await invoke("write_file", {
            filePath: (await path.appDir()) + "LauncherData.json",
            fileContent: JSON.stringify(LauncherData),
          });

          updateColor("backgroundColor", LauncherData.backgroundColor);
        });
        return;
      } else if (input.id === "setting-frontColor") {
        input.addEventListener("input", async () => {
          let hexValue = document.querySelector(`input[id=${input.id}]`).value;
          LauncherData[input.id.split("-")[1]] = `${parseInt(
            hexValue.substr(1, 2),
            16
          )}, ${parseInt(hexValue.substr(3, 2), 16)},${parseInt(
            hexValue.substr(5, 2),
            16
          )}`;

          await invoke("write_file", {
            filePath: (await path.appDir()) + "LauncherData.json",
            fileContent: JSON.stringify(LauncherData),
          });

          updateColor("frontColor", LauncherData.frontColor);
        });
        return;
      } else if (input.id === "setting-primaryColor") {
        input.addEventListener("input", async () => {
          let hexValue = document.querySelector(`input[id=${input.id}]`).value;
          LauncherData[input.id.split("-")[1]] = `${parseInt(
            hexValue.substr(1, 2),
            16
          )}, ${parseInt(hexValue.substr(3, 2), 16)},${parseInt(
            hexValue.substr(5, 2),
            16
          )}`;

          await invoke("write_file", {
            filePath: (await path.appDir()) + "LauncherData.json",
            fileContent: JSON.stringify(LauncherData),
          });

          updateColor("primaryColor", LauncherData.primaryColor);
        });
        return;
      }
      input.addEventListener("change", async () => {
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

            document.getElementById("rpc").innerHTML = "Connected";
          } catch (error) {
            invoke("disable_rpc", { enable: false });
            document.getElementById("rpc").innerHTML = "Disconnected";
          }

          window.location.reload();
        }

        if (input.id === "setting-enable_spotify") {
          await invoke("launcherdata_threads_x");

          const { enable_spotify } = JSON.parse(
            await invoke("read_file", {
              filePath: (await path.appDir()) + "LauncherData.json",
            })
          );

          if (enable_spotify) {
            await invoke("spotify_login");
            await invoke("spotify_connect");
          }

          window.location.reload();
        }

        if (input.id === "setting-enable_overlay") {
          await invoke("launcherdata_threads_x");
          alert("A restart is required.");
        }

        if (input.id === "setting-enableLauncherIcons")
          window.location.reload();
      });
    });

    document.querySelectorAll("div[id^=theme-box-]").forEach((div) => {
      const themes = {
        default: {
          primaryColor: "12, 11, 14",
          backgroundColor: "20, 14, 36",
          frontColor: "42, 16, 81",
          accentColor: "121, 52, 250",
        },
        midnight: {
          primaryColor: "36, 0, 70",
          backgroundColor: "35, 2, 100",
          frontColor: "42, 16, 81",
          accentColor: "121, 52, 250",
        },
        crimson: {
          primaryColor: "102, 7, 8",
          backgroundColor: "43, 8, 14",
          frontColor: "57, 8, 8",
          accentColor: "165, 29, 45",
        },
      };
      div.addEventListener("click", async () => {
        const name = div.id.split("-")[2];
        const theme = themes[name];
        let LauncherData = JSON.parse(
          await invoke("read_file", {
            filePath: (await path.appDir()) + "LauncherData.json",
          })
        );
        LauncherData = { ...LauncherData, ...theme };
        console.log(LauncherData);
        await invoke("write_file", {
          filePath: (await path.appDir()) + "LauncherData.json",
          fileContent: JSON.stringify(LauncherData),
        });

        updateColor("primaryColor", theme.primaryColor);
        updateColor("backgroundColor", theme.backgroundColor);
        updateColor("frontColor", theme.frontColor);
        updateColor("accentColor", theme.accentColor);
      });
    });
    function updateColor(id, color) {
      const vals = {
        primaryColor: "all-color-primary",
        backgroundColor: "all-color-back",
        frontColor: "all-color-front",
        accentColor: "accent-color",
      };
      // document.getElementById("indicator").style.backgroundColor = color;
      document.querySelector(":root").style.setProperty(`--${vals[id]}`, color);
      document.getElementById(`setting-${id}`).value = color;
    }

    async function setActivity(tab) {
      let timestamp = null;
      const { state, details, largeImage, largeText, smallImage, smallText } =
        require("./modules/rpcOptions").selectOption(tab);
      if (timestamp === null) timestamp = Date.now();
      try {
        await invoke(`set_rpc_activity`, {
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
  margin-top: -10px;
  justify-content: center;
  align-content: center;
  border-radius: 16px !important;
}

.settings {
  position: absolute;
  height: 540px;
  width: 600px;
  display: none;
  background: rgba(var(--all-color-primary), 0.7);
  border: 4px solid rgba(var(--accent-color), 0.7);
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
  margin-bottom: 30px;
}

.choser button {
  font-family: Nunito-Bold !important;
  color: rgb(201, 201, 201);
  text-decoration: none;
  text-shadow: none;
  font-size: 18px;

  background: rgba(var(--all-color-front), 0.7);
  width: 150px;
  height: 40px;
  border: none;
  outline: none;
  padding: 0;
}

.choser button:first-child {
  border-radius: 10px;
  border-top-right-radius: 0px;
  border-bottom-right-radius: 0px;
}

.choser button:last-child {
  border-radius: 10px;
  border-top-left-radius: 0px;
  border-bottom-left-radius: 0px;
}

.choser button:hover {
  border: rgba(var(--accent-color), 0.7);
  border-width: 4px;
  border-style: solid;
  cursor: pointer;
}

.appearance-settings,
.general-settings {
  display: flex;
  flex-direction: column;
  align-items: center;
  margin-left: auto;
  overflow: auto;
  height: 320px;
}

.appearance-settings {
  display: none;
}

.appearance-settings .themes {
  height: 70px !important;
  justify-content: center !important;
  margin-bottom: 10px;
}

.appearance-settings .themes .color .theme-box {
  border: 4px solid rgba(var(--accent-color), 0.7);
  border-right: 0px;
}

.appearance-settings .themes .color .theme-box:first-child {
  border-radius: 10px 0px 0px 10px;
  border-right: 0px;
}

.appearance-settings .themes .color .theme-box:last-child {
  border-radius: 0px 10px 10px 0px;
  border-right: 4px solid rgba(var(--accent-color), 0.7);
}

.appearance-settings .setting,
.general-settings .setting {
  display: flex;
  width: 350px;
  height: 45px;
  justify-content: space-between;
  align-items: center;
}

.appearance-settings div p,
.general-settings div p {
  min-width: 105px;
}

.btnInput {
  display: flex;
  align-items: center;
  justify-content: flex-end;
  height: 55px;
  width: 120px;
}

input[type="color"] {
  background-color: rgba(var(--all-color-primary), 0.7);
  color: rgba(var(--all-color-primary), 0.7);
  padding: 0;
  margin: 4px;
  height: 40px;
  width: 55px;
  border: none;
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
  /* margin-top: -41px; */
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

.theme-box {
  height: 45px;
  width: 60px;
  cursor: pointer;
}
</style>
