var http = require('http');
var url = require('url');

var PORT = 8080;
var DEBUG = false;

/********************************** Helpers ***********************************/

function info(content) {
  console.log(content);
}

function debug(content) {
  if(DEBUG) info(content);
}

/************************************ App *************************************/

function onNewData(data) {
  var items = data.split(',');
  info('Got data: ' + data + ' (' + items.length + ' items)');

  // Do stuff with the data
}

function onRequest(request, response) {
  var reqUrl = request.url;
  debug('Got a request: ' + reqUrl);

  if(reqUrl.indexOf('/analytics') > -1) {
    // App request
    var url_parts = url.parse(reqUrl, true);
    var query = url_parts.query;
    var data = query.data;

    onNewData(data);    

    response.end('OK\n');
  } else {
    response.end('Error\n');
  }
}

function setupServer() {
  var server = http.createServer(onRequest);
  server.listen(PORT, function() {
    info('Server started listening on port ' + PORT);
  });
}

setupServer();
