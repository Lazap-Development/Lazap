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

document.getElementById('text').addEventListener('change', (e) => {
    ipcRenderer.send('update-profile', {
        username: e.target.value,
        pfp: img
    });
})

ipcRenderer.on('load-profile', (event, data) => {
    document.querySelector('img#output').src = data.pfp === 'default' ? '../img/default-profile.svg' : data.pfp;
    document.querySelector('input#text').value = data.username;
    if (!img) img = 'default'
});