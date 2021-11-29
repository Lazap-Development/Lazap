const { ipcRenderer } = require("electron");

function load_main () {
    ipcRenderer.send("load-main")
}

function close_window () {
    ipcRenderer.send("close-window")
}

function max_window () {
    ipcRenderer.send("max-window")
}

function min_window () {
    ipcRenderer.send("min-window")
}