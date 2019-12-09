const electron = require('electron');
const url = require('url');
const path = require('path');
const request = require('request');
const gpio = require('onoff').Gpio;

const {app, BrowserWindow, ipcMain} = electron;

const button = new gpio(16, 'in', 'rising', {debounceTimeout: 10});
const status = new gpio(20, 'in');
const clock = new gpio(21, 'in', 'rising');
const vcc = new gpio(12, 'high');

let mainWindow;
let position=0;
let modi=0;
let data = new Array();
let cData = new Array();

//listen for app to be ready
app.on('ready', function(){
	for(var i=0; i<18; i++) {
		data[i] = new Object();
		data[i]['sollRPM'] = 2000+i;
		data[i]['istRPM'] = 0;
	}
	for(var i=0; i<18; i++) {
		cData[i] = new Array();
	}
	//create new window
	mainWindow = new BrowserWindow({
		//width: 800,
		//height: 480,
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
	request('http://localhost:5000/api/fans', function(err, res, body) {
		var obj = JSON.parse(body);
		for(var i=0; i<18; i++) {
			data[i].istRPM = obj[i];
			cData[i][cData[i].length] = obj[i];
		}
		mainWindow.webContents.send('item:rpm', data);
		mainWindow.webContents.send('item:data', cData[position]);
	});
}

button.watch(function(err, value) {
	if(err) {
		console.log('ERROR!');
		return;
	}
	//btn
	if(modi==0) {
		loadInfoWindow();
		modi=1;
	} else if(modi==1) {
		loadMainWindow();
		modi=0;
		position=0;
	}
});

clock.watch(function(err, value) {
	if(err) {
		console.log('ERROR!');
		return;
	}
	if(status.readSync() === 0) {
		//links
		if(modi==0) {
			if(position<=0) {
				position=17;
			} else {
				position--;
			}
			mainWindow.webContents.send('item:position', position);
		} else if(modi==1 && data[position].sollRPM != 0) {
			data[position].sollRPM-=1;
			mainWindow.webContents.send('item:rpm', data);
			//post request
			var arr = new Array();
			arr[0] = position;
			arr[1] = data[position].sollRPM;
			request.put({
				url: 'http://localhost:5000/api/fans/'+(position+1),
				form: arr
				}, function(err, res, body) {
				console.log(res);
			});
		}
	} else {
		//rechts
		if(modi==0) {
			if(position>=17) {
				position=0;
			} else {
				position++;
			}
			mainWindow.webContents.send('item:position', position);
		} else if(modi==1) {
			data[position].sollRPM+=1;
			mainWindow.webContents.send('item:rpm', data);
			//post request
			var arr = new Array();
			arr[0] = position;
			arr[1] = data[position].sollRPM;
			request.put({
				url: 'http://localhost:5000/api/fans/'+(position+1),
				form: arr
				}, function(err, res, body) {
				console.log(res);
			});
		}
	}
});