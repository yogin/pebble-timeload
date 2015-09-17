#include <pebble.h>
#include "common.h"
#include "watchface_battery.h"

void draw_battery(Layer *layer, GContext *ctx) {
  GRect bounds = layer_get_bounds(layer);
  GRect battery_rect = GRect(border, border, sidebar_width, bounds.size.h - 2 * border);
  graphics_draw_round_rect(ctx, battery_rect, 2);  
}
