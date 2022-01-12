async function findGames() {
	const { exec } = require('child_process');

	console.log(exec('which steam'));
}

module.exports = {
	findGames,
};
