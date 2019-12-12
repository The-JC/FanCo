const electron = require('electron');
const  {ipcRenderer} = electron;

var count = 0;
const table = document.querySelector('tbody');
for(var i=0; i<3; i++) {
	const tr = document.createElement('tr');
	for(var j=0; j<6; j++) {
		const th = document.createElement('th');
		th.className = 'position';
		th.id = 'm' + count;
		const divNum = document.createElement('div');
		const divRPM = document.createElement('div');
		divNum.className = 'divNum';
		divNum.id = 't' + count;
		divRPM.className = 'divRPM';
		const pNum = document.createElement('p');
		pNum.className = 'pNum';
		pNum.innerHTML = count+1;
		const pSoll = document.createElement('p');
		pSoll.className = 'pSoll';
		pSoll.id = 's' + count;
		pSoll.innerHTML = 0;
		const pIst = document.createElement('p');
		pIst.className = 'pIst';
		pIst.id = 'i' + count;
		pIst.innerHTML = 0;
		divNum.appendChild(pNum);
		divRPM.appendChild(pSoll);
		divRPM.appendChild(pIst);
		th.appendChild(divNum);
		th.appendChild(divRPM);
		tr.appendChild(th);
		count++;
	}
	table.appendChild(tr);
}

ipcRenderer.on('item:rpm', function(e, item){
	for(var i=0; i<item.length; i++) {
		document.getElementById('s' + i).innerHTML = item[i].sollRPM;
		document.getElementById('i' + i).innerHTML = item[i].istRPM;
		if(item[i].istRPM == 0) {
			document.getElementById('t' + i).style.backgroundColor = '#b13e53';
		} else {
			document.getElementById('t' + i).style.backgroundColor = '#38b764';
		}
	}
});
ipcRenderer.on('item:position', function(e, item){
	for(var i=0; i<18; i++) {
		if(item == i) {
			document.getElementById('m' + i).style.border = '3px solid #f4f4f4';
		} else {
			document.getElementById('m' + i).style.border = '3px solid #566c86';
		}
	}
});
ipcRenderer.on('error:send', function(e, err, desc) {
	document.getElementById('desc').innerHTML = desc;
	document.getElementById('err').innerHTML = err;
	document.getElementById('error').style.display = 'inline';
});
ipcRenderer.on('error:clear', (e) => {
	document.getElementById('error').style.display = 'none';
});