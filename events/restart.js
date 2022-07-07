module.exports = {
	name: 'restart', once: false, execute(mainWindow, tray, app) {
		app.relaunch();
		app.exit();
	},
};