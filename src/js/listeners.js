document.getElementById("home-btn").addEventListener("click", function () {
    document.getElementById("recent-ind").classList.remove("side-tab-indicator")
    document.getElementById("games-ind").classList.remove("side-tab-indicator")
    document.getElementById("favs-ind").classList.remove("side-tab-indicator")
    document.getElementById("messages-ind").classList.remove("side-tab-indicator")
    document.getElementById("activity-ind").classList.remove("side-tab-indicator")
    document.getElementById("friends-ind").classList.remove("side-tab-indicator")
    
    document.getElementById("home-ind").classList.add("side-tab-indicator")
})

document.getElementById("recent-btn").addEventListener("click", function () {
    document.getElementById("home-ind").classList.remove("side-tab-indicator")
    document.getElementById("games-ind").classList.remove("side-tab-indicator")
    document.getElementById("favs-ind").classList.remove("side-tab-indicator")
    document.getElementById("messages-ind").classList.remove("side-tab-indicator")
    document.getElementById("activity-ind").classList.remove("side-tab-indicator")
    document.getElementById("friends-ind").classList.remove("side-tab-indicator")

    document.getElementById("recent-ind").classList.add("side-tab-indicator")
})

document.getElementById("games-btn").addEventListener("click", function () {
    document.getElementById("home-ind").classList.remove("side-tab-indicator")
    document.getElementById("recent-ind").classList.remove("side-tab-indicator")
    document.getElementById("favs-ind").classList.remove("side-tab-indicator")
    document.getElementById("messages-ind").classList.remove("side-tab-indicator")
    document.getElementById("activity-ind").classList.remove("side-tab-indicator")
    document.getElementById("friends-ind").classList.remove("side-tab-indicator")

    document.getElementById("games-ind").classList.add("side-tab-indicator")
})

document.getElementById("favs-btn").addEventListener("click", function () {
    document.getElementById("home-ind").classList.remove("side-tab-indicator")
    document.getElementById("recent-ind").classList.remove("side-tab-indicator")
    document.getElementById("games-ind").classList.remove("side-tab-indicator")
    document.getElementById("messages-ind").classList.remove("side-tab-indicator")
    document.getElementById("activity-ind").classList.remove("side-tab-indicator")
    document.getElementById("friends-ind").classList.remove("side-tab-indicator")

    document.getElementById("favs-ind").classList.add("side-tab-indicator")
})

document.getElementById("messages-btn").addEventListener("click", function () {
    document.getElementById("home-ind").classList.remove("side-tab-indicator")
    document.getElementById("recent-ind").classList.remove("side-tab-indicator")
    document.getElementById("games-ind").classList.remove("side-tab-indicator")
    document.getElementById("favs-ind").classList.remove("side-tab-indicator")
    document.getElementById("activity-ind").classList.remove("side-tab-indicator")
    document.getElementById("friends-ind").classList.remove("side-tab-indicator")

    document.getElementById("messages-ind").classList.add("side-tab-indicator")
})

document.getElementById("activity-btn").addEventListener("click", function () {
    document.getElementById("home-ind").classList.remove("side-tab-indicator")
    document.getElementById("recent-ind").classList.remove("side-tab-indicator")
    document.getElementById("games-ind").classList.remove("side-tab-indicator")
    document.getElementById("favs-ind").classList.remove("side-tab-indicator")
    document.getElementById("messages-ind").classList.remove("side-tab-indicator")
    document.getElementById("friends-ind").classList.remove("side-tab-indicator")

    document.getElementById("activity-ind").classList.add("side-tab-indicator")
})

document.getElementById("friends-btn").addEventListener("click", function () {
    document.getElementById("home-ind").classList.remove("side-tab-indicator")
    document.getElementById("recent-ind").classList.remove("side-tab-indicator")
    document.getElementById("games-ind").classList.remove("side-tab-indicator")
    document.getElementById("favs-ind").classList.remove("side-tab-indicator")
    document.getElementById("messages-ind").classList.remove("side-tab-indicator")
    document.getElementById("activity-ind").classList.remove("side-tab-indicator")

    document.getElementById("friends-ind").classList.add("side-tab-indicator")
})

