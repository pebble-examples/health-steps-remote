#include "scheduler.h"

static SchedulerEvent *s_callback;
static AppTimer *s_timer;
static int s_interval_minutes;

static void timer_handler(void *context) {
  s_callback();

  s_timer = app_timer_register(s_interval_minutes * 60 * 1000, timer_handler, NULL);
}

void scheduler_begin(SchedulerEvent callback, int interval_minutes) {
  s_callback = callback;
  s_interval_minutes = interval_minutes;

  s_timer = app_timer_register(s_interval_minutes * 60 * 1000, timer_handler, NULL);
}
