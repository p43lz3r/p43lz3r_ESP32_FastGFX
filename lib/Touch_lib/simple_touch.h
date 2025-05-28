// simple_touch.h - Simplified touch handling for PlatformIO
#include <Arduino.h>
#include <Wire.h>
#include <TAMC_GT911.h>

#ifndef SIMPLE_TOUCH_H
#define SIMPLE_TOUCH_H

#define TOUCH_GT911
#define TOUCH_GT911_SCL 9
#define TOUCH_GT911_SDA 8
#define TOUCH_GT911_INT 4
#define TOUCH_GT911_RST -1
#define TOUCH_GT911_ROTATION ROTATION_NORMAL
#define TOUCH_MAP_X1 800
#define TOUCH_MAP_X2 0
#define TOUCH_MAP_Y1 480
#define TOUCH_MAP_Y2 0

extern int touch_last_x, touch_last_y;

extern TAMC_GT911 ts;

void touch_init();
bool touch_touched();

#endif // SIMPLE_TOUCH_H