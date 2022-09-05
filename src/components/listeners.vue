<script>
window.addEventListener('load', async function () {
  const time = Date.now();
  const home = document.getElementById('home');
  const recent = document.getElementById('recent');
  const games = document.getElementById('games');
  const favs = document.getElementById('favs');
  const friends = document.getElementById('friends');
  const messages = document.getElementById('messages');
  const activity = document.getElementById('activity');
  const settings = document.getElementById('settings-popup');
  const settingsbackblur = document.getElementById('settings-backblur');
  const searchbars = document.querySelectorAll('div.search-bar > input[type="text"]');
  const fs = window.__TAURI__.fs;
  const path = window.__TAURI__.path;
  const dialog = window.__TAURI__.dialog;
  console.log(Date.now() - time, 'imports')

  const data = JSON.parse(await fs.readTextFile(await path.appDir() + 'storage/cache/user/UserProfile.json', (err) => {
    if (err) throw err;
  }));
  document.getElementById('text').value = data.username;
  try {
    await fs.readBinaryFile(await path.appDir() + 'storage/cache/user/pfp.png');
    document.getElementById('output').src = window.__TAURI__.tauri.convertFileSrc(await path.appDir() + 'storage/cache/user/pfp.png') + `?${new Date().getSeconds()}`;
  } catch (err) {
    //console.error(error)
  }
  console.log(Date.now() - time, 'pfp')
  try {
    let { accentColor } = JSON.parse(await fs.readTextFile(await path.appDir() + 'storage/LauncherData.json'));
    if (!accentColor) accentColor = "#7934FA";
    updateAccentColor(accentColor);
  } catch (error) {
    console.error(error);
  }
  console.log(Date.now() - time, 'accent')

  const allGames = await require('./launchers/find-games').getInstalledGames()
    .catch((err) => {
      return console.error(err);
    });
  console.log(Date.now() - time, 'game fetching')
  const gamesdata = await require('./launchers/find-games').getGames()
    .catch((err) => {
      return console.error(err);
    });
  console.log(Date.now() - time, 'game data fetching')

  await require('./launchers/find-games').loadGames('recentGamesListMainPage', allGames, gamesdata)
    .catch((err) => {
      return console.error(err);
    });
  await require('./launchers/find-games').loadGames('recentGamesList', allGames, gamesdata)
    .catch((err) => {
      return console.error(err);
    });
  await require('./launchers/find-games').loadGames('allGamesList', allGames)
    .catch((err) => {
      return console.error(err);
    });
  await require('./launchers/find-games').loadGames('favGamesList', allGames, gamesdata)
    .catch((err) => {
      return console.error(err);
    });
  console.log(Date.now() - time, 'game loading')

  document.getElementById('main-loading-overlay').style.opacity = '0';
  document.getElementById('main-loading-overlay').style.visibility = 'hidden';
  console.log(Date.now() - time);

  document.getElementById('home-btn').addEventListener('click', async function () {
    this.appendChild(document.getElementById('indicator'));
    if (home.style.display !== 'flex') {
      toggleIndicatorAnim();
    }

    home.style.display = 'flex';
    recent.style.display = 'none';
    games.style.display = 'none';
    favs.style.display = 'none';
    friends.style.display = 'none';
    messages.style.display = 'none';
    activity.style.display = 'none';

    await require('./launchers/find-games').loadGames('recentGamesListMainPage')
      .catch((err) => {
        return console.error(err);
      });
  });

  document.getElementById('recent-btn').addEventListener('click', async function () {
    this.appendChild(document.getElementById('indicator'));
    if (recent.style.display !== 'flex') {
      toggleIndicatorAnim();
    }

    home.style.display = 'none';
    recent.style.display = 'flex';
    games.style.display = 'none';
    favs.style.display = 'none';
    friends.style.display = 'none';
    messages.style.display = 'none';
    activity.style.display = 'none';

    await require('./launchers/find-games').loadGames('recentGamesList')
      .catch((err) => {
        return console.error(err);
      });
  });

  document.getElementById('games-btn').addEventListener('click', async function () {
    this.appendChild(document.getElementById('indicator'));

    if (games.style.display !== 'flex') {
      setTimeout(() => document.getElementById('gamesInput').focus(), 100);
      toggleIndicatorAnim();
    }

    home.style.display = 'none';
    recent.style.display = 'none';
    games.style.display = 'flex';
    favs.style.display = 'none';
    friends.style.display = 'none';
    messages.style.display = 'none';
    activity.style.display = 'none';

    await require('./launchers/find-games').loadGames('allGamesList')
      .catch((err) => {
        return console.error(err);
      });
  });

  document.getElementById('favs-btn').addEventListener('click', async function () {
    this.appendChild(document.getElementById('indicator'));

    if (favs.style.display !== 'flex') {
      setTimeout(() => document.getElementById('favsInput').focus(), 100);
      toggleIndicatorAnim();
    }

    home.style.display = 'none';
    recent.style.display = 'none';
    games.style.display = 'none';
    favs.style.display = 'flex';
    messages.style.display = 'none';
    activity.style.display = 'none';
    friends.style.display = 'none';

    await require('./launchers/find-games').loadGames('favGamesList')
      .catch((err) => {
        return console.error(err);
      });
  });

  document.getElementById('messages-btn').addEventListener('click', async function () {
    this.appendChild(document.getElementById('indicator'));
    if (messages.style.display !== 'flex') {
      toggleIndicatorAnim();
    }

    home.style.display = 'none';
    recent.style.display = 'none';
    games.style.display = 'none';
    favs.style.display = 'none';
    messages.style.display = 'flex';
    activity.style.display = 'none';
    friends.style.display = 'none';
  });

  document.getElementById('activity-btn').addEventListener('click', async function () {
    this.appendChild(document.getElementById('indicator'));
    if (activity.style.display !== 'flex') {
      toggleIndicatorAnim();
    }

    home.style.display = 'none';
    recent.style.display = 'none';
    games.style.display = 'none';
    favs.style.display = 'none';
    messages.style.display = 'none';
    activity.style.display = 'flex';
    friends.style.display = 'none';
  });

  document.getElementById('friends-btn').addEventListener('click', async function () {
    this.appendChild(document.getElementById('indicator'));
    if (friends.style.display !== 'flex') {
      toggleIndicatorAnim();
    }

    home.style.display = 'none';
    recent.style.display = 'none';
    games.style.display = 'none';
    favs.style.display = 'none';
    messages.style.display = 'none';
    activity.style.display = 'none';
    friends.style.display = 'flex';
  });

  document.getElementById('text').addEventListener('change', async function (change) {
    this.appendChild(document.getElementById('indicator'))
    fs.writeTextFile(await path.appDir() + 'storage/cache/user/UserProfile.json', JSON.stringify({ username: change.target.value, }), (err) => {
      if (err) throw err;
    });
  });

  settingsbackblur.addEventListener('click', () => {
    settings.style.display = 'none';
    settingsbackblur.style.display = 'none';
  });

  document.getElementById('settings-btn').addEventListener('click', async () => {
    settingsbackblur.style.display = 'flex';
    settings.style.display = 'flex';

    const appDirPath = await path.appDir();
    const Data = JSON.parse(await fs.readTextFile(appDirPath + 'storage/LauncherData.json'));
    document.querySelectorAll('input[id^=setting-]').forEach((input) => {
      // if(input.id === "setting-accentColor") return input.value = Data[input.id.split("-")[1]];
      // input.checked = Data[input.id.split('-')[1]] ? true : false;
      if (input.id === "setting-accentColor") input.value = Data[input.id.split("-")[1]]
      else input.checked = Data[input.id.split('-')[1]] ? true : false;
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

  document.querySelector(".repeatButton").addEventListener("click", async () => {
    let LauncherData = JSON.parse(await fs.readTextFile(await path.appDir() + 'storage/LauncherData.json'));
    LauncherData["accentColor"] = "#7934FA";
    fs.writeTextFile(await path.appDir() + 'storage/LauncherData.json', JSON.stringify(LauncherData));
    document.querySelector("input[id=setting-accentColor]").value = LauncherData["accentColor"];
    updateAccentColor(LauncherData["accentColor"]);
  });

  document.querySelectorAll('input[id^=setting-]').forEach((input) => {
    if (input.id === "setting-accentColor") {
      input.addEventListener("input", async () => {
        const LauncherData = JSON.parse(await fs.readTextFile(await path.appDir() + 'storage/LauncherData.json'));
        LauncherData[input.id.split("-")[1]] = document.querySelector(`input[id=${input.id}]`).value;
        fs.writeTextFile(await path.appDir() + 'storage/LauncherData.json', JSON.stringify(LauncherData));

        updateAccentColor(LauncherData.accentColor);
      });
      return;
    }
    input.addEventListener('change', async () => {
      const LauncherData = JSON.parse(await fs.readTextFile(await path.appDir() + 'storage/LauncherData.json'));
      LauncherData[input.id.split('-')[1]] = document.querySelector(`input[id=${input.id}]`).checked;
      fs.writeTextFile(await path.appDir() + 'storage/LauncherData.json', JSON.stringify(LauncherData));
    });
  });

  searchbars.item(0).addEventListener('keyup', () => {
    const query = searchbars.item(0).value;
    const allGames = document.querySelectorAll('#allGamesList > div[id^="game-div"]');
    allGames.forEach((game) => {
      if (game.id.split('-').slice(2).join('-').match(new RegExp(`${query.replaceAll(' ', '_')}`, 'gi'))) {
        game.style.display = 'block';
      }
      else if (query.length === 0) {
        game.style.display = 'block';
      }
      else {
        game.style.display = 'none';
      }
    });
  });
  searchbars.item(1).addEventListener('keyup', () => {
    const query = searchbars.item(1).value;
    const allGames = document.querySelectorAll('#favGamesList > div[id^="game-div"]');

    allGames.forEach((game) => {
      if (game.id.split('-').slice(2).join('-').match(new RegExp(`${query.replaceAll(' ', '_')}`, 'gi'))) {
        game.style.display = 'block';
      }
      else if (query.length === 0) {
        game.style.display = 'block';
      }
      else {
        game.style.display = 'none';
      }
    });
  });


  document.addEventListener('keydown', async function (e) {
    if (e.ctrlKey && e.key === "r") {
      window.location.reload();
    }
  })

  document.getElementById("addGameBtn").addEventListener('click', async function () {
    if (window.getComputedStyle(document.getElementById("addGamePopUp")).display === "flex") {
      document.getElementById("addGameCustomBannerOutput").style.backgroundImage = "url()";
      document.getElementById("addGamePopUp").style.display = "none";
      document.getElementById("inputGameName").value = "";
      document.getElementById("addGameCustomBannerTxt").style.opacity = "1"
      try {
        await fs.removeFile(await path.appDir() + `storage/cache/games/banners/newcustombanner.png`)
      } catch (e) {
        return e;
      }
    } else {
      document.getElementById("addGamePopUp").style.display = "flex";
      document.getElementById('inputGameName').focus()
    }
  })

  let newGameLocation;
  document.getElementById("addGameLocation").addEventListener("click", async function () {
    const selected = await dialog.open({
      multiple: false,
      filters: [{
        name: 'Windows Executable',
        extensions: ['exe']
      }]
    });
    if (selected !== null) {
      newGameLocation = selected;
    }
  })

  document.getElementById("addGameFinalBtn").addEventListener("click", async function () {
    if (document.getElementById("inputGameName").value.trim().length > 0 && newGameLocation) {
      let scheme = {
        DisplayName: document.getElementById("inputGameName").value,
        LauncherName: 'CustomGame',
        GameID: 'CustomGame',
        Executable: newGameLocation,
        Args: [],
      }

      try {
        await fs.renameFile(await path.appDir() + `storage/cache/games/banners/newcustombanner.png`, await path.appDir() + `storage/cache/games/banners/${require('./modules/sha256.js').sha256(document.getElementById("inputGameName").value.replaceAll(' ', '_'))}.png`)
        require("./launchers/find-games").Elements.createGameElement(scheme, "allGamesList");
        let data = JSON.parse(await fs.readTextFile(await path.appDir() + 'storage/cache/games/data.json'));
        data.push(scheme)
        await fs.writeTextFile(await path.appDir() + 'storage/cache/games/data.json', JSON.stringify(data))
      } catch (e) {
        require("./launchers/find-games").Elements.createGameElement(scheme, "allGamesList");
        let data = JSON.parse(await fs.readTextFile(await path.appDir() + 'storage/cache/games/data.json'));
        data.push(scheme)
        await fs.writeTextFile(await path.appDir() + 'storage/cache/games/data.json', JSON.stringify(data))
      }
      document.getElementById("addGameCustomBannerOutput").style.backgroundImage = "url()";
      document.getElementById("addGamePopUp").style.display = "none";
      document.getElementById("inputGameName").value = "";
      document.getElementById("addGameCustomBannerTxt").style.opacity = "1"
      newGameLocation = "";
    } else if (document.getElementById("inputGameName").value.trim().length > 0 && !newGameLocation) {
      return alert("You are missing the game location.");
    } else if (document.getElementById("inputGameName").value.length < 0 && newGameLocation) {
      return alert("You are missing the game name.");
    } else {
      return alert("You are missing the game name, and the game location.");
    }
  })

  document.getElementById("removeGame").addEventListener("click", async function () {
    let data = JSON.parse(await fs.readTextFile(await path.appDir() + 'storage/cache/games/data.json'));
    data = data.filter(a => a.DisplayName != document.getElementById("removeGame").parentNode.parentNode.firstChild.innerHTML)
    await fs.writeTextFile(await path.appDir() + 'storage/cache/games/data.json', JSON.stringify(data));
    document.getElementById(`game-div-${document.getElementById("removeGame").parentNode.parentNode.firstChild.innerHTML.replaceAll(' ', '_')}`).remove()
    document.getElementById("removeGame").parentNode.parentNode.remove()
  })

  function toggleIndicatorAnim() {
    let indicator = document.getElementById('indicator');
    indicator.classList.add("anim-indicatorscaleY")
    setTimeout(() => {
      indicator.classList.remove("anim-indicatorscaleY")
    }, 200);
  }
  function updateAccentColor(accentColor) {
    document.getElementById('indicator').style.backgroundColor = accentColor;
    document.querySelector(":root").style.setProperty("--back", accentColor);
  }
});
</script>
