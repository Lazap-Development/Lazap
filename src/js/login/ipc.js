const { ipcRenderer } = require("electron");

function load_main () {
    ipcRenderer.send("load-main")
}