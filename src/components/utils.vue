<script>
const fs = window.__TAURI__.fs;

const handleStorageAndTransportData = (mainWindow, userDataPath) => {
    let LauncherData = JSON.parse(fs.readFileSync(userDataPath + '/storage/Settings/userprofile.json').toString());
    if (!Object.keys(LauncherData).length) {
        const os = require('os');
        LauncherData = {
            username: os.userInfo().username,
            pfp: 'default',
        };
        fs.writeFileSync(userDataPath + 'storage/Settings/userprofile.json', JSON.stringify(LauncherData));
    }
    else {
        if (LauncherData.pfp !== 'default' && !fs.existsSync(LauncherData.pfp)) {
            LauncherData.pfp = 'default';
        }
        fs.writeFileSync(userDataPath + 'storage/Settings/userprofile.json', JSON.stringify(LauncherData));
    }

    mainWindow.webContents.send('load-profile', LauncherData);
};

module.exports = {
    handleStorageAndTransportData,
};
</script>