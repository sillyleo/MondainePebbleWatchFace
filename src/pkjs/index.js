console.log=function(strOutput){};

var Clay = require('pebble-clay');
var clayConfig = require('./config');
var clay = new Clay(clayConfig);

Pebble.addEventListener('webviewclosed', function(e) {
	if (e && !e.response) {
		return;
	}
	// Get the keys and values from each config item
	console.log(e.response);
	var dict = clay.getSettings(e.response);
	dict.TYPE = "config";
	// Send settings values to watch side
	Pebble.sendAppMessage(dict, function(e) {
		console.log('Sent config data to Pebble');
	}, function(e) {
		console.log('Failed to send config data!');
		console.log(JSON.stringify(e));
	});
});
