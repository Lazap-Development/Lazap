<template>
  <div class="settings-background" id="settings-backblur"></div>
  <div class="settings" id="settings-popup">
    <div>
      <h1>Settings</h1>
      <div class="settings-body">
        <div class="chooser" id="chooser">
          <button class="active" id="generalbtn">General</button>
          <button id="appearancebtn">Appearance</button>
          <button id="addonsbtn">Connections</button>
          <button id="expbtn">Experimental</button>
        </div>
        <div class="general-settings" id="general-settings">
          <div class="setting">
            <p>Launch on Startup</p>
            <div class="btnInput">
              <label class="switch">
                <input type="checkbox" id="setting-launch_on_startup" />
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
                <input
                  type="checkbox"
                  id="setting-tray_min_launch"
                  disabled="readonly"
                />
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

        <div
          class="general-settings"
          id="addons-settings"
          style="display: none"
        >
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
        </div>

        <div class="general-settings" id="exp-settings" style="display: none">
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
          <div class="setting">
            <p>Window Blur</p>
            <div class="btnInput">
              <label class="switch">
                <input type="checkbox" id="setting-enable_blur" />
                <div>
                  <span></span>
                </div>
              </label>
            </div>
          </div>
        </div>
      </div>
      <div class="settings-footer">Release v0.8.3</div>
    </div>
  </div>
</template>

<script>
import { selectOption } from "./misc/rpcOptions.js";
import { enable, isEnabled, disable } from "tauri-plugin-autostart-api";

