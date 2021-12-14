const marker = document.querySelector("#indicator");
const items = document.querySelectorAll(".side-tab");
const home = document.querySelector("#home");
const recent = document.querySelector("#recent");
const games = document.querySelector("#games");
const favs = document.querySelector("#favs");
const friends = document.querySelector("#friends");
const messages = document.querySelector("#messages");
const activity = document.querySelector("#activity");

document.querySelector("#home-btn").addEventListener("click", function () {
    home.style.display = "flex";
    recent.style.display = "none";
    games.style.display = "none";
    favs.style.display = "none";
    friends.style.display = "none";
    messages.style.display = "none";
    activity.style.display = "none";
})

document.querySelector("#recent-btn").addEventListener("click", function () {
    home.style.display = "none";
    recent.style.display = "flex";
    games.style.display = "none";
    favs.style.display = "none";
    friends.style.display = "none";
    messages.style.display = "none";
    activity.style.display = "none";
})

document.querySelector("#games-btn").addEventListener("click", function () {
    home.style.display = "none";
    recent.style.display = "none";
    games.style.display = "flex";
    favs.style.display = "none";
    friends.style.display = "none";
    messages.style.display = "none";
    activity.style.display = "none";

    document.querySelector('div#games')
    require('./js/launchers/find-games.js').loadGames();
})

document.querySelector("#favs-btn").addEventListener("click", function () {
    home.style.display = "none";
    recent.style.display = "none";
    games.style.display = "none";
    favs.style.display = "flex";
    messages.style.display = "none";
    activity.style.display = "none";
    friends.style.display = "none";
})

document.querySelector("#messages-btn").addEventListener("click", function () {
    home.style.display = "none";
    recent.style.display = "none";
    games.style.display = "none";
    favs.style.display = "none";
    messages.style.display = "flex";
    activity.style.display = "none";
    friends.style.display = "none";
})

document.querySelector("#activity-btn").addEventListener("click", function () {
    home.style.display = "none";
    recent.style.display = "none";
    games.style.display = "none";
    favs.style.display = "none";
    messages.style.display = "none";
    activity.style.display = "flex";
    friends.style.display = "none";
})

document.querySelector("#friends-btn").addEventListener("click", function () {
    home.style.display = "none";
    recent.style.display = "none";
    games.style.display = "none";
    favs.style.display = "none";
    messages.style.display = "none";
    activity.style.display = "none";
    friends.style.display = "flex";
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