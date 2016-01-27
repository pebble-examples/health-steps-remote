#include "comm.h"

static int s_index;

static void send_data_item(int index) {
  int *data = data_get_steps_data();

  DictionaryIterator *out;
  if(app_message_outbox_begin(&out) == APP_MSG_OK) {
    dict_write_int(out, AppKeyIndex, &index, sizeof(int), true);
    dict_write_int(out, AppKeyData, &data[s_index], sizeof(int), true);

    // Include the total number of data items
    if(s_index == 0) {
      const int num_items = DATA_NUM_ENTRIES;
      dict_write_int(out, AppKeyNumDataItems, &num_items, sizeof(int), true);
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

  if(s_index < DATA_NUM_ENTRIES) {
    // Send next item
    send_data_item(s_index);
  } else {
    APP_LOG(APP_LOG_LEVEL_INFO, "Upload complete!");
  }
}

void comm_init(int inbox, int outbox) {
  app_message_register_outbox_sent(outbox_sent_handler);
  app_message_open(inbox, outbox);
}

void comm_begin_upload() {
  s_index = 0;
  send_data_item(s_index);
}
