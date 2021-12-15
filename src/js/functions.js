let img;

function loadFile(event) {
    console.log(event)
    var image = document.getElementById("output");
    image.src = URL.createObjectURL(event.target.files[0]);
    img = event.target.files[0].path;

    ipcRenderer.send('update-profile', {
        username: document.querySelector('input#text').value,
        pfp: event.target.files[0].path
    });
}

document.querySelector('#text').addEventListener('change', (e) => {
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
const axios = require('axios').default;
const fs = require('fs');

async function loggedIn() {
    const loggedInDiv = document.querySelector('div#loggedIn');
    loggedInDiv.addEventListener('click', () => {
        ipcRenderer.send('load-login');
    });

    const res = await identify();
    if (res.status === 'SUCCESS') {
        loggedInDiv.textContent = 'You are logged in!';
    }
    else {
        loggedInDiv.textContent = 'You are not logged in!';
    }

    loggedInDiv.textContent += `\n Click here to login in ${loggedInDiv.textContent.includes('not') ? '' : 'from different account'}`;
}
loggedIn();

async function identify() {
    let res;

    if (!fs.existsSync(__dirname.split('\\').slice(0, -1).join('\\') + '\\storage\\userprofile.json')) return { status: 'ACCOUNT_NOT_FOUND', data: null };
    const { token } = JSON.parse(fs.readFileSync(__dirname.split('\\').slice(0, -1).join('\\') + '\\storage\\userprofile.json').toString());
    res = await axios.post('http://localhost:3000/accounts/identify', { token }).catch((e) => e.response?.status || 0);

    const errcodes = {
        401: 'INVALID_BODY',
        402: 'ACCOUNT_NOT_FOUND',
        200: 'SUCCESS',
        0: 'OFFLINE/API_DOWN'
    };
    return { data: res.data, status: errcodes[res.request?.res.statusCode] || 'OFFLINE/API_DOWN' };
}