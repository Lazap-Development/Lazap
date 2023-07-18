<script>
// TODO benchmark everything to see what's causing so much delay DONE
// Classes
class GameElement {
	constructor(data, listID, jsondata, settings, bannerdirarr) {
		this.data = data;
		this.jsondata = jsondata; // Not to be used in events, only during initialisation(getHTMLElement)
		this.settings = settings; // Not to be used in events, only during initialisation(getHTMLElement)
		this.bannerdirarr = bannerdirarr;
		this.listID = listID;
		this.process = null;
	}

	async getHTMLElement() {
		const gameElement = this.getElement();
		const banner = await this.getBannerElement();
		gameElement.appendChild(banner);
		// eslint-disable-next-line no-self-assign
		this.data.Banner = this.data.Banner;
		if (this.listID.startsWith('recent') && this.listID.includes('Main')) return gameElement;
		const { enableLauncherIcons } = this.settings;
		if (enableLauncherIcons) gameElement.prepend(this.getLauncherIconElement());

		const gameBottom = document.createElement('div');
		gameBottom.classList.add('gamebox-bottom');
		gameElement.appendChild(gameBottom);
		gameBottom.appendChild(this.getTextElement());
		gameBottom.appendChild(await this.getStarElement());
		gameBottom.appendChild(this.getMenuElement());

		return gameElement;
	}

	getElement() {
		const element = document.createElement('div');
		element.id = `game-div-${this.data.DisplayName.replaceAll(' ', '_')}`;
		element.className += this.listID.startsWith('recent') && this.listID.includes('Main')
			? 'mainPageGamebox'
			: 'gamebox';
		element.style.display = 'table';
		return element;
	}

	async getBannerElement() {
		const element = document.createElement('img');

		const image = require('../assets/img/default-game-banner.png');
		element.setAttribute('src', image);

		element.classList.add('game_banner_img');
		element.height = 500;
		element.width = 500;

		let banner;
		let block = false;
		element.addEventListener('error', () => {
			block = true;
			if (this.data.LauncherName !== 'CustomGame' && banner) console.warn(`${this.data.DisplayName}'s banner did not load successfully`);
			const image = require('../assets/img/default-game-banner.png');
			element.setAttribute('src', image);
		});
		element.addEventListener('load', async () => {
			if (block) return;
			// Load banner from getBanner() after default banner has loaded
			banner = await this.getBanner();
			element.setAttribute('src', banner);
			element.style.content = 'none';
			this.cacheBanner(banner);
			block = true;
		});
		element.addEventListener('click', () => {
			this.handleLaunch();
			storage.addLaunch(this.data.GameID, this.data.LauncherName);
		});

		return element;
	}

	getTextElement() {
		const element = document.createElement('span');

		if (this.data.DisplayName.length > 17) {
			element.innerText = `${this.data.DisplayName.slice(0, 17)}...`;
		}
		else {
			element.innerText = this.data.DisplayName;
		}
		return element;
	}

	getMenuElement() {
		const element = document.createElement('div');
		element.classList.add('menu');
		element.id = 'menu';

		element.addEventListener('click', () => {
			const gamemenu = document.getElementById('gameMenu');
			gamemenu.style.display = gamemenu.style.display === 'flex' ? 'none' : 'flex';
			document.getElementById("gameMenuTitle").innerHTML = this.data.DisplayName 
		});

		return element;
	}

	async getStarElement() {
		const element = document.createElement('div');
		element.classList.add('star');
		element.id = 'star';

		// Check whether the game is starred or not
		const isFav = this.jsondata?.Favourite || false;
		if (isFav) {
			element.classList.add('star-fill');
			element.style.filter = 'invert(77%) sepia(68%) saturate(616%) hue-rotate(358deg) brightness(100%) contrast(104%)';
		} else {
			element.classList.remove('star-fill');
			element.style.filter = 'invert(100%) sepia(0%) saturate(1489%) hue-rotate(35deg) brightness(116%) contrast(100%)';
		}

		// Handle onclick
		element.addEventListener('click', async () => {
			const isFavourite = await storage.toggleFavourite(this.data.GameID, this.data.LauncherName);
			const ele = Array.from(document.getElementById(`game-div-${this.data.DisplayName.replaceAll(' ', '_')}`).children).find(x => x.classList.contains('gamebox-bottom')).children.namedItem('star');

			ele.style.filter = isFavourite
				? 'invert(77%) sepia(68%) saturate(616%) hue-rotate(358deg) brightness(100%) contrast(104%)'
				: 'invert(100%) sepia(0%) saturate(1489%) hue-rotate(35deg) brightness(116%) contrast(100%)';
			if (isFavourite) {
				ele.classList.add('star-fill');
				ele.classList.add('shake');
				setTimeout(() => ele.classList.remove('shake'), 500);
			}
			else {
				ele.classList.remove('star-fill');
			}
		});
		return element;
	}

