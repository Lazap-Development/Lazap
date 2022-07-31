const http = window.__TAURI__.http;
const fs = window.__TAURI__.fs;
const path = window.__TAURI__.path;
const tauri = window.__TAURI__.tauri;

async function getBanners(games) {
	const arr = [];
	for (let i = 0; i < games.length; i++) {
		arr.push((async () => {
			switch (games[i].LauncherName) {
				case 'EpicGames': {
					return;
				}
				case 'Steam': {
					return `https://cdn.akamai.steamstatic.com/steam/apps/${games[i].GameID}/library_600x900.jpg`;
				}
				case 'RiotGames': {
					return 'https://valorant-config.fr/wp-content/uploads/2020/05/7d604cf06abf5866f5f3a2fbd0deacf9-200x300.png';
				}
				case 'Uplay': {
					return;
				}
				case 'Minecraft': {
					return 'https://image.api.playstation.com/vulcan/img/cfn/11307uYG0CXzRuA9aryByTHYrQLFz-HVQ3VVl7aAysxK15HMpqjkAIcC_R5vdfZt52hAXQNHoYhSuoSq_46_MT_tDBcLu49I.png';
				}
				case 'FiveM': {
					return 'https://logos-world.net/wp-content/uploads/2021/03/FiveM-Symbol.png';
				}
				case 'Lunar': {
					return 'https://www.lunarclient.com/assets/img/default-twitter-icon.webp';
				}
				case 'Lutris': {
					if (games[i].DisplayName.replaceAll(' ', '_') === 'Epic_Games_Store') return 'https://pcper.com/wp-content/uploads/2021/02/epic-games-store.png'
					if (games[i].DisplayName.replaceAll(' ', '_') === 'Rockstar_Games_Launcher') return 'https://cdn.player.one/sites/player.one/files/2019/08/26/rockstar-games.png'
					return `https://thumbnails.pcgamingwiki.com/5/53/${games[i].DisplayName.replaceAll(' ', '_')}_-_cover.png/300px-${games[i].DisplayName.replaceAll(' ', '_')}_-_cover.png`
				}
				case 'XboxGames': {
					return games[i].Banner;
				}
			}
		})());

	}

	cacheBanners(games, arr.filter(async x => { return (await x) !== '../img/icons/icon.ico'; }));
	return arr;
}

async function cacheBanners(data, res) {
	const appDirPath = await path.appDir();
	const { sha256 } = require('../modules/sha256')
	const bannerBasePath = appDirPath + 'storage/Cache/Games/Images';
	const readBanners = (await fs.readDir(bannerBasePath)).map(x => x.name);

	let alreadyProcessed = false;
	let existingProcessed = 0;
	let fetchProcessed = 0;

	for (let i = 0; i < data.length; i++) {
		if (readBanners.includes(`${sha256(data[i].DisplayName.replaceAll(' ', '_'))}.png`)) {
			existingProcessed++;
			if (existingProcessed === data.length) {
				alreadyProcessed = true;
			}
		}
	}

	if (alreadyProcessed === true) {
		document.getElementById('game-loading-overlay').style.opacity = '0';
		document.getElementById('game-loading-overlay').style.visibility = 'hidden';
		return console.log('[BANNER] Banners are already loaded. Skipping.');
	}

	res.filter(async (x) => (await x)?.startsWith('http')).forEach(async (x, i) => {
		await http.fetch(await x, {
			method: 'GET',
			headers: {
				'Content-Type': 'text/plain; charset=utf-8'
			},
			responseType: 3
		}).then(async (response) => {
			if (response.status === 404 && data[i].LauncherName === 'Lutris') return;
			await fs.writeBinaryFile(bannerBasePath + `/${sha256(data[i].DisplayName.replaceAll(' ', '_'))}.png`, response.data);
			document.getElementById(`game-div-${data[i].DisplayName.replaceAll(' ', '_')}`)?.firstElementChild?.setAttribute('src', tauri.convertFileSrc(bannerBasePath + `/${sha256(data[i].DisplayName.replaceAll(' ', '_'))}.png`));
		}).catch((e) => console.log(e));

		fetchProcessed++;
		if (fetchProcessed === data.length) {
			document.getElementById('game-loading-overlay').style.opacity = '0';
			document.getElementById('game-loading-overlay').style.visibility = 'hidden';
			return console.log('[BANNER] Just finished processing banners.');
		}
	});
}

export {
	getBanners,
}
