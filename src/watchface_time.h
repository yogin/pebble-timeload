#ifndef WATCHFACE_TIME_H
#define WATCHFACE_TIME_H

extern int hour_idx;
extern int minute_idx;

void update_time();
void tick_handler(struct tm *tick_time, TimeUnits units_changed);
void draw_time(Layer *layer, GContext *ctx);

#endif