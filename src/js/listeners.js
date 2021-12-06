const marker = document.querySelector("#indicator");
const items = document.querySelectorAll(".side-tab");
const home = document.getElementById("home");
const recent = document.getElementById("recent");
const games = document.getElementById("games");
const favs = document.getElementById("favs");

document.getElementById("home-btn").addEventListener("click", function () {
    home.style.display = "flex";
    recent.style.display = "none";
    games.style.display = "none";
    favs.style.display = "none";
})

document.getElementById("recent-btn").addEventListener("click", function () {
    home.style.display = "none";
    recent.style.display = "flex";
    games.style.display = "none";
    favs.style.display = "none";
})

document.getElementById("games-btn").addEventListener("click", function () {
    home.style.display = "none";
    recent.style.display = "none";
    games.style.display = "flex";
    favs.style.display = "none";

    document.querySelector('div#games')
    require('./js/launchers/find-games.js').loadGames();
})

document.getElementById("favs-btn").addEventListener("click", function () {
    home.style.display = "none";
    recent.style.display = "none";
    games.style.display = "none";
    favs.style.display = "flex";
})


items.forEach(link => link.addEventListener("click", e => {
    marker.style.top = "220px";
    marker.style.height = "0px";
    indicator(e.target)
}));

function indicator(item) {
    marker.style.top = item.offsetTop + "px";
    marker.style.height = "30px";
}

window.onkeydown = function (evt) {
    if ((evt.code == "Minus" || evt.code == "Equal") && (evt.ctrlKey || evt.metaKey)) {
        evt.preventDefault()
    }
}