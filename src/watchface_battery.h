#ifndef WATCHFACE_BATTERY_H
#define WATCHFACE_BATTERY_H

void draw_battery(Layer *layer, GContext *ctx);
void battery_handler(BatteryChargeState charge_state);

#endif