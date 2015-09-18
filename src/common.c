#include <pebble.h>
#include "common.h"

int screen_width = 144;
int screen_height = 168;
int border = 1;

bool show_battery = true;
bool show_timeline = false;
int sidebar_width = 10;

Window *s_main_window;
Layer *s_main_layer;
