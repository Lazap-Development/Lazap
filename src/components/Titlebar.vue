<template>
  <div data-tauri-drag-region class="titlebar-parent">
    <div data-tauri-drag-region class="titlebar">
      <div data-tauri-drag-region class="titlebar-icons">
        <div class="titlebar-options">
          <img
            class="titlebar-settings"
            src="../assets/svg/settings.svg"
            id="settings-btn"
          />
          <img
            class="titlebar-account"
            src="../assets/svg/account.svg"
            id="account-btn"
          />
          <img
            class="titlebar-update"
            src="../assets/svg/download.svg"
            id="update-btn"
          />

          <img
            class="titlebar-rpc"
            src="../assets/svg/discord.svg"
            id="rpcbtn"
          />
          <span id="rpc" class="rpc"></span>
        </div>
        <div class="titlebar-options">
          <div @click="min_window" class="titlebar-min mx-1"></div>
          <div @click="max_window" class="titlebar-max mx-1"></div>
          <div @click="close_window" class="titlebar-exit mx-1"></div>
        </div>
      </div>
    </div>
  </div>
</template>

<script>
import { selectOption } from "./misc/rpcOptions.js";

const path = window.__TAURI__.path;
const invoke = window.__TAURI__.invoke;
const os = window.__TAURI__.os;

export default {
  name: "titlebar-comp",
  methods: {
    min_window() {
      window.__TAURI__.window.appWindow.minimize();
    },
    max_window() {
      window.__TAURI__.window.appWindow.toggleMaximize();
    },
    async close_window() {
      let LauncherData = JSON.parse(
        await invoke("read_file", {
          filePath: (await path.appDir()) + "LauncherData.json",
        })
      );
      if (LauncherData["tray_min_quit"] == true) {
        window.__TAURI__.window.appWindow.hide();
      } else {
        window.__TAURI__.window.WebviewWindow.getByLabel("external").close();
        window.__TAURI__.window.WebviewWindow.getByLabel("overlay").close();
        window.__TAURI__.window.appWindow.close();
      }
    },
  },
  async mounted() {
    document
      .getElementById("update-btn")
      .addEventListener("click", async () => {
        if ((await os.platform()) === "win32") {
          window.__TAURI__.updater.installUpdate();
        } else {
          window.__TAURI__.shell.open(
            "https://github.com/Lazap-Development/lazap/releases/latest"
          );
        }
      });
    let timestamp = null;

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

    setTimeout(() => {
      document.getElementById("rpc").classList.add("fadeAwayRPCTxt");
    }, 1500);

    document.getElementById("rpcbtn").addEventListener("click", function () {
      document.getElementById("rpc").classList.remove("fadeAwayRPCTxt");
      setTimeout(() => {
        document.getElementById("rpc").classList.add("fadeAwayRPCTxt");
      }, 1000);
    });

    async function setActivity(tab) {
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
  },
};
</script>

<style>
.titlebar-parent {
  border-radius: 15px;
  padding: 10px;
}

.titlebar {
  border-radius: 15px;
  background-color: rgba(var(--all-color-primary), 0.7);
  height: 35px;
}

.titlebar-icons {
  display: flex;
  justify-content: space-between;
}

.titlebar-options {
  display: flex;
  justify-content: space-between;
  padding: 8px 15px;
}

.titlebar-exit,
.titlebar-max,
.titlebar-min {
  border-radius: 50px;
  transition: all 0.2s cubic-bezier(0.165, 0.84, 0.44, 1);
  height: 18px;
  width: 18px;
  background-color: rgba(var(--accent-color), 1);
  display: block;
}

.titlebar-exit:hover {
  background: #ff0000;
  background-size: cover;
}

.titlebar-max:hover {
  background: #ffc400;
}

.titlebar-min:hover {
  background: #22ff00;
}

.titlebar-settings {
  display: block;
  height: 18px;
  width: 18px;
  align-items: flex-start;
}

.titlebar-settings:hover {
  cursor: pointer;
  animation-delay: 200ms;
  animation: settings-spin 1.5s linear;
}

.titlebar-account {
  display: none;
  height: 18px;
  width: 18px;
  margin-left: 14px;
  align-items: flex-start;
  border-radius: 50px/50px;
}

.titlebar-account:hover {
  cursor: pointer;
  filter: invert(36%) sepia(89%) saturate(4522%) hue-rotate(225deg)
    brightness(99%) contrast(99%);
}

.titlebar-update {
  -webkit-app-region: no-drag;
  display: none;
  height: 17px;
  width: 17px;
  margin-left: 14px;
  align-items: flex-start;
  filter: invert(50%);
}

.titlebar-update:hover {
  cursor: pointer;
  filter: invert(90%) sepia(7%) saturate(2944%) hue-rotate(60deg)
    brightness(101%) contrast(84%);
}

.titlebar-loading {
  -webkit-app-region: no-drag;
  display: flex;
  opacity: 0;
  height: 24px;
  width: 24px;
  margin-top: -2px;
  margin-left: 8px;
  align-items: flex-start;
}

.titlebar-rpc {
  display: flex;
  height: 20px;
  width: 20px;
  margin-left: 14px;
  align-items: flex-start;
  filter: invert(50%);
}

.titlebar-rpc:hover {
  filter: invert(50%) sepia(75%) saturate(4277%) hue-rotate(219deg)
    brightness(91%) contrast(109%);
}

.rpc {
  display: flex;
  color: darkgray;
  font-family: Nunito-Bold;
  justify-content: center;
  align-items: center;
  font-size: 14px;
  margin-left: 10px;
  cursor: default;
}

.fadeAwayRPCTxt {
  animation: fadeAway 300ms forwards;
}

@keyframes fadeAway {
  0% {
    opacity: 1;
  }

  100% {
    opacity: 0;
  }
}
</style>
