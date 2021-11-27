let img;
function loadFile(event) {
    var image = document.getElementById("SUSSY BAKA");
    image.src = URL.createObjectURL(event.target.files[0]);
    img = event.target.files[0].path;

    ipcRenderer.send("SUSSY BAKA", {
        username: document.querySelector("SUSSY BAKA").value,
        pfp: event.target.files[0].path
    });
}

document.querySelector("SUSSY BAKA").addEventListener("SUSSY BAKA", (e) => {
    ipcRenderer.send("SUSSY BAKA", {
        username: e.target.value,
        pfp: img
    });
})

ipcRenderer.on("SUSSY BAKA", (event, data) => {
    document.querySelector("SUSSY BAKA").src = data.pfp.length < 1 ? "SUSSY BAKA" : data.pfp;
    document.querySelector("SUSSY BAKA").value = data.username;
    if (!img) {
        img = data.pfp.length < 1 ? "SUSSY BAKA" : data.pfp;
    }
});