	getLauncherIconElement() {
		const element = document.createElement('img');
		const image = require(`../assets/icons/${this.data.LauncherName}.png`);
		element.setAttribute('src', image);
		element.classList.add('gamebox-icon');

		return element;
	}

	async getBanner() {
		const bannersDir = this.bannerdirarr;
		const dispsha256 = await invoke('sha256', { content: this.data.DisplayName.replaceAll(' ', '_').replace(/[\u{0080}-\u{FFFF}/]/gu, '') });

		if (bannersDir.includes(`${dispsha256}.png`)) {
			return await tauri.convertFileSrc(`${storage.bannersDir}/${dispsha256}.png`);
		}
		else {
			switch (this.data.LauncherName) {
				case 'Steam': {
					return `https://cdn.akamai.steamstatic.com/steam/apps/${this.data.GameID}/library_600x900.jpg`;
				}
				case 'RiotGames': {
					return 'https://valorant-config.fr/wp-content/uploads/2020/05/7d604cf06abf5866f5f3a2fbd0deacf9-200x300.png';
				}
				case 'Minecraft': {
					return 'https://i.imgur.com/PJFx3U2.jpg';
				}
				case 'FiveM': {
					return 'https://logos-world.net/wp-content/uploads/2021/03/FiveM-Symbol.png';
				}
				case 'Lunar': {
					return 'https://pbs.twimg.com/profile_images/1608698913476812801/uLTLhANK_400x400.jpg';
				}
				case 'Lutris': {
					if (this.data.DisplayName.replaceAll(' ', '_') === 'Epic_Games_Store')
						return 'https://pcper.com/wp-content/uploads/2021/02/epic-games-store.png';
					if (this.data.DisplayName.replaceAll(' ', '_') === 'Rockstar_Games_Launcher')
						return 'https://cdn.player.one/sites/player.one/files/2019/08/26/rockstar-games.png';
					return;
				}
				case 'XboxGames': {
					return this.data.Banner;
				}
				case 'Osu': {
					return 'https://cdn2.steamgriddb.com/file/sgdb-cdn/grid/a5d7420f9fdc41087377b4d58c5fe94b.png';
				}
				case 'EpicGames': {
					const fetchEpicGame = await http.fetch(
						`https://api.rawg.io/api/games?key=f8854c401fed44b89f4e1e4faa56ccc8&search=${this.data.DisplayName.replaceAll(' ', '-')}&search_exact&search_precise&stores=11`,
						{
							method: 'GET',
							mode: 'no-cors',
							headers: {
								Accept: 'application/json',
								'Content-Type': 'application/json',
							},
						}
					);
					if (!fetchEpicGame.data.results[0]) break;
					return (
						fetchEpicGame.data.results[0].background_image.slice(0, 27) + '/crop/600/400' + fetchEpicGame.data.results[0].background_image.slice(27)
					);
				}
				case 'Uplay': {
					const fetchUplay = await http.fetch(
						`https://api.rawg.io/api/games?key=f8854c401fed44b89f4e1e4faa56ccc8&search=${this.data.DisplayName.replaceAll(' ', '-')}&search_exact&search_precise`,
						{
							method: 'GET',
							mode: 'no-cors',
							headers: {
								Accept: 'application/json',
								'Content-Type': 'application/json',
							},
						}
					);
					if (!fetchUplay.data.results[0]) break;
					return (
						fetchUplay.data.results[0].background_image.slice(0, 27) + '/crop/600/400' + fetchUplay.data.results[0].background_image.slice(27)
					);
				}
				case 'Rockstar': {
					return `https://media-rockstargames-com.akamaized.net/rockstargames-newsite/img/global/games/fob/640/${this.data.bannerId}.jpg`;
				}
			}
		}
	}

