#include <pebble.h>
#include "common.h"
#include "watchface_battery.h"

int battery_charge_percent;

void battery_handler(BatteryChargeState charge_state) {
	battery_charge_percent = charge_state.charge_percent;
	APP_LOG(APP_LOG_LEVEL_DEBUG, "battery charge: %d%%", battery_charge_percent);
	layer_mark_dirty(s_main_layer);
}

void draw_battery(Layer *layer, GContext *ctx) {
  GRect bounds = layer_get_bounds(layer);
  int battery_height = bounds.size.h - 2 * border;
  GRect battery_rect = GRect(border, border, sidebar_width, battery_height);
  graphics_draw_round_rect(ctx, battery_rect, 2);

  int fill_height = (int)(battery_height / 100.0 * battery_charge_percent);
  int fill_start_y = border + battery_height - fill_height;
  GRect battery_fill_rect = GRect(border, fill_start_y, sidebar_width, fill_height);
  graphics_fill_rect(ctx, battery_fill_rect, 2, GCornersAll);
}
