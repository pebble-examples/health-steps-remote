var VERSION = "1.0";
var DEBUG = false;
var NODE_SERVER_URL = '10.132.31.209:8080';

var lastData = [];
var numItems = 0;

/********************************** Helpers ***********************************/

function info(content) {
  console.log(content);
}

function debug(content) {
  if(DEBUG) info(content);
}

function ajax(url, type, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback(this.responseText);
  };
  xhr.open(type, url);
  xhr.send();
};

/******************************** PebbleKit JS ********************************/

Pebble.addEventListener('ready', function() {
  info('PebbleKit JS ready! Version ' + VERSION);
});

Pebble.addEventListener('appmessage', function(dict) {
  debug('Got appmessage: ' + JSON.stringify(dict.payload));

  if(dict.payload['AppKeyNumDataItems'] != undefined) {
    numItems = dict.payload['AppKeyNumDataItems'];
    debug('Number of items: ' + numItems);
  }

  if(dict.payload['AppKeyIndex'] != undefined) {
    var index = dict.payload['AppKeyIndex'];

    if(index == 0) {
      // New data set
      debug('Clearing data');
      lastData = [];
    }

    // Store this data item
    var value = dict.payload['AppKeyData'];
    lastData[index] = value;
    debug('Got item ' + index + ': ' + value);

    if(index == numItems - 1) {
      // Last item, transmit to server
      info('Last item received. Uploading...');

      // Convert to string
      var str = '' + lastData[0];
      for(var i = 1; i < numItems; i++) {
        str += ',' + lastData[i];
      }
      info('Sending data string: ' + str);

      var url = 'http://' + NODE_SERVER_URL + '/analytics?data=' + str;
      ajax(url, 'GET', function(responseText) {
        console.log('Server response: ' + responseText);
      });
    }
  }
});
