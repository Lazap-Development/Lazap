const axios = require('axios').default;
const fs = require('fs');
const { ipcMain } = require('electron');
let mainWindow;
ipcMain.on('signup-request', async (e, data) => {
	mainWindow.webContents.send('signup-response', await handleSignup(data));
});
ipcMain.on('signin-request', async (e, data) => {
	mainWindow.webContents.send('signin-response', await handleSignin(data));
});
ipcMain.on('login-identify', async () => {
	mainWindow.webContents.send('login-identify-response', await identify());
});

const { editLocalStorage } = require('../utils.js');

const handleSignup = async (data) => {
	let deniedCode;
	const res = await axios.post('http://localhost:3000/accounts/add-account', data).catch(e => {
		deniedCode = e.response?.status;
	});
	return res ? res.status : deniedCode;
}

const handleSignin = async (data) => {
	let deniedCode;
	const res = await axios.post('http://localhost:3000/accounts/login', data).catch(e => {
		deniedCode = e.response?.status;
	});

	if (res) {
		// console.log(res.data);
		if (fs.existsSync('./storage/Settings/userprofile.json')) {
			const json = JSON.parse(fs.readFileSync('./storage/Settings/userprofile.json').toString());
			json['token'] = res.data;
			json['password'] = data.pass;
			json['username'] = data.username;
			editLocalStorage(json);
		}
		else {
			editLocalStorage({
				token: res.data,
			});
		}

		return res.status;
	}
	else {
		return deniedCode || null;
	}
}

const identify = async () => {
	if (!fs.existsSync('./storage/Settings/userprofile.json')) return { status: 'ACCOUNT_NOT_FOUND', data: null };
	const { token, password, username } = JSON.parse(fs.readFileSync('./storage/Settings/userprofile.json').toString());
	const res = await axios.post('http://localhost:3000/accounts/identify', { token, pass: password, name: username }).catch((e) => e.response?.status || 0);

	const errcodes = {
		401: 'INVALID_BODY',
		402: 'ACCOUNT_NOT_FOUND',
		200: 'SUCCESS',
		0: 'OFFLINE/API_DOWN',
	};

	return { data: res.data, status: errcodes[res.request?.res.statusCode] || 'OFFLINE/API_DOWN' };
}

module.exports = {
	handleSignin,
	handleSignup,
	identify,
	_setup: (win) => mainWindow = win,
};