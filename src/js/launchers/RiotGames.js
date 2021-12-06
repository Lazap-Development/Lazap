module.exports = {
    getInstalledGames,
    parseGameObject,
};

const fs = require('fs');
const { promisify } = require('util');
let { exec } = require('child_process');
exec = promisify(exec);

async function getInstalledGames(os = process.platform) {
    let launcher_location;
    if (os === 'win32') {
        const { stdout, err } = await exec('Reg query HKEY_CLASSES_ROOT\\riotclient\\DefaultIcon');
        if (err) {
            console.error('There was an error while trying to find Riot Games: \n' + require('util').inspect(err, { depth: 1 }));
            alert('There was an error while finding Riot Games.');
        }
        launcher_location = stdout.split('"')[1];
    }
    return [await parseGameObject(launcher_location)];
}

async function parseGameObject(path){
    const Executable = 'RiotClientServices.exe';
    const Location = path.slice(0, -22);
    const Args = ["--launch-product=valorant", "--launch-patchline=live"];
    const DisplayName = 'Valorant';
    const Size = await promisify(fs.stat)(Location.slice(0, -12) + 'VALORANT').size;

    return {
        Executable,
        Location,
        Args,
        DisplayName,
        Size,
        LauncherName: 'RiotGames',
        GameID: 'Valorant',
    };
}