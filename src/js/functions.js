/* eslint-disable no-undef */
let img;

// eslint-disable-next-line no-unused-vars
function loadFile(event) {
	const image = document.getElementById('output');
	image.src = URL.createObjectURL(event.target.files[0]);
	img = event.target.files[0].path;

	ipcRenderer.send('update-profile', {
		username: document.getElementById('text').value,
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
	document.getElementById('output').src = data.pfp === 'default' ? '../img/default-profile.svg' : data.pfp;
	document.getElementById('text').value = data.username;
	if (!img) img = data.pfp;
});


ipcRenderer.on('check-for-login', async (e, r) => {
	console.log('e');
	const loggedInDiv = document.querySelector('div#loggedIn');
	loggedInDiv.addEventListener('click', () => {
		ipcRenderer.send('load-login');
	});
	const res = r;
	if (res.status === 'SUCCESS') {
		loggedInDiv.textContent = 'You are logged in!';
	}
	else {
		loggedInDiv.textContent = 'You are not logged in!';
	}

	loggedInDiv.textContent += `\n Click here to login in ${loggedInDiv.textContent.includes('not') ? '' : 'from different account'}`;
});