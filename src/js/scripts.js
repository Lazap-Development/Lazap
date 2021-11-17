// Steam
document.getElementById("steam").addEventListener("mouseenter", function( event ) {
    document.getElementById("backgroundId").style.backgroundImage = "url(../img/steam-preview.jpg) ";
});
document.getElementById("steam").addEventListener("mouseleave", function( event ) {
    document.getElementById("backgroundId").style.backgroundImage = "";
    document.getElementById("backgroundId").style.backgroundColor = "rgba(193, 73, 255, 1)";
});


// Epic Games
document.getElementById("epic").addEventListener("mouseenter", function( event ) {
    document.getElementById("backgroundId").style.backgroundImage = "url(../img/epic-preview.jpg)";
});
document.getElementById("epic").addEventListener("mouseleave", function( event ) {
    document.getElementById("backgroundId").style.backgroundImage = "";
    document.getElementById("backgroundId").style.backgroundColor = "rgba(193, 73, 255, 1)";
});


// Rockstar Launcher
document.getElementById("rockstar").addEventListener("mouseenter", function( event ) {
    document.getElementById("backgroundId").style.backgroundImage = "url(../img/rockstar-preview.jpg)";
});
document.getElementById("rockstar").addEventListener("mouseleave", function( event ) {
    document.getElementById("backgroundId").style.backgroundImage = "";
    document.getElementById("backgroundId").style.backgroundColor = "rgba(193, 73, 255, 1)";
});