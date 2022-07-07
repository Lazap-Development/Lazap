const fs = require('fs');
const eventHandler = (ipcMain, app) => {
	const eventFiles = fs
		.readdirSync('./events')
		.filter((file) => file.endsWith('.js'));
	for (const file of eventFiles) {
		const event = require(`../events/${file}`);
		event.once ? ipcMain.once(event.name, (...args) => event.execute(...args, app)) : ipcMain.on(event.name, (...args) => event.execute(...args, app));
	}
};

exports.eventHandler = eventHandler;