#include "data.h"

static int *s_data;

void data_init() {
  s_data = (int*)malloc(DATA_NUM_ENTRIES * sizeof(int));
}

void data_deinit() {
  free(s_data);
}

void data_reload_steps() {
  if(TEST) {
    // Use dummy test data
    for(int i = 0; i < DATA_NUM_ENTRIES; i++) {
      s_data[i] = i * i;
    }
    return;
  }

  // Clear old data
  data_deinit();
  data_init();
  HealthMinuteData *minute_data = (HealthMinuteData*)malloc(DATA_NUM_ENTRIES * sizeof(HealthMinuteData));

  time_t end = time(NULL);
  time_t start = end - (DATA_NUM_ENTRIES * SECONDS_PER_MINUTE);

  // Check data is available
  HealthServiceAccessibilityMask result = health_service_metric_accessible(HealthMetricStepCount, start, end);
  if(result == HealthServiceAccessibilityMaskAvailable) {
    uint32_t num_records = health_service_get_minute_history(minute_data, DATA_NUM_ENTRIES, &start, &end);
    APP_LOG(APP_LOG_LEVEL_INFO, "Got %d new entries from the Health API", (int)num_records);

    // Store it
    for(uint32_t i = 0; i < num_records; i++) {
      s_data[i] = (int)minute_data[i].steps;
    }
  } else {
    APP_LOG(APP_LOG_LEVEL_ERROR, "No data available from %d to %d!", (int)start, (int)end);
  }

  free(minute_data);
}

int* data_get_steps_data() {
  return s_data;  
}

void data_record_last_upload_time() {
  time_t now = time(NULL);
  persist_write_int(PersistKeyLastUploadTime, (int)now);
}

time_t data_get_last_upload_time() {
  return (time_t)persist_read_int(PersistKeyLastUploadTime);
}
