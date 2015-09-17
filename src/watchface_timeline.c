#include <pebble.h>
#include "common.h"
#include "watchface_timeline.h"

void draw_timeline(Layer *layer, GContext *ctx) {
  GRect bounds = layer_get_bounds(layer);
  GRect timeline_rect = GRect(bounds.size.w - sidebar_width - border, border, sidebar_width, bounds.size.h - 2 * border);
  graphics_draw_round_rect(ctx, timeline_rect, 2);  
}
