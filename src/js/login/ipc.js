const { ipcRenderer } = require('electron');

// eslint-disable-next-line no-unused-vars
function load_main() {
	ipcRenderer.send('load-main');
}
// eslint-disable-next-line no-unused-vars
function close_window() {
	ipcRenderer.send('close-window');
}
// eslint-disable-next-line no-unused-vars
function max_window() {
	ipcRenderer.send('max-window');
}
// eslint-disable-next-line no-unused-vars
function min_window() {
	ipcRenderer.send('min-window');
}

// handle singup
document.querySelector('#container > div.form-container.sign-up-container > div > button').addEventListener('click', (e) => {
	const name = document.querySelector('#container > div.form-container.sign-up-container > div > input[type=text]:nth-child(2)');
	const email = document.querySelector('#container > div.form-container.sign-up-container > div > input[type=email]:nth-child(3)');
	const pass = document.querySelector('#container > div.form-container.sign-up-container > div > input[type=password]:nth-child(4)');

	if (name.value.length < 4) { // TODO: handle this all better
		return alert('Name should have atleast 2 or more characters');
	}
	else if (email.value.length < 2) {
		return alert('Email should have atleast 2 or more characters');
	}
	else if (pass.value.length < 6) {
		return alert('Password should have atleast 6 or more characters');
	}

	ipcRenderer.send('signup-request', {
		name: name.value,
		mail: email.value,
		pass: pass.value,
	});
	// e.target.disabled = true;
});

ipcRenderer.on('signup-response', (event, code) => {
	if (code === 200) {
		window.alert('Account made successfully! redirecting to signin...');
		const container = document.getElementById('container');
		container.classList.remove('right-panel-active');
	}
	else if (code === 401) {
		window.alert('There was an error making your account'); // ? idk what to put here, change it later ig
	}
	else if (code === 403) {
		window.alert('An account with the same email already exists! Head to signin');
	}
	else {
		window.alert('Something went wrong while trying to create an account!\n\nPossible Issues: You are offline, API was down, Unable to reach the API');
	}
});

// handle singin
document.querySelector('#container > div.form-container.sign-in-container > div > button').addEventListener('click', (e) => {
	const username = document.querySelector('#container > div.form-container.sign-in-container > div > input[type=text]:nth-child(2)');
	const pass = document.querySelector('#container > div.form-container.sign-in-container > div > input[type=password]:nth-child(3)');

	if (username.value.length < 4) { // TODO: handle this better
		return alert('Email should have atleast 4 or more characters');
	}
	else if (pass.value.length < 6) {
		return alert('Password should have atleast 6 or more characters');
	}

	ipcRenderer.send('signin-request', {
		username: username.value,
		pass: pass.value,
	});
});


ipcRenderer.on('signin-response', (event, code) => {
	if (code === 200) {
		window.alert('Singin successful!');
		// const container = document.getElementById('container');
		// container.classList.remove("right-panel-active");
		ipcRenderer.send('load-main');
	}
	else if (code === 401) {
		window.alert('There was an error singing you in');
	}
	else if (code === 402) {
		window.alert('An account with that email does\'nt even exists! Make sure that you typed in the correct email');
	}
	else if (code === 403) {
		window.alert('Wrong password!');
	}
	else {
		window.alert('Something went wrong while trying to login!\n\nPossible issues: You are not connected to internet, API was offline, API returned unexpected error(in which case you should consider updating the launcher)');
	}
});

ipcRenderer.on('alert', (e, msg) => {
	alert(msg);
});

ipcRenderer.on('replace-ignore-and-continue', () => {
	document.querySelector('button.ignore').textContent = 'Back To Launcher';
});

ipcRenderer.on('check-if-logged-in', async (e, r) => {
	const loggedIn = r;

	if (loggedIn.status === 'SUCCESS') {
		ipcRenderer.send('load-main', r);
	}
	else {
		alert('Something went wrong while trying to login!');
	}
});
