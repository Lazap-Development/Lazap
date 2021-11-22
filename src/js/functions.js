let img;
function loadFile(event) {
    var image = document.getElementById("output");
    image.src = URL.createObjectURL(event.target.files[0]);
    img = event.target.files[0].path;

    ipcRenderer.send('update-profile', {
        username: document.querySelector('input#text').value,
        pfp: event.target.files[0].path
    });
}

document.querySelector('#text').addEventListener('change', (e) => {  // TODO: no work
    ipcRenderer.send('update-profile', {
        username: e.target.value,
        pfp: img
    });
})

ipcRenderer.on('load-profile', (event, data) => {
    document.querySelector('img#output').src = data.pfp.length < 1 ? 'https://cdn.discordapp.com/avatars/633730629560958976/5c1abedd641bb81ecc381696950a0b16.png?size=1024' : data.pfp;
    document.querySelector('input#text').value = data.username;
    if (!img) {
        img = data.pfp.length < 1 ? 'https://cdn.discordapp.com/avatars/633730629560958976/5c1abedd641bb81ecc381696950a0b16.png?size=1024' : data.pfp;
    }
});