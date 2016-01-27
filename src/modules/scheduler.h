#pragma once

#include <pebble.h>

#include "data.h"

typedef void(SchedulerEvent)(void);

void scheduler_begin(SchedulerEvent *callback, int interval_minutes);