	async cacheBanner(banner) {
		if (this.data.LauncherName === 'CustomGame' || !banner) return;
		const bannersDir = await storage.readBannersDir();
		const dispsha256 = await invoke('sha256', { content: this.data.DisplayName.replaceAll(' ', '_').replace(/[\u{0080}-\u{FFFF}/]/gu, '') });

		if (bannersDir.includes(`${dispsha256}.png`)) {
			return await tauri.convertFileSrc(`${storage.bannersDir}/${dispsha256}.png`);
		}
		else {
			http.fetch(banner, {
				method: 'GET',
				headers: {
					'Content-Type': 'text/plain; charset=utf-8',
				},
				responseType: 3,
			}).then(async (response) => {
				if (response.status === 404 && this.data.LauncherName === 'Lutris') return;

				await invoke('write_binary_file', {
					filePath: `${storage.bannersDir}/${dispsha256}.png`,
					fileContent: response.data,
				});
				const banner = document.getElementById(`game-div-${this.data.DisplayName.replaceAll(' ', '_')}`)?.firstElementChild;
				banner?.setAttribute('src', tauri.convertFileSrc(`${storage.bannersDir}/${dispsha256}.png`));
				banner.style = 'content: none;';
				banner.addEventListener('error', () => (banner.style = ''));
			}).catch((e) => console.error(e));
		}

	}

	async handleLaunch() {
		let res;
		if (os === 'win32') {
			switch (this.data.LauncherName) {
				case 'EpicGames': {
					res = this.createProcess(`/C start com.epicgames.launcher://apps/${encodeURIComponent(this.data.LaunchID)}?action=launch&silent=true`);
					break;
				}
				case 'Steam': {
					res = this.createProcess(`/C start steam://rungameid/${this.data.GameID}`);
					break;
				}
				case 'Uplay': {
					res = this.createProcess(`/C start uplay://launch/${this.data.GameID}/0`);
					break;
				}
				case 'Minecraft': {
					res = this.createProcess(`/C powershell start "${this.data.Location}\\${this.data.Executable}"`);
					break;
				}
				case 'Lunar': {
					res = this.createProcess(`/C powershell start "${this.data.Location}\\${this.data.Executable}"`);
					break;
				}
				default: {
					res = this.createProcess(`/C powershell start "${this.data.Location}\\${this.data.Executable}"`);
					break;
				}
			}
		} else if (os === 'linux') {
			switch (this.data.LauncherName) {
				case 'Steam': {
					res = this.createProcess('steam', `steam://rungameid/${this.data.GameID} -silent`);
					break;
				}
				case 'Minecraft': {
					res = this.createProcess('minecraft-launcher');
					break;
				}
				case 'Lunar': {
					res = this.createProcess('lunarclient');
					break;
				}
				case 'Lutris': {
					res = this.createProcess('lutris', `lutris:rungameid/${this.data.LaunchID}`);
					break;
				}
				default: {
					res = this.createProcess(`"${this.data.Location}/${this.data.Executable}"`, this.data.Args);
					break;
				}
			}
		}
		return res;
	}

	async createProcess(exec, args = '') {
		// TODO: add back "ALREADY_RUNNING"
		VisibilityState({ LauncherName: this.data.LauncherName, DisplayName: this.data.DisplayName });
		invoke('launch_game', { exec, args }).then(() => {
			VisibilityState({ LauncherName: this.data.LauncherName, DisplayName: this.data.DisplayName });
		});

		async function VisibilityState() {
			try {
				const { tray_min_launch } = await storage.getSettings();
				if (tray_min_launch === true) {
					if ((await Window.appWindow.isVisible()) === true) {
						Window.appWindow.hide();
						if (!timestamp) timestamp = Date.now();
						try {
							await invoke(`set_rpc_activity`, {
								state: `Launcher: ${this.data.LauncherName}`,
								details: this.data.DisplayName,
								largeImage: this.data.LauncherName.toLowerCase(),
								largeText: 'Lazap',
								smallImage: 'lazap_icon',
								smallText: 'Lazap',
								timestamp: timestamp,
							});
						} catch (error) {
							console.error(error);
						}
					} else {
						Window.appWindow.show();
						const rpc = require('./modules/rpcOptions');
						const { details, largeText, smallImage, smallText } = rpc.selectOption(rpc.currentRpc);
						if (timestamp === null) timestamp = Date.now();
						try {
							await invoke(`set_rpc_activity`, {
								details,
								largeText,
								smallImage,
								smallText,
								timestamp: timestamp === null ? Date.now() : timestamp,
							});
						} catch (error) {
							console.error(error);
						}
					}
				}
			} catch (e) {
				return console.error(e);
			}
		}
	}
}

