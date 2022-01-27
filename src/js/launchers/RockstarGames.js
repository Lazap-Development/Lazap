// eslint-disable-next-line no-unused-vars
const { exec } = require('child_process');
const pexec = require('util').promisify(exec);

async function getInstalledGames(os = process.platform) {
	if (os === 'linux') {
		const checkIfInstalled = exec('find . -name Launcher.exe | head -n 1');

		// Check if Rockstar Games is already installed
		if (checkIfInstalled) {
			const { VTexec } = require('open-term');

			VTexec(`"${checkIfInstalled}"`);
		}
		else {
			const findWine = exec('which wine');

			if (!findWine == '/usr/bin/wine') {
				alert(
					'Since you are on linux, You need to have winehq installed. Please refer to your Distro package manager and install it.',
				);
				return;
			}
			else {
				alert(
					'The setup for Rockstar Games is going to start now. \n\nPlease make sure you remember (or manualy set) the path you want to install rockstar games in. \n\nHit Ok to contniue once you are ready',
				);

				const { VTexec } = require('open-term');
				VTexec(
					'(cd ~ && curl -O https://gamedownloads.rockstargames.com/public/installer/Rockstar-Games-Launcher.exe && wine $HOME/Rockstar-Games-Launcher.exe)',
					{
						linux: {
							priorityTerms: [
								'xterm',
								'guake',
								'konsole',
								'xfce',
							],
							terms: ['xterm', 'guake', 'konsole', 'xfce'],
						},
					},
				);
			}
		}
	}
	else if (os === 'win32') {
		let allGames = (await pexec('Reg query "HKEY_LOCAL_MACHINE\\SOFTWARE\\Rockstar Games" /s')).stdout?.split('\n');
	}
}
