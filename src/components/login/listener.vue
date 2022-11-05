<script>
const fs = window.__TAURI__.fs;
const path = window.__TAURI__.path;
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

ignore.addEventListener('click', async () => {
	const appDirPath = await path.appDir();
	const parseJson = JSON.parse((await fs.readTextFile(appDirPath + 'storage/LauncherData.json')).toString());
	parseJson.skipLogin = true;
	fs.writeTextFile(appDirPath + 'storage/LauncherData.json', JSON.stringify(parseJson));
});
</script>