const http = window.__TAURI__.http;
const fs = window.__TAURI__.fs;
const path = window.__TAURI__.path;
const tauri = window.__TAURI__.tauri;

async function getBanners(games) {
	const arr = [];
	for (let i = 0; i < games.length; i++) {
		arr.push((() => {
			switch (games[i].LauncherName) {
				case 'EpicGames': {
					return "https://media.sidefx.com/uploads/article/epic-games-invests-in-sidefx/epic_logo_black_banner3.jpg";
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
					return 'https://i.imgur.com/PJFx3U2.jpg';
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
					return;
				}
				case 'XboxGames': {
					return games[i].Banner;
				}
				case 'Osu' : {
					return "https://cdn2.steamgriddb.com/file/sgdb-cdn/grid/a5d7420f9fdc41087377b4d58c5fe94b.png";
				}
			}
		})());

	}

	cacheBanners(games.filter(x => !['Uplay'].includes(x.LauncherName)), arr.filter(x => x));
	return arr;
}

async function cacheBanners(data, res) {
	if(data?.length === 0) {
		document.getElementById('game-loading-overlay').style.opacity = '0';
		document.getElementById('game-loading-overlay').style.visibility = 'hidden';
		return console.log('%c[BANNER] ' + '%cNo games to process', "color:blue", "color:white");
	}
	const appDirPath = await path.appDir();
	const { sha256 } = require('../modules/sha256')
	const bannerBasePath = appDirPath + 'storage/cache/games/banners';
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
		return console.log('%c[BANNER] ' + '%cBanners are already loaded. Skipping.', "color:blue", "color:white");
	}

	if (res.length === 0) {
		document.getElementById('game-loading-overlay').style.opacity = '0';
		document.getElementById('game-loading-overlay').style.visibility = 'hidden';
		return console.log('%c[BANNER] ' + '%cNo banners to load.', "color:blue", "color:white");
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
		}).catch((e) => console.erro(e));

		fetchProcessed++;
		if (fetchProcessed === data.length) {
			document.getElementById('game-loading-overlay').style.opacity = '0';
			document.getElementById('game-loading-overlay').style.visibility = 'hidden';
			return console.log('%c[BANNER] ' + '%cJust finished processing banners.', "color:blue", "color:white");
		}
	});
}

export {
	getBanners,
}
