function fetch_banner(data, userDataPath) {
	/* const htmlparser = require('htmlparser2');
	const fetch = require('node-fetch'); */
	const arr = [];

	/*function isConnected() {
		const obj = window.__TAURI__.os.networkInterfaces();
		const arr = Object.entries(obj);
		let result = false;
		for (let a of arr) {
			a[1] = a[1].filter(x => x.internal === false);
			result = a[1].length !== 0;
			// console.log(a[1]);
			/* for (const xd of a[1]) {
				if(xd.family === 'IPv6') return;
				console.log(xd);
			}
		}
		console.log(result);
	}*/

	for (let i = 0; i < data.length; i++) {
		arr.push((async () => {
			// const { fetch } = require('@tauri-apps/api/http')
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
			else if (data[i].LauncherName === 'Lutris') {
				let url = `https://lutris.net/games/banner/${data[i].GameID}.jpg`;
				/*if(!isConnected()) {
					url = '../img/icons/icon.ico';
				} else {
					await fetch(`https://lutris.net/games/banner/${data[i].GameID}.jpg`).then(res => {
						console.log(res.status, typeof res.status);
						if(res.status === 404) return url = 'https://cdn2.steamgriddb.com/file/sgdb-cdn/thumb/ac3f115b3a14f86b904bdc5ed5f82c4b.jpg'
					});
				//}*/
				return url;
			}
			else if (data[i].LauncherName === 'Steam') {
				let url = `https://cdn.akamai.steamstatic.com/steam/apps/${data[i].GameID}/library_600x900.jpg`;
				/*if(!isConnected()) {
					url = '../img/icons/icon.ico';
				} else {
					await fetch(`https://cdn.akamai.steamstatic.com/steam/apps/${data[i].GameID}/library_600x900.jpg`).then(res => {
						console.log(res.status, typeof res.status);
						if(res.status === 404) return url = 'https://www.sketchappsources.com/resources/source-image/roberto-steam-logo.png';
					});*/
				if(data[i].DisplayName === 'FrostRunner') return url = 'https://cdnb.artstation.com/p/assets/covers/images/026/711/505/large/david-rosario-iii-david-rosario-iii-frostrunner-banner-small.jpg?1589504830';
				//}
				return url;
			}
			else if (data[i].LauncherName === 'RiotGames') {
				if (data[i].DisplayName === "Valorant") {
					let url = 'https://valorant-config.fr/wp-content/uploads/2020/05/7d604cf06abf5866f5f3a2fbd0deacf9-200x300.png';
					/*if(!isConnected()) {
						url = '../img/icons/icon.ico';
					}*/
					return url;
				}
				if(data[i].DisplayName === "League of Legends") {
					let url = 'https://images.igdb.com/igdb/image/upload/t_cover_big/co49wj.png';
					/*if(!isConnected()) {
						url = '../img/icons/icon.ico';
					}*/
					return url;
				}
				if(data[i].DisplayName === "Legends of Runeterra") {
					let url = 'https://images.igdb.com/igdb/image/upload/t_cover_big/co3wnv.png';
					/*if(!isConnected()) {
						url = '../img/icons/icon.ico';
					}*/
					return url;
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
				let url = 'https://image.api.playstation.com/vulcan/img/cfn/11307uYG0CXzRuA9aryByTHYrQLFz-HVQ3VVl7aAysxK15HMpqjkAIcC_R5vdfZt52hAXQNHoYhSuoSq_46_MT_tDBcLu49I.png';
				/*if(!isConnected()) {
						url = '../img/icons/icon.ico';
					}*/
				return url;
			}
			else if (data[i].LauncherName === 'FiveM') {
				let url = 'https://logos-world.net/wp-content/uploads/2021/03/FiveM-Symbol.png';
				/*if(!isConnected()) {
						url = '../img/icons/icon.ico';
					}*/
				return url;
			}
			else if (data[i].LauncherName === 'Lunar') {
				let url = 'https://www.lunarclient.com/assets/img/default-twitter-icon.webp';
				/*if(!isConnected()) {
						url = '../img/icons/icon.ico';
					}*/
				return url;
			}
			else if (data[i].LauncherName === 'XboxGames') {
				return data[i].Banner;
			}
		})());
	}

	cacheBanners(data, arr.filter(async x => {
		return (await x) !== '../img/icons/icon.ico';
	}), userDataPath).then(() => console.log("cm"));
	return arr;
}
// eslint-disable-next-line
async function cacheBanners(data, el, userDataPath) {
	const http = window.__TAURI__.http;
	// const { checkForDirAndCreate } = require('../utils.js');
	const fs = window.__TAURI__.fs;
	// checkForDirAndCreate(userDataPath + '/storage/Cache/Games/Images', '{}', userDataPath);

	el.filter(async (x) => (await x).startsWith('http')).forEach(async (x, i) => {
		await http.fetch(await x, {
			method: 'GET',
			responseType: 3
		}).then(async (response) => {
			fs.writeBinaryFile(`${data[i].DisplayName}.png`, Buffer.from(await response.arrayBuffer()), { dir: `${userDataPath}storage/Cache/Games/Images/` })
				.then(e => console.log(e));
			// fs.createWriteStream(userDataPath + `/storage/Cache/Games/Images/${data[i].DisplayName}.png`).write(Buffer.from(await response.arrayBuffer()));
		}).catch(() => console.error("doesn't work lol"));
			//.then(async (rese) => await fs.writeBinaryFile(userDataPath + `storage/Cache/Games/Images/${data[i].DisplayName}.png`, Buffer.from(await rese.arrayBuffer())))
			//.then((resp) => resp.blob())
			//.then(blob => {
			//	const imageObjectURL = URL.createObjectURL(blob);
			//	console.log(imageObjectURL);
			//})
			//.catch(() => console.log("L u suck can't even use fs to write to a file"));
	});
}

module.exports = {
	fetch_banner,
	cacheBanners,
};