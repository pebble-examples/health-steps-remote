#include "comm.h"

static int s_index, s_num_records;

static void send_data_item(int index) {
  int *data = data_get_steps_data();

  DictionaryIterator *out;
  if(app_message_outbox_begin(&out) == APP_MSG_OK) {
    dict_write_int(out, AppKeyIndex, &index, sizeof(int), true);
    dict_write_int(out, AppKeyData, &data[s_index], sizeof(int), true);

    // Include the total number of data items
    if(s_index == 0) {
      dict_write_int(out, AppKeyNumDataItems, &s_num_records, sizeof(int), true);
    }

    if(app_message_outbox_send() != APP_MSG_OK) {
      APP_LOG(APP_LOG_LEVEL_ERROR, "Error sending message");
    }
  } else {
    APP_LOG(APP_LOG_LEVEL_ERROR, "Error beginning message");
  }
}

static void outbox_sent_handler(DictionaryIterator *iter, void *context) {
  // Last message was successful
  s_index++;

  if(s_index < s_num_records) {
    // Send next item
    send_data_item(s_index);
  } else {
    APP_LOG(APP_LOG_LEVEL_INFO, "Upload complete!");
  }
}

static void inbox_received_handler(DictionaryIterator *iter, void *context) {
  Tuple *js_ready_t = dict_find(iter, AppKeyJSReady);
  if(js_ready_t) {
    // Check that it has been at least INTERVAL_MINUTES since the last upload
    time_t now = time(NULL);
    if(now - data_get_last_upload_time() > INTERVAL_MINUTES * 60) {
      // Send the first data
      int num_items = data_reload_steps();
      comm_begin_upload(num_items);

      // Remember the upload time
      data_record_last_upload_time();
      main_window_set_updated_time(now);
    } else {
      APP_LOG(APP_LOG_LEVEL_DEBUG, "Last update was less than %d minutes ago", (int)INTERVAL_MINUTES);
    }
  }
}

void comm_init(int inbox, int outbox) {
  app_message_register_inbox_received(inbox_received_handler);
  app_message_register_outbox_sent(outbox_sent_handler);
  app_message_open(inbox, outbox);
}

void comm_begin_upload(int num_records) {
  s_index = 0;
  s_num_records = num_records;

  send_data_item(s_index);
}
