<template>
  <div class="login-interface">
    <div data-tauri-drag-region class="titlebar-l">
      <div data-tauri-drag-region class="titlebar-icons-l">
        <div @click="min_window" class="titlebar-min-l mx-1"></div>
        <div @click="max_window" class="titlebar-max-l mx-1"></div>
        <div @click="close_window" class="titlebar-exit-l mx-1"></div>
      </div>
    </div>

    <div class="alert-box fadeInUpNoDelay" id="alertbox">
      <div class="alert-box-container">
        <img class="alert-box-exclamation" src="./assets/svg/triangle.svg" />
        <p class="alert-box-title">Alert</p>

        <span class="alert-box-cross" id="alertboxexit">&times;</span>

        <hr class="alert-box-divider" />
        <div class="alert-box-message" style="align-items: center">
          <p class="alert-box-message">The Launcher Loaded 😱</p>
        </div>
      </div>
    </div>

    <div class="bg" id="bg">
      <button
        @click="changeComponent('Main')"
        class="fadeInUp ignore"
        id="ignoreBTN"
      >
        Continue and Ignore
      </button>
      <div class="container fadeInUp" id="container">
        <div class="form-container sign-up-container">
          <div class="leftsection">
            <h1>Create Account</h1>
            <input type="text" placeholder="Username" />
            <input type="email" placeholder="Email" />
            <input type="password" placeholder="Password" />
            <button @click="changeComponent('Main')">Sign Up</button>
          </div>
        </div>
        <div class="form-container sign-in-container">
          <div class="leftsection">
            <h1>Sign In</h1>
            <input type="text" placeholder="Username" />
            <input type="password" placeholder="Password" />
            <button @click="changeComponent('Main')">Sign In</button>
            <a>Forgot your password?</a>
          </div>
        </div>
        <div class="overlay-container">
          <div class="overlay">
            <div class="overlay-panel overlay-left">
              <h1 class="mt-4">Dear, User!</h1>
              <p>
                We believe and respect complete privacy. We do not sell your
                data! It is completely your choice to sign up or login.
                <br /><br />
                If you do not login or sign up, You will not have access to some
                integerated features in this launcher. <br /><br /><br />
                - Lazap Development Team
              </p>
              <button id="signIn">Sign In</button>
            </div>
            <div class="overlay-panel overlay-right">
              <h1 class="mt-4">Welcome Back!</h1>
              <p>
                We believe in and respect privacy. We do not sell or miss-use
                any of gathered data. Moreover, It is completely your choice to
                sign up or login. <br /><br />
                If you do not login or sign up, You will not have access to some
                integerated features in this launcher. <br /><br /><br />
                - Lazap Development Team
              </p>
              <button id="signUp">Sign Up</button>
            </div>
          </div>
        </div>
      </div>
      <p class="line typewriter">
        © 2022 Lazap Development Team All Rights Reserved
      </p>
    </div>
  </div>
</template>

<script scoped>
export default {
  name: "-",
  props: ["changeComponent"],
  async mounted() {
    if ((await window.__TAURI__.os.platform()) === "win32") {
      document.getElementById("bg").style.height = "111.5vh";
    }

    const fs = window.__TAURI__.fs;
    const path = window.__TAURI__.path;
    const signUpButton = document.getElementById("signUp");
    const signInButton = document.getElementById("signIn");
    const container = document.getElementById("container");
    const alertbox = document.querySelector(".alert-box");
    const alertboxcross = document.getElementById("alertboxexit");
    const ignore = document.getElementById("ignoreBTN");
    alertboxcross.addEventListener("click", function () {
      alertbox.style.display = "none";
    });
    signUpButton.addEventListener("click", () => {
      container.classList.add("right-panel-active");
    });
    signInButton.addEventListener("click", () => {
      container.classList.remove("right-panel-active");
    });
    ignore.addEventListener("click", async () => {
      const appDirPath = await path.appDir();
      const parseJson = JSON.parse(
        (
          await fs.readTextFile(appDirPath + "storage/LauncherData.json")
        ).toString()
      );
      parseJson.skipLogin = true;
      fs.writeTextFile(
        appDirPath + "storage/LauncherData.json",
        JSON.stringify(parseJson)
      );
    });
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
  },
};
</script>

<style scoped>
@import "./css/login.css";
</style>
