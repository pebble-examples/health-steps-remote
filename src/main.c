#include <pebble.h>

#include "config.h"

#include "modules/comm.h"
#include "modules/data.h"
#include "modules/scheduler.h"

#include "windows/main_window.h"

static void upload_event() {
  // Get last minute data
  int num_records = data_reload_steps();

  if(num_records == 0) {
    APP_LOG(APP_LOG_LEVEL_INFO, "No new data");
    return;
  }

  // Send to JS
  if(connection_service_peek_pebble_app_connection()) {
    APP_LOG(APP_LOG_LEVEL_INFO, "Beginning upload...");
    comm_begin_upload(num_records);

    time_t now = time(NULL);
    main_window_set_updated_time(now);
    data_record_last_upload_time();
  } else {
    APP_LOG(APP_LOG_LEVEL_ERROR, "Could not send data, connection unavailable");
  }
}

static void health_handler(HealthEventType event, void *context) {
  // Update step count
  HealthMetric metric = HealthMetricStepCount;
  HealthServiceAccessibilityMask result = 
    health_service_metric_accessible(metric, time_start_of_today(), time(NULL));
  int steps = 0;
  if(result == HealthServiceAccessibilityMaskAvailable) {
    steps = (int)health_service_sum_today(metric);
  }
  main_window_update_steps(steps);
}

static void tick_handler(struct tm *tick_time, TimeUnits changed) {
  main_window_update_time(tick_time);
}

static void init() {
  comm_init(64, 64);

  main_window_push();

  scheduler_begin(upload_event, INTERVAL_MINUTES);

  if(!health_service_events_subscribe(health_handler, NULL)) {
    APP_LOG(APP_LOG_LEVEL_ERROR, "Health not available!");
  }

  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  time_t now = time(NULL);
  struct tm *time_now = localtime(&now);
  tick_handler(time_now, MINUTE_UNIT);
}

static void deinit() { }

int main() {
  init();
  app_event_loop();
  deinit();
}
