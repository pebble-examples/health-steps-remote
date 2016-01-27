# health-steps-remote

Basic app that can send steps data to a remote server. Default behavior is to
upload the last 15 minutes worth of minute data after that interval, but can be
configured in `src/config.h`.

The server receiving the data is selected with the `NODE_SERVER_URL` variable in
`src/js/pebble-js- app.js`, and in this example is a simple Node server with a
function (`onNewData()`) broken out to allow use of the received data array as
preferred.
