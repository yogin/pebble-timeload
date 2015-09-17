#include <pebble.h>
#include "common.h"
#include "watchface_time.h"

int hour_idx;
int minute_idx;

void update_time() {
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);

  // each hour_index represents 1 hour
  // convert hours from 24h format to 12h format
  hour_idx = tick_time->tm_hour > 12 ? tick_time->tm_hour - 12 : tick_time->tm_hour;
  // each minute_index represents 5 minutes
  minute_idx = tick_time->tm_min / 5; 

  APP_LOG(APP_LOG_LEVEL_DEBUG, "updated time %d:%d into hour_idx:%d minute_idx:%d", tick_time->tm_hour, tick_time->tm_min, hour_idx, minute_idx);
  layer_mark_dirty(s_main_layer);
}

void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

void draw_time(Layer *layer, GContext *ctx) {
  GRect bounds = layer_get_bounds(layer);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "bounds size: %dx%d", bounds.size.w, bounds.size.h);

  int block_spacer = 2 * border;

  int time_area_width = bounds.size.w;
  int time_area_height = bounds.size.h;
  
  if (show_battery) {
    time_area_width -= sidebar_width + border;
  }

  if (show_timeline) {
    time_area_width -= sidebar_width + 2 * border;
  }

  APP_LOG(APP_LOG_LEVEL_DEBUG, "time area size: %dx%d", time_area_width, time_area_height);
  
  int block_width = (time_area_width - (2 * border) - block_spacer) / 2;
  int block_height = (time_area_height - (2 * border) - (11*block_spacer)) / 12;
  APP_LOG(APP_LOG_LEVEL_DEBUG, "block %dx%d border:%d spacer:%d", block_width, block_height, border, block_spacer);

  graphics_context_set_stroke_color(ctx, GColorBlack);
  graphics_context_set_fill_color(ctx, GColorBlack);

  int offset_x = border;
  if (show_battery) {
      offset_x += sidebar_width + block_spacer;
  }
  
  // draw hour blocks
  for (int i = 0, x1 = offset_x, x2 = offset_x + block_width + block_spacer, start_y = border; i < 12; i++) {
    // left column
    GRect left_rect = GRect(x1, start_y, block_width, block_height);
    if (12-i <= hour_idx) {
      graphics_fill_rect(ctx, left_rect, 2, GCornersAll);
    }
    else {
      graphics_draw_round_rect(ctx, left_rect, 2);
    }

    // right column
    GRect right_rect = GRect(x2, start_y, block_width, block_height);
    if (12-i <= minute_idx) {
      graphics_fill_rect(ctx, right_rect, 2, GCornersAll);
    }
    else {
      graphics_draw_round_rect(ctx, right_rect, 2);  
    }

    // move down
    start_y += block_height + block_spacer;
  }  
}
