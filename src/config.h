#pragma once

#include <pebble.h>

// Number of data items to transmit to JS.
#define DATA_NUM_ENTRIES 15

// Number of minutes between uploads, and the number of items read from
// the HealthService minute data API.
#define INTERVAL_MINUTES 15

// Send squares test data instead of data from the HealthService minute
// minute data API
#define TEST false
