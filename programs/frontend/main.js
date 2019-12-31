
const electron = require('electron');
const url = require('url');
const path = require('path');
const request = require('request');
const gpio = require('onoff').Gpio;

const {app, BrowserWindow, ipcMain, globalShortcut} = electron;

const NUMFANS = 18;
const API = 'http://localhost:5000/'

const button = new gpio(16, 'in', 'rising', {debounceTimeout: 10});
const status = new gpio(20, 'in');
const clock = new gpio(21, 'in', 'rising');
const vcc = new gpio(12, 'high');

let errorState = 0;

let mainWindow;
let position=0;
let modi=0;
let data = new Array();
let cData = new Array();

//listen for app to be ready
app.on('ready', function(){
	for(var i=0; i<18; i++) {
		data[i] = new Object();
		data[i]['sollRPM'] = 0;
		data[i]['istRPM'] = 0;
	}
	for(var i=0; i<18; i++) {
		cData[i] = new Array();
	}
	//create new window
	mainWindow = new BrowserWindow({
		width: 800,
		height: 480,
		frame: false,
		webPreferences: {
			nodeIntegration: true
		}
	});
	mainWindow.setFullScreen(true);
	loadMainWindow();
	
	ipcMain.on('item:callback', function(e, item){
		mainWindow.webContents.send('item:fan', position);
		mainWindow.webContents.send('item:rpm', data);
		mainWindow.webContents.send('item:data', cData[position]);
	});
	setInterval(handleTimer, 2000);
});

function loadMainWindow() {
	//load html into window
	mainWindow.loadURL(url.format({
		pathname: path.join(__dirname, 'index.html'),
		protocol: 'file:',
		slashes: true
	}));
}

function loadInfoWindow() {
	//load html into window
	mainWindow.loadURL(url.format({
		pathname: path.join(__dirname, 'info.html'),
		protocol: 'file:',
		slashes: true
	}));
}

function handleTimer() {
	request(API+'api/fans', function(err, res, body) {
		if(err) errorHandler(err);
		try {
			var obj = JSON.parse(body);
		} catch(e) {
			errorHandler(e);
			return;
		}
		// clearErrorState();
		for(var i=0; i<18; i++) {
			data[i].istRPM = obj[i];
			cData[i][cData[i].length] = obj[i];
			if(cData[i].length > 120) {
				cData[i].shift()
			}
		}
		mainWindow.webContents.send('item:rpm', data);
		mainWindow.webContents.send('item:data', cData[position]);
	});
}

app.on('ready', () => {
	globalShortcut.register("Space", () => {
		buttonHandler();
	});
	globalShortcut.register("Left", () => {
		encoderHandler(0);
	});
	globalShortcut.register("Right", () => {
		encoderHandler(1);
	});
});

function buttonHandler() {
	if(modi==0) {
		loadInfoWindow();
		modi=1;
	} else if(modi==1) {
		loadMainWindow();
		modi=0;
		position=0;
	}
}

/**
 * Handling encoder actions
 * @param {num} dir 0 equals left by one, 1 equals right by one
 */
function encoderHandler(dir) {
	switch(modi) {
		case 0:
			if(dir)
				if(position>=NUMFANS-1) position=0; else ++position;	// increment positon by 1
			else
				if(position<=0) position=NUMFANS-1; else --position; 	// decrement position by 1
			mainWindow.webContents.send('item:position', position);
			break;
		case 1:
				data[position].sollRPM += dir ? 1 : -1;
				mainWindow.webContents.send('item:rpm', data);

				request.put({
					url: API+ 'api/fans/' + position,
					json: {
						"set_speed": data[position].sollRPM
					}
					}, (err, res, body) => {
						if(err)
							errorHandler(err);
					}
				);
				break;
		default:
			throw 'Invalid mode';
	}
}

button.watch(function(err, value) {
	if(err) {
		console.log('ERROR!');
		return;
	}
	buttonHandler();
});

clock.watch(function(err, value) {
	if(err) {
		console.log('ERROR!');
		return;
	}
	if(status.readSync() === 0) {
		//links
		encoderHandler(0);
	} else {
		//rechts
		encoderHandler(1);
	}
});

function errorHandler(err, desc='') {
	if(errorState==1) return;
	errorState=1;
	mainWindow.webContents.send('error:send', err.message, desc);
}

function clearErrorState() {
	errorState=0;
	mainWindow.webContents.send('error:clear');
}