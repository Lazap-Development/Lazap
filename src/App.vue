<template>
  <KeepAlive :max="2">
    <component :is="currentComponent" :change-component="changeComponent" />
  </KeepAlive>
</template>
<script>
import Main from "./Main.vue";
import Login from "./Login.vue";

export default {
  data() {
    return {
      currentComponent: null,
      components: {
        Login,
        Main,
      },
    };
  },
  async created() {
    const os = window.__TAURI__.os;
    const fs = window.__TAURI__.fs;
    const path = window.__TAURI__.path;
    console.info(`OS: ${await os.platform()} | Arch: ${await os.arch()}`);

    if (!this.currentComponent) {
      try {
        let data = JSON.parse(
          await fs.readTextFile(
            (await path.appDir()) + "storage/LauncherData.json",
            () => {
              return;
            }
          )
        );
        this.changeComponent(data?.skipLogin === true ? "Main" : "Login");
      } catch {
        this.changeComponent("Main");
      }
    }
  },
  methods: {
    min_window() {
      window.__TAURI__.window.appWindow.minimize();
    },
    max_window() {
      window.__TAURI__.window.appWindow.toggleMaximize();
    },
    close_window() {
      window.__TAURI__.window.appWindow.close();
    },
    changeComponent(component) {
      this.currentComponent = this.components[component];
    },
  },
};
</script>
