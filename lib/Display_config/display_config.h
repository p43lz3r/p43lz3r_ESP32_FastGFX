#ifndef DISPLAY_CONFIG_H
#define DISPLAY_CONFIG_H

#include <stdint.h> // For uint16_t
#include "esp_lcd_panel_ops.h" // For esp_lcd_panel_handle_t

// --- Display Configuration ---
#define LCD_PIXEL_CLOCK_HZ     (16 * 1000 * 1000)
#define LCD_BK_LIGHT_ON_LEVEL  1
#define LCD_BK_LIGHT_OFF_LEVEL !LCD_BK_LIGHT_ON_LEVEL
#define LCD_H_RES              800
#define LCD_V_RES              480

// --- RGB Pin Definitions ---
#define PIN_NUM_DE             5
#define PIN_NUM_VSYNC          3
#define PIN_NUM_HSYNC          46
#define PIN_NUM_PCLK           7
#define PIN_NUM_DATA0          14  // B0
#define PIN_NUM_DATA1          38  // B1
#define PIN_NUM_DATA2          18  // B2
#define PIN_NUM_DATA3          17  // B3
#define PIN_NUM_DATA4          10  // B4
#define PIN_NUM_DATA5          39  // G0
#define PIN_NUM_DATA6          0   // G1
#define PIN_NUM_DATA7          45  // G2
#define PIN_NUM_DATA8          48  // G3
#define PIN_NUM_DATA9          47  // G4
#define PIN_NUM_DATA10         21  // G5
#define PIN_NUM_DATA11         1   // R0
#define PIN_NUM_DATA12         2   // R1
#define PIN_NUM_DATA13         42  // R2
#define PIN_NUM_DATA14         41  // R3
#define PIN_NUM_DATA15         40  // R4

// --- Global Variable Declarations (defined in display_config.cpp) ---
extern esp_lcd_panel_handle_t panel_handle;
extern uint16_t *frame_buffer;

// --- Function Prototype for Display Initialization ---
bool initialize_display_and_framebuffer();

#endif // DISPLAY_CONFIG_H