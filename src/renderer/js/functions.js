let img;
const usernameElementId = document.getElementById('input#text');

function loadFile(event) {
    var image = document.getElementById('output');
    image.src = URL.createObjectURL(event.target.files[0]);
    img = event.target.files[0].path;

    ipcRenderer.send('update-profile', {
        username: usernameElementId.value,
        pfp: event.target.files[0].path,
    });
}

document.getElementById('text').addEventListener('change', (e) => {
    ipcRenderer.send('update-profile', {
        username: e.target.value,
        pfp: img,
    });
});

ipcRenderer.on('load-profile', (event, data) => {
    document.getElementById('output').src =
    data.pfp === 'default' ? '../assets/img/default-profile.svg' : data.pfp;
    usernameElementId.value = data.username;
    if (!img) img = 'default';
});
