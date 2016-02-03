#include "data.h"

static int s_data[MAX_ENTRIES];

int data_reload_steps() {
  if(TEST) {
    // Use dummy test data
    for(int i = 0; i < MAX_ENTRIES; i++) {
      s_data[i] = i * i;
    }
    return MAX_ENTRIES;
  }

  // Clear old data
  for(int i = 0; i < MAX_ENTRIES; i++) {
    s_data[i] = 0;
  }

  time_t end = time(NULL);
  time_t start = end - (MAX_ENTRIES * SECONDS_PER_MINUTE);

  // Last 15 minutes may not be available
  start -= (15 * SECONDS_PER_MINUTE);
  end -= (15 * SECONDS_PER_MINUTE);

  // Check data is available
  uint32_t num_records = 0;
  HealthServiceAccessibilityMask result = health_service_metric_accessible(HealthMetricStepCount, start, end);
  if(result == HealthServiceAccessibilityMaskAvailable) {
    // Read the data
    HealthMinuteData minute_data[MAX_ENTRIES];
    num_records = health_service_get_minute_history(&minute_data[0], MAX_ENTRIES, &start, &end);

    // Store it
    for(uint32_t i = 0; i < num_records; i++) {
      s_data[i] = (int)minute_data[i].steps;
    }
  } else {
    APP_LOG(APP_LOG_LEVEL_INFO, "No data available from %d to %d!", (int)start, (int)end);
  }

  APP_LOG(APP_LOG_LEVEL_INFO, "Got %d/%d new entries from the Health API", (int)num_records, MAX_ENTRIES);
  return (int)num_records;
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
