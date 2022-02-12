const fs = require('fs');

const signUpButton = document.getElementById('signUp');
const signInButton = document.getElementById('signIn');
const container = document.getElementById('container');
const alertbox = document.querySelector('.alert-box');
const alertboxcross = document.getElementById('alertboxexit');
const ignore = document.getElementById('ignoreBTN');

alertboxcross.addEventListener('click', function() {
	alertbox.style.display = 'none';
});
signUpButton.addEventListener('click', () => {
	container.classList.add('right-panel-active');
});

signInButton.addEventListener('click', () => {
	container.classList.remove('right-panel-active');
});

ignore.addEventListener('click', () => {
	const parseJson = JSON.parse(fs.readFileSync('./storage/Settings/LauncherData.json').toString());
	parseJson.skipLogin = true;
	fs.writeFileSync('./storage/Settings/LauncherData.json', JSON.stringify(parseJson));
	console.log(parseJson);
});