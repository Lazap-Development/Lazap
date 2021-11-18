const electron = require('electron');
const app = electron.app;
require('v8-compile-cache');
app.commandLine.appendSwitch('auto-detect', 'false');
app.commandLine.appendSwitch('no-proxy-server')

app.on('ready', () => {
    const mainWindow = new electron.BrowserWindow({
        width: 950,
        height: 500,
        minWidth: 850,
        minHeight: 450,
        resizable: true,
        frame: false,
        show: false
    });
    mainWindow.loadFile('src/splash.html')

    mainWindow.once('ready-to-show', () => {
        mainWindow.show()
        
        setTimeout(() => {
            mainWindow.loadFile('src/index.html')
        }, 3000);
    })
    
});