#include <pebble.h>
#include "common.h"
#include "watchface_time.h"
#include "watchface_battery.h"
#include "watchface_timeline.h"

static void main_layer_update_proc(Layer *layer, GContext *ctx) {
  draw_time(layer, ctx);
  draw_battery(layer, ctx);
  draw_timeline(layer, ctx);
}

static void main_window_load(Window *window) {
  s_main_layer = layer_create(GRect(0, 0, screen_width, screen_height));
  layer_set_update_proc(s_main_layer, main_layer_update_proc);
  layer_add_child(window_get_root_layer(window), s_main_layer);

  update_time();
}

static void main_window_unload(Window *window) {
  layer_destroy(s_main_layer);
}

static void init() {
  s_main_window = window_create();

  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);

  window_stack_push(s_main_window, true);
}

static void deinit() {
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
