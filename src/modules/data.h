#pragma once

#include <pebble.h>

#include "config.h"

typedef enum {
  PersistKeyLastUploadTime = 0
} PersistKey;

void data_init();

void data_deinit();

int data_reload_steps();

int* data_get_steps_data();

void data_record_last_upload_time();

time_t data_get_last_upload_time();