class Storage {
	constructor() {
		(async () => {
			this.appDataDir = await window.__TAURI__.path.appDir();
			this.gamesDataJSON = `${this.appDataDir}/cache/games/data.json`;
			this.bannersDir = `${this.appDataDir}/cache/games/banners`;
			this.settingsJSON = `${this.appDataDir}/LauncherData.json`
		})();
	}
	async setGamesData(games, source) {
		const data = await this.getGamesData();

		data.forEach((d) => {
			Object.keys(d).forEach((x) => {
				if ([undefined, null].includes(d[x])) delete d[x];
			});
		});
		games.forEach((d) => {
			Object.keys(d).forEach((y) => {
				if ([undefined, null].includes(d[y])) delete d[y];
			});
		});

		try {
			if (source === 'getInstalledGames') {
				if (data.length > 0) {
					const newdata = [];
					// data.filter(x => games.find(y => y.LauncherName === x.LauncherName && y.GameID === x.GameID));
					for (let i = 0; i < games.length; i++) {
						const gamedata = data.find(x => x.LauncherName === games[i].LauncherName && x.GameID === games[i].GameID);
						if (!gamedata) {
							newdata.push(gamedata);
						}
						else if (Object.keys(games[i]).length > Object.keys(gamedata).length) {
							data[data.findIndex(x => x.LauncherName === games[i].LauncherName && x.GameID === games[i].GameID)] = games[i];
						}
						else if (Object.keys(games[i]).length < Object.keys(gamedata).length) {
							let obj = {
								...gamedata,
							};
							for (let j = 0; j < Object.keys(games[i]).length; j++) {
								obj[Object.keys(games[i])[j]] = Object.values(games[i])[j];
							}
							data[data.findIndex(x => x.LauncherName === games[i].LauncherName && x.GameID === games[i].GameID)] = obj;
						}
					}
					newdata.push(...games.filter(x => !data.find(y => y.LauncherName === x.LauncherName && y.GameID === x.GameID)));
				}
				else {
					await invoke('write_file', { filePath: this.gamesDataJSON, fileContent: JSON.stringify(games) });
				}
			}
			else if (['toggleFavourite', 'addLaunch'].includes(source)) {
				await invoke('write_file', { filePath: this.gamesDataJSON, fileContent: JSON.stringify(games) });
			}
		}
		catch (e) {
			console.error(e);
		}
	}
	async getGamesData() {
		try {
			return JSON.parse(await invoke('read_file', { filePath: this.gamesDataJSON }));
		}
		catch (e) {
			console.error(e);
		}
	}
	async readBannersDir() {
		try {
			return await invoke('read_dir', { dirPath: this.bannersDir });
		}
		catch (e) {
			console.error(e);
		}
	}
	async toggleFavourite(GameID, LauncherName) {
		const data = await this.getGamesData();

		// Invert Favourite field of the game and save the data
		const game = data.find(x => x.GameID === GameID && x.LauncherName === LauncherName);
		game.Favourite = !game.Favourite;
		this.setGamesData(data, 'toggleFavourite');

		// Check if game exists in favourites game list, if so delete the element
		const list = document.getElementById('favGamesList');
		const element = list.children.namedItem(`game-div-${game.DisplayName.replaceAll(' ', '_')}`);
		if (game.Favourite === false && element) {
			element.classList.add('fadeOutUpNoDelay');
			setTimeout(() => {
				list.removeChild(element)
				if (document.getElementById("favGamesList").childNodes.length == 0) {
					document.getElementById("favGamesPlaceholder").style.display =
						"block";
				}
			}, 200);
		}

		return game.Favourite;
	}
	async getSettings() {
		try {
			return JSON.parse(await invoke('read_file', { filePath: this.settingsJSON }));
		}
		catch (e) {
			console.error(e);
		}
	}
	async addLaunch(GameID, LauncherName) {
		const data = await this.getGamesData();
		if (!data) return;
		const game = data.find((x) => x.GameID === GameID && x.LauncherName === LauncherName);
		game.LastLaunch = Date.now();
		game.Launches = typeof game.Launches === 'number' ? game.Launches + 1 : 1;
		this.setGamesData(data, 'addLaunch');
		if (
			!document.getElementById('recentGamesList').children.namedItem(`game-div-${game.DisplayName.replaceAll(' ', '_')}`)
		) {
			// eslint-disable-next-line no-undef
			// recentGamesList.replaceChildren([]);
			// this.loadGames('recentGamesList', null, data);
			// if (document.getElementById('recentGamesListMainPage').children.length < 5) {
			// eslint-disable-next-line no-undef
			//   recentGamesListMainPage.replaceChildren([]);
			//   this.loadGames("recentGamesListMainPage", null, data);
			// }
		}
	}
}

