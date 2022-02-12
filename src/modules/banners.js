/* eslint-disable indent */
const { JSDOM } = require('jsdom');
const md5 = require('md5');
const axios = require('axios').default;
const fs = require('fs');
const path = require('path');
const APP_BASE_PATH = path.join(__dirname, path.relative(__dirname, './'));

const { checkForDirAndCreate } = require('../utils.js');

let isOffline = checkOffline();
let lastCheck;

function fetch_banner(data) {
	const arr = [];
	for (let i = 0; i < data.length; i++) {
		arr.push((async () => {
			if (lastCheck + 1000 * 60 * 5 < Date.now()) {
				await checkOffline();
				lastCheck = Date.now();
			}
			if (isOffline) return '../icon.ico';
			if (data[i].LauncherName === 'EpicGames') {
				const response = await axios({
					url: `https://www.epicgames.com/store/en-US/browse?q=${encodeURIComponent(data[i].DisplayName)}&sortBy=releaseDate&sortDir=DESC&count=5&category=Game&start=0`,
					method: 'GET',
					responseType: 'arraybuffer',
				}).catch((err) => {
					console.warn(err.stack.slice(0, 500));
					return '../icon.ico';
				});

				const dom = new JSDOM(response.data);
				const elements = dom.window.document.querySelectorAll('#dieselReactWrapper > div > div > main > div > div > div > div > div > section > div > section > div > section > section > ul > li > div > div > div > a > div > div > div > div > div > img[alt]');
				let index;
				elements.forEach((element, _index) => {
					if (typeof index === 'number') return;
					if (element.getAttribute('alt') === data[i].DisplayName) {
						index = _index;
					}
					else if (element.getAttribute('alt').includes(data[i].DisplayName)) {
						index = _index;
					}
				});
				const element = elements.item(index) ?? elements.item(0);
				return element?.getAttribute('data-image') ? element?.getAttribute('data-image') : '../icon.ico';
			}
			else if (data[i].LauncherName === 'Steam') {
				return `https://cdn.akamai.steamstatic.com/steam/apps/${data[i].GameID}/library_600x900.jpg`;
			}
			else if (data[i].LauncherName === 'RiotGames') {
				return 'https://valorant-config.fr/wp-content/uploads/2020/05/7d604cf06abf5866f5f3a2fbd0deacf9-200x300.png';
			}
			else if (data[i].LauncherName === 'Uplay') {
				let ubisoftified = data[i].DisplayName.replaceAll('_', ' ');
				if (data[i].DisplayName.replaceAll('_', ' ').match(/\d$/ig) && !data[i].DisplayName.replaceAll('_', ' ').replaceAll('\\d', '').endsWith(' ')) {
					const numlength = ubisoftified.split('').reverse().join('').match(/\d/ig)[0].length;
					ubisoftified = ubisoftified.slice(0, ubisoftified.length - numlength) + ' ' + ubisoftified.slice(ubisoftified.length - numlength);
				}

				/* Use Epic Games to get banners of Uplay Games for now, unless new alternative found */
				const response = await axios({
					url: `https://www.epicgames.com/store/en-US/browse?q=${encodeURIComponent(ubisoftified)}&sortBy=releaseDate&sortDir=DESC&count=5&category=Game&start=0`,
					method: 'GET',
					responseType: 'arraybuffer',
				}).catch((err) => {
					console.warn(err.stack.slice(0, 500));
					return '../icon.ico';
				});
				const dom = new JSDOM(response.data);
				const elements = dom.window.document.querySelectorAll('#dieselReactWrapper > div > div > main > div > div > div > div > div > section > div > section > div > section > section > ul > li > div > div > div > a > div > div > div > div > div > img[alt]');
				let index;
				elements.forEach((element, _index) => {
					const name = element.getAttribute('alt').split(' ').map(x => x.toLowerCase()).includes('edition') ? element.getAttribute('alt').split(' ').slice(0, element.getAttribute('alt').split(' ').length - element.getAttribute('alt').split(' ').reverse().findIndex(x => x.toLowerCase() === 'edition') - 2).join(' ') : element.getAttribute('alt');
					if (typeof index === 'number') return;

					const matched = name.toLowerCase().split('').map((str, _i) =>
						str === ubisoftified.toLowerCase().split('')[_i],
					).filter(x => x === true).length;

					// This makes it prioritize results that matched 100%, 90%, 75%
					if (((matched / name.length) * 100) === 100 && ubisoftified.toLowerCase() === name.toLowerCase()) {
						index = _index;
					}
					else if (((matched / name.length) * 100) > 90 && ((matched / name.length) * 100) !== 100) {
						index = _index;
					}
					else if (((matched / name.length) * 100) > 75 && !((matched / name.length) * 100) > 90) {
						index = _index;
					}
				});
				const element = elements.item(index) ?? elements.item(0);
				return element?.getAttribute('data-image') ? element?.getAttribute('data-image') : '../icon.ico';
			}
			else if (data[i].LauncherName === 'Minecraft') {
				return 'https://image.api.playstation.com/vulcan/img/cfn/11307uYG0CXzRuA9aryByTHYrQLFz-HVQ3VVl7aAysxK15HMpqjkAIcC_R5vdfZt52hAXQNHoYhSuoSq_46_MT_tDBcLu49I.png';
			}
			else if (data[i].LauncherName === 'FiveM') {
				return 'https://logos-world.net/wp-content/uploads/2021/03/FiveM-Symbol.png';
			}
			else if (data[i].LauncherName === 'Lunar') {
				return 'https://www.lunarclient.com/assets/img/default-twitter-icon.webp';
			}
		})());
	}

	cacheBanners(data, arr.filter(async x => {
		return (await x) !== '../icon.ico';
	}));
	return arr;
}

async function checkOffline() {
	await axios.get('https://google.com').then(() => isOffline = false).catch(() => isOffline = true);
}

function cacheBanners(data, res) {
	checkForDirAndCreate(APP_BASE_PATH + '/storage/Cache/Games/Images');

	res.filter(async (x) => (await x).startsWith('http')).forEach(async (x, i) => {
		axios({
			url: (await x),
			method: 'GET',
			responseType: 'stream',
		}).then((response) => {
			response.data.pipe(fs.createWriteStream(`./storage/Cache/Games/Images/${md5(data[i].DisplayName)}.png`));
		}).catch(() => '');
	});
}

module.exports = {
	fetch_banner,
	cacheBanners,
};