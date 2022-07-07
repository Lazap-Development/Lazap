async function getInstalledGames(os = process.platform) {
	if (!navigator.onLine) return [];
	return new Promise(async (resolve) => {
		let { exec } = require('child_process');
		exec = require('util').promisify(exec);
		if (os === 'win32') {
			const { stdout, stderr } = await exec('"C:/Windows/System32/WindowsPowerShell/v1.0/powershell.exe" "get-appxpackage"');
			if (stderr) {
				console.log(stderr);
				resolve([]);
			}
			let apps = await parseRawToJSON(stdout).filter(x => x.IsFramework !== 'True' && x.SignatureKind === 'Store' && x.PublisherId !== '8wekyb3d8bbwe');
			apps = (await Promise.all(verifyGames(apps))).filter(async x => typeof (await x) === 'object');
			apps = apps.filter(x => x?.Banner);
			resolve(apps.map(x => parseGmeObject(x)));
		}
		else {
			resolve([]);
		}
	});
}

const parseRawToJSON = (res) => {
	return res.split('\r\n\r\n').slice(1).map(x => {
		return x.split('\r\n').filter(y => !y.startsWith(' ')).map(y => y.split(':', 3).map((z, i, arr) => `${arr.length === 3 && i !== 0 ? (i === 2 ? '' : '"') : '"'}${z.trim()}${arr.length === 3 && i !== 0 ? (i === 2 ? '"' : '') : '"'}`).join(':')).join(',\n').replaceAll('\\', '/');
	}).filter(x => x.length > 20).map(x => {
		return JSON.parse(`{${x}}`);
	});
}

const parseGmeObject = (obj) => {
	const fs = require('fs');
	const {
		Banner,
		InstallLocation,
	} = obj;
	const Location = InstallLocation;
	const manifest = fs.readFileSync(Location + '/AppxManifest.xml').toString().split('\r\n');
	const Executable = manifest.find(x => x.trim().startsWith('<Application ')).split(/<[/]{0,1}Application[>]{0,1}/)[1].match(/Executable="[a-z0-9\s-]{1,}\.exe"/i)[0];
	const DisplayName = manifest.find(x => x.trim().startsWith('<DisplayName>')).split(/<[/]{0,1}DisplayName>/)[1];

	return {
		Executable,
		Location,
		DisplayName,
		GameID: obj.PackageFullName,
		LaunchID: `start shell:AppsFolder/${obj.PackageFamilyName}!${manifest.find(x => x.trim().startsWith('<Application ')).split(/<[/]{0,1}Application[>]{0,1}/)[1].match(/Id="[a-z]{1,}"/i)[0]}`,
		Size: fs.statSync(Location).size,
		Banner,
		LauncherName: 'XboxGames',
	};
}

const verifyGames = (jsons) => {
	const arr = [];
	for (let i = 0; i < jsons.length; i++) {
		const json = jsons[i];
		arr.push(new Promise(async (resolve) => {
			if (!navigator.onLine) return resolve(false);
			json.Name = json.Name.split('.')[1];
			const fetch = (...args) => import('node-fetch').then(({ default: fetch }) => fetch(...args));
			const htmlparser = require('htmlparser2');
			const response = await fetch(`https://www.microsoft.com/en-in/search/shop/games?q=${encodeURIComponent(json.Name)}&devicetype=pc`);
			const dom = htmlparser.parseDocument(await response.text(), { 'decodeEntities': true });
			const list = dom.children.find(x => x.name === 'html').children.find(x => x.name === 'body').children.find(x => x.attribs ? x.attribs['data-grid'] === 'container pad-12x stack-2' : false).children.find(x => x.name === 'section').children.find(x => x.type === 'tag').children.find(x => x.attribs ? x.attribs.id.includes('productplacementlist') : false).children;
			if (list.find(x => x.name === 'p')) return resolve(false);

			const results = list.filter(x => x.type === 'tag').find(x => Object.values(x.attribs).length === 0).children.find(x => x.type === 'tag' && x.name === 'div' && x.attribs?.class).children.filter(x => x.type === 'tag');

			if (!results[0].children[1].children.find(x => x.type === 'tag' && x.attribs?.id?.startsWith('coreui')).children.find(x => x.name === 'h3').children[0].data.toLowerCase().includes(json.Name.toLowerCase())) return resolve();

			json.Banner = results[0].children[1].children.find(x => x.type === 'tag' && !x.attribs?.id?.startsWith('coreui')).children[1].children.find(x => x.name === 'img').attribs['data-src'];
			return resolve(json);
		}));
	}
	return arr;
}

module.exports = {
	getInstalledGames,
};
