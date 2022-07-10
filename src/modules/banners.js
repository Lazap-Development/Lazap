/* eslint-disable indent */
function fetch_banner(data, userDataPath) {
	/* const htmlparser = require('htmlparser2');
	const fetch = require('node-fetch'); */
	const arr = [];
	for (let i = 0; i < data.length; i++) {
		arr.push((async () => {
			if (data[i].LauncherName === 'EpicGames') {
				return '../img/icons/icon.ico';
				/* const response = await fetch(`https://www.epicgames.com/store/en-US/browse?q=${encodeURIComponent(data[i].DisplayName)}&sortBy=releaseDate&sortDir=DESC&count=5&category=Game&start=0`, {
					method: 'GET',
					responseType: 'arraybuffer',
				}).catch((err) => {
					console.warn(err.stack.slice(0, 500));
					return '../img/icons/icon.ico';
				});

				const dom = htmlparser.parseDocument(await response.text(), { 'decodeEntities': true });
				const elements = dom.children[1].children[1].children.filter(x => x.name === 'div' && x.children.length > 0)[0].children[0].children.filter(x => x.children.length > 0)[0].children.find(x => x.name === 'main').children[1].children[0].children[0].children[0].children[0].children[0].children[0].children[1].children[0].children.find(x => x.name === 'section').children[0].children[0].children.map(x => x.children[0].children[0].children[0].children[0].children[0].children[0].children[0].children[0].children[0].children[0]).filter(x => x.attribs.alt);
				let index;
				elements.forEach((element, _index) => {
					if (typeof index === 'number') return;
					if (element.attribs['alt'] === data[i].DisplayName) {
						index = _index;
					}
					else if (element.attribs['alt'].includes(data[i].DisplayName)) {
						index = _index;
					}
				});
				const element = elements[index] ?? elements[0];
				return element?.attribs['data-image'] ? element.attribs['data-image'] : '../img/icons/icon.ico'; */
			}
			else if (data[i].LauncherName === 'Steam') {
				return `https://cdn.akamai.steamstatic.com/steam/apps/${data[i].GameID}/library_600x900.jpg`;
				// return `https://steamcdn-a.akamaihd.net/steam/apps/${data[i].GameID}/library_600x900.jpg`;
			}
			else if (data[i].LauncherName === 'RiotGames') {
                                if (data[i].DisplayName == "Valorant") {
      				      return 'https://valorant-config.fr/wp-content/uploads/2020/05/7d604cf06abf5866f5f3a2fbd0deacf9-200x300.png';
                                }
                                if(data[i].DisplayName == "League of Legends") {
                                      return 'https://images.igdb.com/igdb/image/upload/t_cover_big/co49wj.png';
                                }
                                if(data[i].DisplayName == "Legends of Runeterra") {
                                      return 'https://images.igdb.com/igdb/image/upload/t_cover_big/co3wnv.png';
                                }
			}
			else if (data[i].LauncherName === 'Uplay') {
				return '../img/icons/icon.ico';
				/* let ubisoftified = data[i].DisplayName.replaceAll('_', ' ');
				if (data[i].DisplayName.replaceAll('_', ' ').match(/\d$/ig) && !data[i].DisplayName.replaceAll('_', ' ').replaceAll('\\d', '').endsWith(' ')) {
					const numlength = ubisoftified.split('').reverse().join('').match(/\d/ig)[0].length;
					ubisoftified = ubisoftified.slice(0, ubisoftified.length - numlength) + ' ' + ubisoftified.slice(ubisoftified.length - numlength);
				}

				/* Use Epic Games to get banners of Uplay Games for now, unless new alternative found *\/
				const response = await fetch(`https://www.epicgames.com/store/en-US/browse?q=${encodeURIComponent(ubisoftified)}&sortBy=releaseDate&sortDir=DESC&count=5&category=Game&start=0`, {
					method: 'GET',
				}).catch((err) => {
					console.warn(err.stack.slice(0, 500));
					return '../img/icons/icon.ico';
				});

				const dom = htmlparser.parseDocument(await response.text(), { 'decodeEntities': true });
				const elements = dom.children[1].children[1].children.filter(x => x.name === 'div' && x.children.length > 0)[0].children[0].children.filter(x => x.children.length > 0)[0].children.find(x => x.name === 'main').children[1].children[0].children[0].children[0].children[0].children[0].children[0].children[1].children[0].children.find(x => x.name === 'section').children[0].children[0].children.map(x => x.children[0].children[0].children[0].children[0].children[0].children[0].children[0].children[0].children[0].children[0]).filter(x => x.attribs.alt);
				let index;
				elements.forEach((element, _index) => {
					const name = element.attribs['alt'].split(' ').map(x => x.toLowerCase()).includes('edition') ? element.attribs['alt'].split(' ').slice(0, element.attribs['alt'].split(' ').length - element.attribs['alt'].split(' ').reverse().findIndex(x => x.toLowerCase() === 'edition') - 2).join(' ') : element.attribs['alt'];
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
				const element = elements[index] ?? elements[0];
				return element?.attribs['data-image'] ? element?.attribs['data-image'] : '../img/icons/icon.ico'; */
			}
			else if (data[i].LauncherName === 'Minecraft') {
				return 'https://image.api.playstation.com/vulcan/img/cfn/11307uYG0CXzRuA9aryByTHYrQLFz-HVQ3VVl7aAysxK15HMpqjkAIcC_R5vdfZt52hAXQNHoYhSuoSq_46_MT_tDBcLu49I.png';
			}
			else if (data[i].LauncherName === 'FiveM') {
				return 'https://logos-world.net/wp-content/uploads/2021/03/FiveM-Symbol.png';
			}
			else if (data[i].LauncherName === 'osu!') {
				return 'https://static.wikia.nocookie.net/osugame/images/4/41/Osu_new_logo.png/revision/latest?cb=20220105150330';
			}
			else if (data[i].LauncherName === 'Lunar') {
				return 'https://www.lunarclient.com/assets/img/default-twitter-icon.webp';
			}
			else if (data[i].LauncherName === 'XboxGames') {
				return data[i].Banner;
			}
		})());
	}

	cacheBanners(data, arr.filter(async x => { return (await x) !== '../img/icons/icon.ico';}), userDataPath);
	return arr;
}

function cacheBanners(data, res, userDataPath) {
	const fetch = require('node-fetch');
	const { checkForDirAndCreate } = require('../utils.js');

	checkForDirAndCreate(userDataPath + '/storage/Cache/Games/Images', '{}', userDataPath);

	res.filter(async (x) => (await x).startsWith('http')).forEach(async (x, i) => {
		fetch(await x, {
			method: 'GET',
		}).then(async (response) => {
			const md5 = require('md5');
			const fs = require('fs');
			fs.createWriteStream(userDataPath + `/storage/Cache/Games/Images/${md5(data[i].DisplayName)}.png`).write(Buffer.from(await response.arrayBuffer()));
		}).catch(() => '');
	});
}

module.exports = {
	fetch_banner,
	cacheBanners,
};