export default {
  name: "settings-comp",
  async mounted() {
    console.log(await isEnabled());
    const settings = document.getElementById("settings-popup");
    const settingsbackblur = document.getElementById("settings-backblur");
    const path = window.__TAURI__.path;
    const invoke = window.__TAURI__.invoke;
    const os = window.__TAURI__.os;

    settingsbackblur.addEventListener("click", () => {
      settings.style.display = "none";
      settingsbackblur.style.display = "none";
      document.getElementById("general-settings").style.display = "flex";
      document.getElementById("appearance-settings").style.display = "none";
      document.getElementById("addons-settings").style.display = "none";
      document.getElementById("exp-settings").style.display = "none";
    });

    document.addEventListener("keydown", (event) => {
      if (event.key === "Escape" || event.key === "Esc") {
        settings.style.display = "none";
        settingsbackblur.style.display = "none";
        document.getElementById("general-settings").style.display = "flex";
        document.getElementById("appearance-settings").style.display = "none";
        document.getElementById("addons-settings").style.display = "none";
        document.getElementById("exp-settings").style.display = "none";
      }
    });

    document
      .getElementById("settings-btn")
      .addEventListener("click", async () => {
        settingsbackblur.style.display = "flex";
        settings.style.display = "flex";
        toggleSettings("general");

        const Data = JSON.parse(
          await invoke("read_file", {
            filePath: (await path.appDir()) + "LauncherData.json",
          })
        );
        document
          .querySelectorAll("input[id^=setting-]")
          .forEach(async (input) => {
            if (input.id.startsWith("setting") && input.id.endsWith("Color"))
              input.value = rgbToHex(Data[input.id.split("-")[1]]);
            else if (input.id.endsWith("startup")) {
              input.value = await isEnabled();
            } else input.checked = Data[input.id.split("-")[1]] ? true : false;
          });
      });

    document.getElementById("generalbtn").addEventListener("click", () => {
      toggleSettings("general");
    });

    document.getElementById("appearancebtn").addEventListener("click", () => {
      toggleSettings("appearance");
    });

    document.getElementById("addonsbtn").addEventListener("click", () => {
      toggleSettings("addons");
    });

    document.getElementById("expbtn").addEventListener("click", () => {
      toggleSettings("exp");
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
        document.getElementById(`setting-${id}`).value = rgbToHex(
          LauncherData[id]
        );
        updateColor(id, LauncherData[id]);
      });
    }

    document.querySelectorAll("input[id^=setting-]").forEach((input) => {
      if (input.id === "setting-accentColor") {
        input.addEventListener("input", async () => {
          let hexValue = document.getElementById(input.id).value;
          const clr = `${parseInt(hexValue.substr(1, 2), 16)}, ${parseInt(
            hexValue.substr(3, 2),
            16
          )}, ${parseInt(hexValue.substr(5, 2), 16)}`;
          updateColor("accentColor", clr);
          LauncherData = JSON.parse(
            await invoke("read_file", {
              filePath: (await path.appDir()) + "LauncherData.json",
            })
          );
          LauncherData[input.id.split("-")[1]] = clr;

          await invoke("write_file", {
            filePath: (await path.appDir()) + "LauncherData.json",
            fileContent: JSON.stringify(LauncherData),
          });
        });
        return;
      } else if (input.id === "setting-backgroundColor") {
        input.addEventListener("input", async () => {
          let hexValue = document.getElementById(input.id).value;
          const clr = `${parseInt(hexValue.substr(1, 2), 16)}, ${parseInt(
            hexValue.substr(3, 2),
            16
          )}, ${parseInt(hexValue.substr(5, 2), 16)}`;
          updateColor("backgroundColor", clr);
          LauncherData = JSON.parse(
            await invoke("read_file", {
              filePath: (await path.appDir()) + "LauncherData.json",
            })
          );
          LauncherData[input.id.split("-")[1]] = clr;

          await invoke("write_file", {
            filePath: (await path.appDir()) + "LauncherData.json",
            fileContent: JSON.stringify(LauncherData),
          });
        });
        return;
      } else if (input.id === "setting-frontColor") {
        input.addEventListener("input", async () => {
          let hexValue = document.getElementById(input.id).value;
          const clr = `${parseInt(hexValue.substr(1, 2), 16)}, ${parseInt(
            hexValue.substr(3, 2),
            16
          )}, ${parseInt(hexValue.substr(5, 2), 16)}`;
          updateColor("frontColor", clr);
          LauncherData = JSON.parse(
            await invoke("read_file", {
              filePath: (await path.appDir()) + "LauncherData.json",
            })
          );
          LauncherData[input.id.split("-")[1]] = clr;

          await invoke("write_file", {
            filePath: (await path.appDir()) + "LauncherData.json",
            fileContent: JSON.stringify(LauncherData),
          });
        });
        return;
      } else if (input.id === "setting-primaryColor") {
        input.addEventListener("input", async () => {
          let hexValue = document.getElementById(input.id).value;
          const clr = `${parseInt(hexValue.substr(1, 2), 16)}, ${parseInt(
            hexValue.substr(3, 2),
            16
          )}, ${parseInt(hexValue.substr(5, 2), 16)}`;
          updateColor("primaryColor", clr);
          LauncherData = JSON.parse(
            await invoke("read_file", {
              filePath: (await path.appDir()) + "LauncherData.json",
            })
          );
          LauncherData[input.id.split("-")[1]] = clr;

          await invoke("write_file", {
            filePath: (await path.appDir()) + "LauncherData.json",
            fileContent: JSON.stringify(LauncherData),
          });
        });
        return;
      }
      input.addEventListener("change", async () => {
        LauncherData[input.id.split("-")[1]] = document.getElementById(
          input.id
        ).checked;
        await invoke("write_file", {
          filePath: (await path.appDir()) + "LauncherData.json",
          fileContent: JSON.stringify(LauncherData),
        });

        if (input.id === "setting-launch_on_startup") {
          if (LauncherData[input.id.split("-")[1]] == true) {
            await enable();
          } else {
            await disable();
          }
        } else if (input.id === "setting-enable_rpc") {
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
        } else if (input.id === "setting-enable_spotify") {
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
        } else if (input.id === "setting-enable_overlay") {
          await invoke("launcherdata_threads_x");
          alert("A restart is required.");
        } else if (input.id === "setting-enableLauncherIcons") {
          window.location.reload();
        } else if (input.id === "setting-enable_blur") {
          LauncherData = JSON.parse(
            await invoke("read_file", {
              filePath: (await path.appDir()) + "LauncherData.json",
            })
          );

          if (!LauncherData[input.id.split("-")[1]]) {
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
          } else {
            if ((await os.platform()) == "linux") {
              alert(
                "PLEASE NOTE:\n\n Window Blur on Linux depends on your WM or DE compositor settings!\n Usually, adding lazap to window blur whitelist should do the trick.\n\n Otherwise, you will not see any blur effects on the window!"
              );
            }
            window.location.reload();
          }
        }
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

      // document.getElementById(`setting-${id}`).value = rgbToHex(color);
    }

    function rgbToHex(x) {
      let str_array = x.split(", ");
      return (
        "#" +
        ((1 << 24) | (str_array[0] << 16) | (str_array[1] << 8) | str_array[2])
          .toString(16)
          .slice(1)
      );
    }

    async function setActivity(tab) {
      let timestamp = null;
      const { state, details, largeImage, largeText, smallImage, smallText } =
        selectOption(tab);
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

    function toggleSettings(settingsId) {
      const settings = ["general", "appearance", "addons", "exp"];
      settings.forEach((setting) => {
        const element = document.getElementById(setting + "-settings");
        const btn = document.getElementById(setting + "btn");
        if (setting === settingsId) {
          element.style.display = "flex";
          btn.classList.add("active");
        } else {
          element.style.display = "none";
          btn.classList.remove("active");
        }
      });
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
  -webkit-backdrop-filter: blur(10px);
  display: none;
  justify-content: center;
  align-content: center;
  border-radius: 16px !important;
}

.settings {
  position: absolute;
  height: 500px;
  width: 700px;
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

.settings-body {
  display: flex;
  /* flex-direction: row; */
  gap: 40px;
}

.settings-footer {
  color: rgb(201, 201, 201, 0.7);
  position: absolute;
  bottom: 0;
  left: 0;
  margin-bottom: 14px;
  margin-left: 20px;
}

.chooser {
  display: flex;
  align-items: center;
  flex-direction: column;
}

.chooser button {
  font-family: Nunito-Bold !important;
  color: rgb(201, 201, 201);
  text-decoration: none;
  text-shadow: none;
  font-size: 18px;

  background: rgba(var(--all-color-front), 0.7);
  width: 180px;
  height: 40px;
  border: none;
  outline: none;
  padding: 0;
  transition: all 0.15s cubic-bezier(0.165, 0.74, 0.44, 1);
}

.chooser .active {
  background-color: rgba(var(--accent-color), 0.4);
}

.chooser button:first-child {
  border-radius: 10px;
  border-bottom-right-radius: 0px;
  border-bottom-left-radius: 0px;
}

.chooser button:last-child {
  border-radius: 10px;
  border-top-right-radius: 0px;
  border-top-left-radius: 0px;
}

.chooser button:hover {
  background-color: rgba(var(--accent-color), 0.4);
  transition: all 0.15s cubic-bezier(0.165, 0.74, 0.44, 1);
}

.appearance-settings,
.general-settings {
  display: flex;
  flex-direction: column;
  align-items: center;
  margin-left: auto;
  overflow: auto;
  height: 400px;
  margin-right: 20px;
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
  --dot: rgb(var(--accent-color));
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
