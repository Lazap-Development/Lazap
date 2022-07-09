const signUpButton = document.getElementById('signUp');
const signInButton = document.getElementById('signIn');
const container = document.getElementById('container');
const alertbox = document.querySelector('.alert-box');
const alertboxcross = document.getElementById('alertboxexit');
const ignore = document.getElementById('ignoreBTN');
let userDataPath;
(async () => {
    const result = await ipcRenderer.invoke('read-path');
    userDataPath = result
})();

alertboxcross.addEventListener('click', () => {
	alertbox.style.display = 'none';
});
signUpButton.addEventListener('click', () => {
	container.classList.add('right-panel-active');
});

signInButton.addEventListener('click', () => {
	container.classList.remove('right-panel-active');
});

ignore.addEventListener('click', () => {
	const fs = require('fs');
	const parseJson = JSON.parse(fs.readFileSync(userDataPath + '/storage/Settings/LauncherData.json').toString());
	parseJson.skipLogin = true;
	fs.writeFileSync(userDataPath + '/storage/Settings/LauncherData.json', JSON.stringify(parseJson));
	console.log(parseJson);
});