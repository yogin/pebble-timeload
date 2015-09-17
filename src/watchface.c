#include <pebble.h>

static int screen_width = 144;
static int screen_height = 168;

static Window *s_main_window;
static Layer *s_main_layer;
static int *current_hour_idx;
static int *current_minute_idx;

static void update_time() {
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);

  // each hour_index represents 1 hour
  // convert hours from 24h format to 12h format
  *current_hour_idx = tick_time->tm_hour > 12 ? tick_time->tm_hour - 12 : tick_time->tm_hour;
  // each minute_index represents 5 minutes
  *current_minute_idx = tick_time->tm_min / 5; 

  APP_LOG(APP_LOG_LEVEL_DEBUG, "updated time %d:%d into hour_idx:%d minute_idx:%d", tick_time->tm_hour, tick_time->tm_min, *current_hour_idx, *current_minute_idx);
  layer_mark_dirty(s_main_layer);
}


static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void main_layer_update_proc(Layer *layer, GContext *ctx) {
  GRect bounds = layer_get_bounds(layer);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "bounds size: %dx%d", bounds.size.w, bounds.size.h);

  int block_border = 1;
  int block_spacer = 2 * block_border;
  int block_width = (bounds.size.w - (2*block_border) - block_spacer) / 2;
  int block_height = (bounds.size.h - (2*block_border) - (11*block_spacer)) / 12;
  APP_LOG(APP_LOG_LEVEL_DEBUG, "block %dx%d border:%d spacer:%d", block_width, block_height, block_border, block_spacer);

  graphics_context_set_stroke_color(ctx, GColorBlack);
  graphics_context_set_fill_color(ctx, GColorBlack);

  // draw hour blocks
  for (int i = 0, x1 = block_border, x2 = block_border + block_width + block_spacer, start_y = block_border; i < 12; i++) {
    // left column
    if (12-i <= *current_hour_idx) {
      graphics_fill_rect(ctx, GRect(x1, start_y, block_width, block_height), 2, GCornersAll);
    }
    else {
      graphics_draw_round_rect(ctx, GRect(x1, start_y, block_width, block_height), 2);
    }

    // right column
    if (12-i <= *current_minute_idx) {
      graphics_fill_rect(ctx, GRect(x2, start_y, block_width, block_height), 2, GCornersAll);
    }
    else {
      graphics_draw_round_rect(ctx, GRect(x2, start_y, block_width, block_height), 2);  
    }

    // move down
    start_y += block_height + block_spacer;
  }
}

static void main_window_load(Window *window) {
  current_hour_idx = (int*)malloc(sizeof(int));
  current_minute_idx = (int*)malloc(sizeof(int));

  s_main_layer = layer_create(GRect(0, 0, screen_width, screen_height));
  layer_set_update_proc(s_main_layer, main_layer_update_proc);
  layer_add_child(window_get_root_layer(window), s_main_layer);

  update_time();
}

static void main_window_unload(Window *window) {
  layer_destroy(s_main_layer);
  free(current_hour_idx);
  free(current_minute_idx);
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
