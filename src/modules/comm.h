#pragma once

#include <pebble.h>

#include "data.h"

#include "../windows/main_window.h"

typedef enum {
  AppKeyIndex = 0,
  AppKeyData,
  AppKeyNumDataItems,
  AppKeyJSReady
} AppKey;

void comm_init(int inbox, int outbox);

void comm_begin_upload(int num_items);
