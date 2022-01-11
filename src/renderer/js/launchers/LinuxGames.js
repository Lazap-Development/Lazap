async function findGames() {
    var { exec } = require('child_process');

    console.log(exec('which steam'));
}

module.exports = {
    findGames,
};
