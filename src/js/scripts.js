//Webpage On Load Event
window.onload = (event) => {
    document.getElementById("backgroundId").style.backgroundImage = "";
    document.getElementById("backgroundId").style.backgroundColor = "#141414";
    document.getElementById("libraytext").innerHTML = `<i class="bi bi-collection-fill" style="font-size: 80px"></i>
    <br/>Your Personal Games library`;
}

// Library
document.getElementById("library").addEventListener("mouseover", function (event) {
    document.getElementById("backgroundId").style.backgroundImage = "";
    document.getElementById("backgroundId").style.backgroundColor = "#141414";
    document.getElementById("libraytext").innerHTML = `<i class="bi bi-collection-fill" style="font-size: 80px"></i>
    <br/>Your Personal Games library`;
});

        
// Steam
document.getElementById("steam").addEventListener("mouseenter", function (event) {
    document.getElementById("backgroundId").style.backgroundImage = "url(../img/steam-preview.jpg)";
    document.getElementById("libraytext").innerHTML = ``;
});

// Epic Games
document.getElementById("epic").addEventListener("mouseover", function (event) {
    document.getElementById("backgroundId").style.backgroundImage = "url(../img/epic-preview.jpg)";
    document.getElementById("libraytext").innerHTML = ``;
});

// Rockstar Launcher
document.getElementById("rockstar").addEventListener("mouseenter", function (event) {
    document.getElementById("backgroundId").style.backgroundImage = "url(../img/rockstar-preview.jpg)";
    document.getElementById("libraytext").innerHTML = ``;
});

document.getElementById("backgroundId").innerHTML = `${window.navigator.platform}`;
