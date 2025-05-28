/**
 * @file main.cpp
 * @brief ESP32-S3 FastGFX Demo Application
 * @author p43lz3r
 * @date 2025
 */

 #include <Arduino.h>
 #include "display_config.h"
 #include "simple_touch.h"
 #include "FastGraphics.h"
 
 /**
  * Main application screen - demonstrates FastGraphics capabilities
  */
 void drawMyApp() {
     // Clear screen and set up text output
     FastGraphics::clear(COLOR_BLACK);
     FastGraphics::setCursor(10, 10);
     FastGraphics::setTextColor(COLOR_WHITE);
     FastGraphics::setTextSize(1);
     FastGraphics::setTextWrap(true);
     
     // Application title
     FastGraphics::setTextSize(2);
     FastGraphics::setTextColor(COLOR_GREEN);
     FastGraphics::println("ESP32-S3 Display");
     FastGraphics::println();
     
     // System information
     FastGraphics::setTextSize(1);
     FastGraphics::setTextColor(COLOR_YELLOW);
     FastGraphics::print("Resolution: ");
     FastGraphics::print(FastGraphics::getWidth());
     FastGraphics::print(" x ");
     FastGraphics::println(FastGraphics::getHeight());
     
     FastGraphics::print("Rotation: ");
     FastGraphics::println(FastGraphics::getRotation());
     FastGraphics::println();
     
     // Simulated sensor data
     FastGraphics::setTextColor(COLOR_CYAN);
     FastGraphics::println("Sensor Readings:");
     FastGraphics::setTextColor(COLOR_RED);
     
     FastGraphics::print("  Temperature: ");
     FastGraphics::print(23.5, 1);
     FastGraphics::println(" C");
     
     FastGraphics::print("  Humidity: ");
     FastGraphics::print(67);
     FastGraphics::println(" %");
     
     FastGraphics::print("  Pressure: ");
     FastGraphics::print(1013.25, 2);
     FastGraphics::println(" hPa");
     FastGraphics::println();
     
     // Status messages with color coding
     FastGraphics::setTextColor(COLOR_GREEN);
     FastGraphics::println("Status: All systems OK");
     FastGraphics::setTextColor(COLOR_BLUE);
     FastGraphics::println("WiFi: Connected");
     FastGraphics::setTextColor(COLOR_MAGENTA);
     FastGraphics::println("Memory: 85% free");
     FastGraphics::println();
     
     // Word wrapping demo with improved line spacing
     FastGraphics::setTextColor(COLOR_RED);
     FastGraphics::setLineSpacing(4);  // Better readability
     FastGraphics::printWrapped(10, FastGraphics::getCursorY(), 
                               FastGraphics::getWidth() - 20,
                               "This demonstrates automatic word wrapping with improved line spacing. "
                               "Long sentences will automatically break at word boundaries to fit within "
                               "the specified width. Notice how the extra space between lines makes it "
                               "much easier to read!",
                               COLOR_ORANGE, 1);
 
     // Update display
     if (panel_handle && frame_buffer) {
         esp_lcd_panel_draw_bitmap(panel_handle, 0, 0, LCD_H_RES, LCD_V_RES, frame_buffer);
     }
 }
 
 void setup() {
     Serial.begin(115200);
 
     // Initialize touch controller
     touch_init();
 
     // Initialize display and framebuffer - critical step
     if (!initialize_display_and_framebuffer()) {
         Serial.println("Display initialization FAILED! Halting.");
         while (1) {
             delay(1000);
         }
     }
 
     // Initialize graphics library
     FastGraphics::begin(frame_buffer);
 
     // Draw initial screen
     drawMyApp();
 
     Serial.println("ESP32-S3 Fast Graphics initialized!");
 }
 
 void loop() {
     // Handle touch input
     if (touch_touched()) {
         Serial.print("Touch at: ");
         Serial.print(touch_last_x);
         Serial.print(", ");
         Serial.println(touch_last_y);
 
         // Draw touch feedback
         FastGraphics::fillCircle(touch_last_x, touch_last_y, 5, COLOR_WHITE);
         
         if (panel_handle && frame_buffer) {
             esp_lcd_panel_draw_bitmap(panel_handle, 0, 0, LCD_H_RES, LCD_V_RES, frame_buffer);
         }
     }
 
     delay(50); // 20Hz update rate
 }