
// Steam
document.getElementById("steam").addEventListener("mouseenter", function (event) {
    document.getElementById("backgroundId").style.backgroundImage = "url(../img/steam-preview.jpg) ";
});

// Epic Games
document.getElementById("epic").addEventListener("mouseover", function (event) {
    document.getElementById("backgroundId").style.backgroundImage = "url(../img/epic-preview.jpg)";
});

// Rockstar Launcher
document.getElementById("rockstar").addEventListener("mouseenter", function (event) {
    document.getElementById("backgroundId").style.backgroundImage = "url(../img/rockstar-preview.jpg)";
});

document.getElementById("backgroundId").innerHTML = `${window.navigator.platform}`;