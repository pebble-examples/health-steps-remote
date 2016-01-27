#include "scheduler.h"

static SchedulerEvent *s_callback;
static AppTimer *s_timer;

static void timer_handler(void *context) {
  s_callback();

  s_timer = app_timer_register(INTERVAL_MINUTES * 60 * 1000, timer_handler, NULL);
}

void scheduler_begin(SchedulerEvent callback, int interval_minutes) {
  s_callback = callback;

  // Adjust for time since last launch
  time_t delta_minutes = (time(NULL) - data_get_last_upload_time()) / SECONDS_PER_MINUTE;
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Last update was %d minutes ago", (int)delta_minutes);

  int initial_interval_ms;
  if(delta_minutes < INTERVAL_MINUTES) {
    // Last update was less than INTERVAL_MINUTES ago
    initial_interval_ms = (INTERVAL_MINUTES - delta_minutes) * 60 * 1000;
  } else {
    // Default interval
    initial_interval_ms = INTERVAL_MINUTES * 60 * 1000;
  }

  s_timer = app_timer_register(initial_interval_ms, timer_handler, NULL);
}
