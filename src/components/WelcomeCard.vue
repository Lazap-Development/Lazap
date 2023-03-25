<template>
  <div class="welcome-background" id="welcome-backblur"></div>
  <div class="welcome" id="welcome-popup">
      <section class="start-page" id="start-page">
        <h1>Welcome to Lazap</h1>
      </section>
      <section class="all-games-page" id="all-games-page">
        end
      </section>
      <div class="choser" id="choser">
        <button id="previous">
          <svg xmlns="http://www.w3.org/2000/svg" width="30" height="30" viewBox="0 0 24 24">
            <path d="M15.41 7.41L14 6l-6 6 6 6 1.41-1.41L10.83 12l4.58-4.59z" />
          </svg>
        </button>
        <button id="next">
          <svg xmlns="http://www.w3.org/2000/svg" width="30" height="30" viewBox="0 0 24 24">
            <path d="M8.59 16.59L10 18l6-6-6-6-1.41 1.41L13.17 12l-4.58 4.59z" />
          </svg>
        </button>
      </div>
  </div>
</template>

<script>
export default {
  name: "welcome-comp",
  async mounted() {
    const welcome = document.getElementById("welcome-popup");
    const welcomebackblur = document.getElementById("welcome-backblur");

    welcomebackblur.addEventListener("click", () => {
      welcome.style.display = "none";
      welcomebackblur.style.display = "none";
    });

    welcomebackblur.style.display = "flex";
    welcome.style.display = "flex";

    document.getElementById("start-page").style.display === "flex" ? document.getElementById("start-page").disabled = true : ""

    let currentSection = 1;
    let totalSections = 2;

    showSection(1)

    function showSection(sectionNumber) {
      let sections = document.querySelectorAll('#welcome-popup section');
      for (let i = 0; i < sections.length; i++) {
        sections[i].style.display = 'none';
      }
      sections[sectionNumber - 1].style.display = 'flex';
      sectionNumber - 1 === 0 ? document.getElementById('welcome-popup').style.background = "black" : document.getElementById('welcome-popup').style.background = "var(--allColorBack)"
      currentSection = sectionNumber;
    }

    function showNextSection() {
      if (currentSection < totalSections) {
        showSection(currentSection + 1);
      }
    }

    function showPrevSection() {
      if (currentSection > 1) {
        showSection(currentSection - 1);
      }
    }

    document.getElementById('previous').addEventListener('click', showPrevSection);
    document.getElementById('next').addEventListener('click', showNextSection);
  },
};
</script>

<style>
.welcome-background {
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

.welcome {
  position: absolute;
  height: 500px;
  width: 600px;
  display: none;
  flex-direction: column;
  background: var(--allColorBack);
  border: 4px solid var(--accentColor);
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

.welcome h1 {
  font-size: 26px;
  text-align: center;
  font-family: Nunito-Bold;
  font-weight: normal;
  color: rgb(201, 201, 201);
  margin-top: 45px;
  margin-bottom: 20px;
}

.welcome p {
  font-size: 18px;
  position: relative;

  font-family: Nunito;
  color: rgb(201, 201, 201);

  text-decoration: none;
  text-shadow: none;
  cursor: default;
  z-index: -15;
}

.choser {
  display: flex;
  justify-content: space-evenly;
  align-items: center;
  height: 10%;
}

.choser button {
  font-family: Nunito-Bold !important;
  color: rgb(201, 201, 201);
  border-radius: 50%;
  text-decoration: none;
  text-shadow: none;
  font-size: 18px;

  background: var(--allColorPrimary);
  width: 40px;
  height: 40px;
  border: none;
  outline: none;
  padding: 0;
  margin: 0 5px;
}

.choser button svg {
  filter: invert(100);
}

.choser button:hover {
  /*animation: settingsBtnAnimation 0.2s forwards;*/
  cursor: pointer;
}

.start-page,
.all-games-page {
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;
  height: 90%;

  color: white;
}

/*@keyframes settingsBtnAnimation {
  0% {
    border-bottom-width: 0px;
    border-bottom-style: none;
  }

  100% {
    border-bottom: var(--accentColor);
    border-bottom-width: 4px;
    border-bottom-style: solid;
  }
}*/
</style>