// Imports
const blacklists = require('./others/blacklist.json')
const invoke = window.__TAURI__.invoke;
const tauri = window.__TAURI__.tauri;
const http = window.__TAURI__.http;

const Window = window.__TAURI__.window;
// Objects
const storage = new Storage();
// Variables
let fetches = 0;
const loads = {};
let lists = ['allGamesList', 'recentGamesList', 'favGamesList'];
let timestamp;
let os;

export default {
	name: 'find-games',
	async mounted() {
		lists = lists.map(x => document.getElementById(x));
		os = await window.__TAURI__.os.platform();
	},
	methods: {
		// Fetches all installed games in the disk according to the Launchers provided
		async getInstalledGames(
			Launchers = ['CustomGames', 'EpicGames.js', 'Lutris.js', 'Minecraft.js', 'RiotGames.js', 'RockstarGames.js', 'Steam.js', 'Uplay.js'],
		) {
			// Prevents multiple simultaneous running of the function
			if (fetches > 1) {
				return [];
			}
			fetches++;
			// Fetches games from all launchers/*.js files
			let games = (await Promise.all(Launchers.filter(x => x.endsWith('.js')).map(x => require(`./launchers/${x}`).getInstalledGames()))).flat().filter(x => !blacklists.includes(x.GameID));

			// Adds Custom Games to the array if requested
			if (Launchers.includes('CustomGames')) {
				const data = await storage.getGamesData();
				games.push(...data.filter(x => x.LauncherName === 'CustomGame'));
			}

			// Save all games into JSON
			storage.setGamesData(games, 'getInstalledGames');

			fetches--;
			return games;
		},
		// Gets all games installed and then filters out games which are required for the specific list
		async getGames(listID, allgames) {
			const games = allgames || await this.getInstalledGames();
			if (listID === 'recentGamesListMainPage') {
				let data = await storage.getGamesData();
				data = data.filter(x => typeof x.Launches === 'number').sort((a, b) => b.Launches - a.Launches).slice(0, 5);

				// Add placeholder elements for the rest of the remaining spaces
				if (document.getElementsByClassName('placeholderGames').length < 1) {
					for (let i = 0; i < 5 - games.length; i++) {
						let element = document.createElement('div');
						element.classList.add('placeholderGames')
						document.getElementById('recentGamesListMainPage').append(element);
					}
				}

				return data;
			}
			else if (listID === 'recentGamesList') {
				let data = await storage.getGamesData();
				data = data.filter(x => typeof x.Launches === 'number').sort((a, b) => b.Launches - a.Launches);
				return data;
			}
			else if (listID === 'allGamesList') {
				return games.map(x => x.DisplayName).sort().map(x => games.find(y => y.DisplayName === x));
			}
			else if (listID === 'favGamesList') {
				let data = await storage.getGamesData();
				data = data.filter(x => typeof x.Favourite === 'boolean' && x.Favourite === true).map(x => x.DisplayName).sort().map(x => data.find(y => y.DisplayName === x));
				return data;
			}
		},
		async createGameElement(data, listID, jsondata, settings, bannersdirarr) {
			const element = await new GameElement(data, listID, jsondata, settings, bannersdirarr).getHTMLElement();
			return element;
		},
		async loadGames(listID, games) {
			const gamesdata = await storage.getGamesData();
			const bannerdirarr = await storage.readBannersDir();
			const settings = await storage.getSettings();
			const allgames = await this.getGames(listID, games);
			const list = document.getElementById(listID);
			const elements = [];

			if (loads[listID] === true) {
				return;
			}
			else {
				loads[listID] = true;
			}

			for (let i = 0; i < allgames.length; i++) {
				const element = list.children.namedItem(`game-div-${allgames[i].DisplayName.replaceAll(' ', '_')}`)
					?? await new GameElement(allgames[i], listID, gamesdata.find(x => x.GameID === allgames[i].GameID && x.LauncherName === allgames[i].LauncherName), settings, bannerdirarr).getHTMLElement();
				elements.push(element);
			}

			if (listID === 'recentGamesListMainPage' && allgames.length < 5) {
				for (let i = 0; i < 5 - allgames.length; i++) {
					let element = document.createElement('div');
					element.classList.add('placeholderGames')
					elements.push(element);
				}
			}

			list.replaceChildren(...elements);
			delete loads[listID];
		},
	},
};

</script>