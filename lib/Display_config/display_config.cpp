#include "display_config.h" // Includes all our defines and extern declarations
#include <Arduino.h>          // For Serial
#include "esp_lcd_panel_rgb.h" // For panel configuration structs and functions
#include "driver/gpio.h"       // For GPIO_NUM_NC
#include "esp_heap_caps.h"     // For heap_caps_malloc (for PSRAM)

// --- Global Variable Definitions ---
esp_lcd_panel_handle_t panel_handle = NULL;
uint16_t *frame_buffer = NULL;

bool initialize_display_and_framebuffer() {
    // Allocate frame buffer
    // Since panel_config.flags.fb_in_psram = 1, we MUST allocate from PSRAM
    frame_buffer = (uint16_t*)heap_caps_malloc(LCD_H_RES * LCD_V_RES * sizeof(uint16_t), MALLOC_CAP_SPIRAM | MALLOC_CAP_DEFAULT);
    
    if (!frame_buffer) {
        Serial.println("Failed to allocate frame buffer in PSRAM!");
        return false;
    }
    
    // Configure RGB LCD panel
    esp_lcd_rgb_panel_config_t panel_config = {
        .clk_src = LCD_CLK_SRC_DEFAULT,
        .timings = {
            .pclk_hz = LCD_PIXEL_CLOCK_HZ,
            .h_res = LCD_H_RES,
            .v_res = LCD_V_RES,
            .hsync_pulse_width = 4,
            .hsync_back_porch = 8,
            .hsync_front_porch = 8,
            .vsync_pulse_width = 4,
            .vsync_back_porch = 8,
            .vsync_front_porch = 8,
            .flags = {
                .hsync_idle_low = 0,
                .vsync_idle_low = 0,
                .de_idle_high = 0,
                .pclk_active_neg = 1, // As per your original config
                .pclk_idle_high = 0,  // As per your original config
            },
        },
        .data_width = 16,
        .bits_per_pixel = 16, // Assuming 16bpp for RGB565
        .num_fbs = 1,
        .bounce_buffer_size_px = 0, // No bounce buffer
        .sram_trans_align = 4,
        .psram_trans_align = 64,
        .hsync_gpio_num = PIN_NUM_HSYNC,
        .vsync_gpio_num = PIN_NUM_VSYNC,
        .de_gpio_num = PIN_NUM_DE,
        .pclk_gpio_num = PIN_NUM_PCLK,
        .disp_gpio_num = GPIO_NUM_NC, // No dedicated display enable/disable if not used
        .data_gpio_nums = {
            PIN_NUM_DATA0, PIN_NUM_DATA1, PIN_NUM_DATA2, PIN_NUM_DATA3,
            PIN_NUM_DATA4, PIN_NUM_DATA5, PIN_NUM_DATA6, PIN_NUM_DATA7,
            PIN_NUM_DATA8, PIN_NUM_DATA9, PIN_NUM_DATA10, PIN_NUM_DATA11,
            PIN_NUM_DATA12, PIN_NUM_DATA13, PIN_NUM_DATA14, PIN_NUM_DATA15,
        },
        .flags = {
            .fb_in_psram = 1, // Frame buffer is in PSRAM
        },
    };
    
    // Install RGB LCD panel driver
    esp_err_t ret;
    ret = esp_lcd_new_rgb_panel(&panel_config, &panel_handle);
    if (ret != ESP_OK) {
        Serial.printf("esp_lcd_new_rgb_panel failed: %s\n", esp_err_to_name(ret));
        return false;
    }
    
    ret = esp_lcd_panel_reset(panel_handle);
    if (ret != ESP_OK) {
        Serial.printf("esp_lcd_panel_reset failed: %s\n", esp_err_to_name(ret));
        return false;
    }
    
    ret = esp_lcd_panel_init(panel_handle);
    if (ret != ESP_OK) {
        Serial.printf("esp_lcd_panel_init failed: %s\n", esp_err_to_name(ret));
        return false;
    }
    
    Serial.println("Display initialized successfully.");
    return true;
}