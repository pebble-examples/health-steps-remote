#pragma once

#include <pebble.h>

#include "data.h"

typedef enum {
  AppKeyIndex = 0,
  AppKeyData
} AppKey;

void comm_init(int inbox, int outbox);

void comm_begin_upload();
