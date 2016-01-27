#pragma once

#include <pebble.h>

#include "config.h"

void data_init();

void data_deinit();

void data_reload_steps();

int* data_get_steps_data();
