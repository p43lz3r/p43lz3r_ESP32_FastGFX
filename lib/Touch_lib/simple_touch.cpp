// simple_touch.cpp - Touch implementation for PlatformIO
#include <Arduino.h>
#include "simple_touch.h"

int touch_last_x = 0, touch_last_y = 0;

TAMC_GT911 ts = TAMC_GT911(TOUCH_GT911_SDA, TOUCH_GT911_SCL, TOUCH_GT911_INT, TOUCH_GT911_RST, 
                           max(TOUCH_MAP_X1, TOUCH_MAP_X2), max(TOUCH_MAP_Y1, TOUCH_MAP_Y2));

void touch_init()
{
  Wire.begin(TOUCH_GT911_SDA, TOUCH_GT911_SCL);

  byte error;
  bool done = false;
  int gt_addr[]{GT911_ADDR1, GT911_ADDR2};

  for (int i = 0; (i < 2) && !done; i++) {
    int addr = gt_addr[i];  
    Wire.beginTransmission(addr);
    error = Wire.endTransmission();
    if (error == 0) {
      Serial.print("GT911 touch controller found at 0x");
      Serial.println(addr, HEX);
      ts.begin(addr);
      done = true;
    }
  }

  if (!done) {
    Serial.println("GT911 touch controller NOT found!");
  }

  ts.setRotation(TOUCH_GT911_ROTATION);
}

bool touch_touched()
{
  ts.read();
  if (ts.isTouched) {
    // Map touch coordinates to screen coordinates
    touch_last_x = map(ts.points[0].x, TOUCH_MAP_X1, TOUCH_MAP_X2, 0, 799);
    touch_last_y = map(ts.points[0].y, TOUCH_MAP_Y1, TOUCH_MAP_Y2, 0, 479);
    return true;
  }
  else {
    return false;
  }
}