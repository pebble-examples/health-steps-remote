#include "main_window.h"

static Window *s_window;
static TextLayer *s_time_layer, *s_update_layer;

static TextLayer* make_text_layer(GRect bounds, GFont font) {
  TextLayer *this = text_layer_create(bounds);
  text_layer_set_background_color(this, GColorClear);
  text_layer_set_text_alignment(this, GTextAlignmentCenter);
  text_layer_set_font(this, font);
  return this;
}

static void window_load(Window *window) {
  Layer *root_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(root_layer);

  s_time_layer = make_text_layer(grect_inset(bounds, GEdgeInsets(40, 5)),
                     fonts_get_system_font(FONT_KEY_BITHAM_42_MEDIUM_NUMBERS));
  layer_add_child(root_layer, text_layer_get_layer(s_time_layer));

  s_update_layer = make_text_layer(grect_inset(bounds, GEdgeInsets(85, 5, 0, 5)),
                     fonts_get_system_font(FONT_KEY_GOTHIC_24));
  text_layer_set_text(s_update_layer, "Not yet updated");
  layer_add_child(root_layer, text_layer_get_layer(s_update_layer));
}

static void window_unload(Window *window) {
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_update_layer);

  window_destroy(s_window);
}

void main_window_push() {
  s_window = window_create();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  window_stack_push(s_window, true);
}

void main_window_update_time(struct tm *tick_time) {
  static char s_buffer[8];
  strftime(s_buffer, sizeof(s_buffer), "%H:%M", tick_time);
  text_layer_set_text(s_time_layer, s_buffer);
}

void main_window_set_updated_time() {
  time_t now = time(NULL);
  struct tm *time_now = localtime(&now);

  static char s_buffer[32];
  strftime(s_buffer, sizeof(s_buffer), "Last updated:\n%H:%M", time_now);
  text_layer_set_text(s_update_layer, s_buffer);
}
