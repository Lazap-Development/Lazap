const { ipcRenderer } = require("electron");

function close_window () {
    ipcRenderer.send("close-window")
}

function max_window () {
    ipcRenderer.send("max-window")
}

function min_window () {
    ipcRenderer.send("min-window")
}

function loadFile(event) {
    var image = document.getElementById("output");
    image.src = URL.createObjectURL(event.target.files[0]);
};