//Webpage On Load Event
window.onload = (event) => {
    document.getElementById("libraytext").innerHTML = `<i class="bi bi-collection-fill" style="font-size: 80px"></i>
    <br/>Welcome to Lazap!`;
}


// Library
document.getElementById("library").addEventListener("mouseover", function (event) {
    document.getElementById("backgroundId").style.backgroundImage = "";
    document.getElementById("backgroundId").style.backgroundColor = "#141414";
    document.getElementById("libraytext").innerHTML = `<i class="bi bi-collection-fill" style="font-size: 80px"></i>
    <br/>Personal Games library`;
});


// Steam
document.getElementById("steam").addEventListener("mouseenter", function (event) {
    document.getElementById("libraytext").innerHTML = `<i class="bi bi-collection-fill" style="font-size: 80px"></i>
    <br/>Steam`;
});

// Epic Games
document.getElementById("epic").addEventListener("mouseover", function (event) {
    document.getElementById("libraytext").innerHTML = `<i class="bi bi-collection-fill" style="font-size: 80px"></i>
    <br/>Epic Games`;
});

// Rockstar Launcher
document.getElementById("rockstar").addEventListener("mouseenter", function (event) {
    document.getElementById("libraytext").innerHTML = `<i class="bi bi-collection-fill" style="font-size: 80px"></i>
    <br/>Rockstar Games Launcher`;
});

document.getElementById("backgroundId").innerHTML = `${window.navigator.platform}`;