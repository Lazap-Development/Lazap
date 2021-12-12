const { ipcRenderer } = require("electron");

function load_main() {
    ipcRenderer.send("load-main")
}

function close_window() {
    ipcRenderer.send("close-window")
}

function max_window() {
    ipcRenderer.send("max-window")
}

function min_window() {
    ipcRenderer.send("min-window")
}

// handle singup
document.querySelector('#container > div.form-container.sign-up-container > div > button').addEventListener('click', (e) => {
    const name = document.querySelector('#container > div.form-container.sign-up-container > div > input[type=text]:nth-child(2)');
    const email = document.querySelector('#container > div.form-container.sign-up-container > div > input[type=email]:nth-child(3)');
    const pass = document.querySelector('#container > div.form-container.sign-up-container > div > input[type=password]:nth-child(4)');

    if (name.value.length < 2) {  // TODO: handle this all better
        return alert('Name should have atleast 2 or more characters');
    } else if (email.value.length < 2) {
        return alert('Email should have atleast 2 or more characters');
    } else if (pass.value.length < 2) {
        return alert('Password should have atleast 2 or more characters');
    }

    ipcRenderer.send('signup-request', {
        name: name.value,
        mail: email.value,
        pass: pass.value
    });
    // e.target.disabled = true;
});

ipcRenderer.on('signup-response', (event, code) => {
    if (code === 200) {
        window.alert('Account made successfully! redirecting to signin...');
        const container = document.getElementById('container');
        container.classList.remove("right-panel-active");
    } else if (code === 401) {
        window.alert('There was an error making your account'); // ? idk what to put here, change it later ig
    } else if (code === 403) {
        window.alert('An account with the same email already exists! Head to signin');
    }
});

// handle singin
document.querySelector('#container > div.form-container.sign-in-container > div > button').addEventListener('click', (e) => {
    const email = document.querySelector('#container > div.form-container.sign-in-container > div > input[type=email]:nth-child(2)');
    const pass = document.querySelector('#container > div.form-container.sign-in-container > div > input[type=password]:nth-child(3)');

    if (email.value.length < 2) {   // TODO: handle this better
        return alert('Email should have atleast 2 or more characters');
    } else if (pass.value.length < 2) {
        return alert('Password should have atleast 2 or more characters');
    }

    ipcRenderer.send('signin-request', {
        mail: email.value,
        pass: pass.value
    });
    // e.target.disabled = true;
});


ipcRenderer.on('signin-response', (event, code) => {
    if (code === 200) {
        window.alert('Singin successful!');
        // const container = document.getElementById('container');
        // container.classList.remove("right-panel-active");
    } else if (code === 401) {
        window.alert('There was an error singing you in');
    } else if (code === 402) {
        window.alert('An account with that email does\'nt even exists! Make sure that you typed in the correct email');
    } else if (code === 403) {
        window.alert('Wrong password!');
    }
});