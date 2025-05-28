// FastGraphics.cpp - Fast Graphics Library Implementation
// Comprehensive JSDoc-style documentation for all functions

#include "FastGraphics.h"

// =============================================================================
// STATIC VARIABLES
// =============================================================================

uint16_t* FastGraphics::frame_buffer = nullptr;
ScreenRotation FastGraphics::current_rotation = ROTATION_0;
int16_t FastGraphics::display_width = LCD_H_RES;
int16_t FastGraphics::display_height = LCD_V_RES;

// Text cursor and settings
int16_t FastGraphics::cursor_x = 0;
int16_t FastGraphics::cursor_y = 0;
uint16_t FastGraphics::text_color = COLOR_WHITE;
uint16_t FastGraphics::text_bg_color = COLOR_BLACK;
uint8_t FastGraphics::text_size = 1;
bool FastGraphics::text_wrap = true;
int16_t FastGraphics::text_area_x = 0;
int16_t FastGraphics::text_area_y = 0;
int16_t FastGraphics::text_area_w = LCD_H_RES;
int16_t FastGraphics::text_area_h = LCD_V_RES;
int16_t FastGraphics::line_spacing = 2;  // Default 2 pixels between lines

/**
 * @brief 8x8 pixel font bitmap data
 * @details Contains bitmap data for ASCII characters 0-127. Each character is
 *          defined as 8 bytes representing 8 rows of 8 pixels each.
 * @note Bit 0 = leftmost pixel, bit 7 = rightmost pixel in each row
 */
const uint8_t font8x8_basic[128][8] = {
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0000 (nul)
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0001
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0002
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0003
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0004
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0005
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0006
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0007
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0008
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0009
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+000A
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+000B
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+000C
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+000D
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+000E
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+000F
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0010
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0011
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0012
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0013
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0014
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0015
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0016
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0017
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0018
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0019
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+001A
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+001B
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+001C
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+001D
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+001E
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+001F
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0020 (space)
    { 0x18, 0x3C, 0x3C, 0x18, 0x18, 0x00, 0x18, 0x00},   // U+0021 (!)
    { 0x36, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0022 (")
    { 0x36, 0x36, 0x7F, 0x36, 0x7F, 0x36, 0x36, 0x00},   // U+0023 (#)
    { 0x0C, 0x3E, 0x03, 0x1E, 0x30, 0x1F, 0x0C, 0x00},   // U+0024 ($)
    { 0x00, 0x63, 0x33, 0x18, 0x0C, 0x66, 0x63, 0x00},   // U+0025 (%)
    { 0x1C, 0x36, 0x1C, 0x6E, 0x3B, 0x33, 0x6E, 0x00},   // U+0026 (&)
    { 0x06, 0x06, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0027 (')
    { 0x18, 0x0C, 0x06, 0x06, 0x06, 0x0C, 0x18, 0x00},   // U+0028 (()
    { 0x06, 0x0C, 0x18, 0x18, 0x18, 0x0C, 0x06, 0x00},   // U+0029 ())
    { 0x00, 0x66, 0x3C, 0xFF, 0x3C, 0x66, 0x00, 0x00},   // U+002A (*)
    { 0x00, 0x0C, 0x0C, 0x3F, 0x0C, 0x0C, 0x00, 0x00},   // U+002B (+)
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x06, 0x00},   // U+002C (,)
    { 0x00, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x00, 0x00},   // U+002D (-)
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C, 0x00},   // U+002E (.)
    { 0x60, 0x30, 0x18, 0x0C, 0x06, 0x03, 0x01, 0x00},   // U+002F (/)
    { 0x3E, 0x63, 0x73, 0x7B, 0x6F, 0x67, 0x3E, 0x00},   // U+0030 (0)
    { 0x0C, 0x0E, 0x0C, 0x0C, 0x0C, 0x0C, 0x3F, 0x00},   // U+0031 (1)
    { 0x1E, 0x33, 0x30, 0x1C, 0x06, 0x33, 0x3F, 0x00},   // U+0032 (2)
    { 0x1E, 0x33, 0x30, 0x1C, 0x30, 0x33, 0x1E, 0x00},   // U+0033 (3)
    { 0x38, 0x3C, 0x36, 0x33, 0x7F, 0x30, 0x78, 0x00},   // U+0034 (4)
    { 0x3F, 0x03, 0x1F, 0x30, 0x30, 0x33, 0x1E, 0x00},   // U+0035 (5)
    { 0x1C, 0x06, 0x03, 0x1F, 0x33, 0x33, 0x1E, 0x00},   // U+0036 (6)
    { 0x3F, 0x33, 0x30, 0x18, 0x0C, 0x0C, 0x0C, 0x00},   // U+0037 (7)
    { 0x1E, 0x33, 0x33, 0x1E, 0x33, 0x33, 0x1E, 0x00},   // U+0038 (8)
    { 0x1E, 0x33, 0x33, 0x3E, 0x30, 0x18, 0x0E, 0x00},   // U+0039 (9)
    { 0x00, 0x0C, 0x0C, 0x00, 0x00, 0x0C, 0x0C, 0x00},   // U+003A (:)
    { 0x00, 0x0C, 0x0C, 0x00, 0x00, 0x0C, 0x06, 0x00},   // U+003B (;)
    { 0x18, 0x0C, 0x06, 0x03, 0x06, 0x0C, 0x18, 0x00},   // U+003C (<)
    { 0x00, 0x00, 0x3F, 0x00, 0x00, 0x3F, 0x00, 0x00},   // U+003D (=)
    { 0x06, 0x0C, 0x18, 0x30, 0x18, 0x0C, 0x06, 0x00},   // U+003E (>)
    { 0x1E, 0x33, 0x30, 0x18, 0x0C, 0x00, 0x0C, 0x00},   // U+003F (?)
    { 0x3E, 0x63, 0x7B, 0x7B, 0x7B, 0x03, 0x1E, 0x00},   // U+0040 (@)
    { 0x0C, 0x1E, 0x33, 0x33, 0x3F, 0x33, 0x33, 0x00},   // U+0041 (A)
    { 0x3F, 0x66, 0x66, 0x3E, 0x66, 0x66, 0x3F, 0x00},   // U+0042 (B)
    { 0x3C, 0x66, 0x03, 0x03, 0x03, 0x66, 0x3C, 0x00},   // U+0043 (C)
    { 0x1F, 0x36, 0x66, 0x66, 0x66, 0x36, 0x1F, 0x00},   // U+0044 (D)
    { 0x7F, 0x46, 0x16, 0x1E, 0x16, 0x46, 0x7F, 0x00},   // U+0045 (E)
    { 0x7F, 0x46, 0x16, 0x1E, 0x16, 0x06, 0x0F, 0x00},   // U+0046 (F)
    { 0x3C, 0x66, 0x03, 0x03, 0x73, 0x66, 0x7C, 0x00},   // U+0047 (G)
    { 0x33, 0x33, 0x33, 0x3F, 0x33, 0x33, 0x33, 0x00},   // U+0048 (H)
    { 0x1E, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00},   // U+0049 (I)
    { 0x78, 0x30, 0x30, 0x30, 0x33, 0x33, 0x1E, 0x00},   // U+004A (J)
    { 0x67, 0x66, 0x36, 0x1E, 0x36, 0x66, 0x67, 0x00},   // U+004B (K)
    { 0x0F, 0x06, 0x06, 0x06, 0x46, 0x66, 0x7F, 0x00},   // U+004C (L)
    { 0x63, 0x77, 0x7F, 0x7F, 0x6B, 0x63, 0x63, 0x00},   // U+004D (M)
    { 0x63, 0x67, 0x6F, 0x7B, 0x73, 0x63, 0x63, 0x00},   // U+004E (N)
    { 0x1C, 0x36, 0x63, 0x63, 0x63, 0x36, 0x1C, 0x00},   // U+004F (O)
    { 0x3F, 0x66, 0x66, 0x3E, 0x06, 0x06, 0x0F, 0x00},   // U+0050 (P)
    { 0x1E, 0x33, 0x33, 0x33, 0x3B, 0x1E, 0x38, 0x00},   // U+0051 (Q)
    { 0x3F, 0x66, 0x66, 0x3E, 0x36, 0x66, 0x67, 0x00},   // U+0052 (R)
    { 0x1E, 0x33, 0x07, 0x0E, 0x38, 0x33, 0x1E, 0x00},   // U+0053 (S)
    { 0x3F, 0x2D, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00},   // U+0054 (T)
    { 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x3F, 0x00},   // U+0055 (U)
    { 0x33, 0x33, 0x33, 0x33, 0x33, 0x1E, 0x0C, 0x00},   // U+0056 (V)
    { 0x63, 0x63, 0x63, 0x6B, 0x7F, 0x77, 0x63, 0x00},   // U+0057 (W)
    { 0x63, 0x63, 0x36, 0x1C, 0x1C, 0x36, 0x63, 0x00},   // U+0058 (X)
    { 0x33, 0x33, 0x33, 0x1E, 0x0C, 0x0C, 0x1E, 0x00},   // U+0059 (Y)
    { 0x7F, 0x63, 0x31, 0x18, 0x4C, 0x66, 0x7F, 0x00},   // U+005A (Z)
    { 0x1E, 0x06, 0x06, 0x06, 0x06, 0x06, 0x1E, 0x00},   // U+005B ([)
    { 0x03, 0x06, 0x0C, 0x18, 0x30, 0x60, 0x40, 0x00},   // U+005C (\)
    { 0x1E, 0x18, 0x18, 0x18, 0x18, 0x18, 0x1E, 0x00},   // U+005D (])
    { 0x08, 0x1C, 0x36, 0x63, 0x00, 0x00, 0x00, 0x00},   // U+005E (^)
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF},   // U+005F (_)
    { 0x0C, 0x0C, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0060 (`)
    { 0x00, 0x00, 0x1E, 0x30, 0x3E, 0x33, 0x6E, 0x00},   // U+0061 (a)
    { 0x07, 0x06, 0x06, 0x3E, 0x66, 0x66, 0x3B, 0x00},   // U+0062 (b)
    { 0x00, 0x00, 0x1E, 0x33, 0x03, 0x33, 0x1E, 0x00},   // U+0063 (c)
    { 0x38, 0x30, 0x30, 0x3e, 0x33, 0x33, 0x6E, 0x00},   // U+0064 (d)
    { 0x00, 0x00, 0x1E, 0x33, 0x3f, 0x03, 0x1E, 0x00},   // U+0065 (e)
    { 0x1C, 0x36, 0x06, 0x0f, 0x06, 0x06, 0x0F, 0x00},   // U+0066 (f)
    { 0x00, 0x00, 0x6E, 0x33, 0x33, 0x3E, 0x30, 0x1F},   // U+0067 (g)
    { 0x07, 0x06, 0x36, 0x6E, 0x66, 0x66, 0x67, 0x00},   // U+0068 (h)
    { 0x0C, 0x00, 0x0E, 0x0C, 0x0C, 0x0C, 0x1E, 0x00},   // U+0069 (i)
    { 0x30, 0x00, 0x30, 0x30, 0x30, 0x33, 0x33, 0x1E},   // U+006A (j)
    { 0x07, 0x06, 0x66, 0x36, 0x1E, 0x36, 0x67, 0x00},   // U+006B (k)
    { 0x0E, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00},   // U+006C (l)
    { 0x00, 0x00, 0x33, 0x7F, 0x7F, 0x6B, 0x63, 0x00},   // U+006D (m)
    { 0x00, 0x00, 0x1F, 0x33, 0x33, 0x33, 0x33, 0x00},   // U+006E (n)
    { 0x00, 0x00, 0x1E, 0x33, 0x33, 0x33, 0x1E, 0x00},   // U+006F (o)
    { 0x00, 0x00, 0x3B, 0x66, 0x66, 0x3E, 0x06, 0x0F},   // U+0070 (p)
    { 0x00, 0x00, 0x6E, 0x33, 0x33, 0x3E, 0x30, 0x78},   // U+0071 (q)
    { 0x00, 0x00, 0x3B, 0x6E, 0x66, 0x06, 0x0F, 0x00},   // U+0072 (r)
    { 0x00, 0x00, 0x3E, 0x03, 0x1E, 0x30, 0x1F, 0x00},   // U+0073 (s)
    { 0x08, 0x0C, 0x3E, 0x0C, 0x0C, 0x2C, 0x18, 0x00},   // U+0074 (t)
    { 0x00, 0x00, 0x33, 0x33, 0x33, 0x33, 0x6E, 0x00},   // U+0075 (u)
    { 0x00, 0x00, 0x33, 0x33, 0x33, 0x1E, 0x0C, 0x00},   // U+0076 (v)
    { 0x00, 0x00, 0x63, 0x6B, 0x7F, 0x7F, 0x36, 0x00},   // U+0077 (w)
    { 0x00, 0x00, 0x63, 0x36, 0x1C, 0x36, 0x63, 0x00},   // U+0078 (x)
    { 0x00, 0x00, 0x33, 0x33, 0x33, 0x3E, 0x30, 0x1F},   // U+0079 (y)
    { 0x00, 0x00, 0x3F, 0x19, 0x0C, 0x26, 0x3F, 0x00},   // U+007A (z)
    { 0x38, 0x0C, 0x0C, 0x07, 0x0C, 0x0C, 0x38, 0x00},   // U+007B ({)
    { 0x18, 0x18, 0x18, 0x00, 0x18, 0x18, 0x18, 0x00},   // U+007C (|)
    { 0x07, 0x0C, 0x0C, 0x38, 0x0C, 0x0C, 0x07, 0x00},   // U+007D (})
    { 0x6E, 0x3B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+007E (~)
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}    // U+007F
};

// =============================================================================
// LIBRARY INITIALIZATION
// =============================================================================

/**
 * @brief Initialize the FastGraphics library
 * @implementation Sets up all static variables and prepares the library for use.
 *                 Must be called before any drawing operations.
 */
void FastGraphics::begin(uint16_t* framebuffer) {
    frame_buffer = framebuffer;
    current_rotation = ROTATION_0;
    display_width = LCD_H_RES;
    display_height = LCD_V_RES;
    
    // Initialize text settings to defaults
    cursor_x = 0;
    cursor_y = 0;
    text_color = COLOR_WHITE;
    text_bg_color = COLOR_BLACK;
    text_size = 1;
    text_wrap = true;
    text_area_x = 0;
    text_area_y = 0;
    text_area_w = display_width;
    text_area_h = display_height;
    line_spacing = 2;  // Default line spacing
}

// =============================================================================
// SCREEN ROTATION FUNCTIONS
// =============================================================================

/**
 * @brief Set the screen rotation
 * @implementation Updates rotation state and recalculates logical display dimensions.
 *                 Portrait rotations swap width and height values.
 */
void FastGraphics::setRotation(ScreenRotation rotation) {
    current_rotation = rotation;
    
    // Update logical display dimensions based on rotation
    switch (rotation) {
        case ROTATION_0:   // Landscape
        case ROTATION_180: // Landscape flipped
            display_width = LCD_H_RES;
            display_height = LCD_V_RES;
            break;
            
        case ROTATION_90:  // Portrait
        case ROTATION_270: // Portrait flipped
            display_width = LCD_V_RES;  // Swapped
            display_height = LCD_H_RES; // Swapped
            break;
    }
    
    // Update text area to match new screen dimensions
    text_area_w = display_width;
    text_area_h = display_height;
}

ScreenRotation FastGraphics::getRotation() {
    return current_rotation;
}

int16_t FastGraphics::getWidth() {
    return display_width;
}

int16_t FastGraphics::getHeight() {
    return display_height;
}

// =============================================================================
// COORDINATE TRANSFORMATION
// =============================================================================

/**
 * @brief Transform logical coordinates to physical framebuffer coordinates
 * @implementation Applies rotation matrix transformations to convert from logical
 *                 display coordinates to physical framebuffer coordinates.
 * @details For each rotation:
 *          - ROTATION_0: No transformation (x=x, y=y)  
 *          - ROTATION_90: x'=LCD_H_RES-1-y, y'=x
 *          - ROTATION_180: x'=LCD_H_RES-1-x, y'=LCD_V_RES-1-y
 *          - ROTATION_270: x'=y, y'=LCD_V_RES-1-x
 */
void FastGraphics::transformCoordinates(int16_t& x, int16_t& y) {
    int16_t temp_x = x;
    int16_t temp_y = y;
    
    switch (current_rotation) {
        case ROTATION_0:   // No rotation
            // x = x, y = y (no change)
            break;
            
        case ROTATION_90:  // 90° clockwise (portrait)
            x = LCD_H_RES - 1 - temp_y;
            y = temp_x;
            break;
            
        case ROTATION_180: // 180° (landscape flipped)
            x = LCD_H_RES - 1 - temp_x;
            y = LCD_V_RES - 1 - temp_y;
            break;
            
        case ROTATION_270: // 270° clockwise (portrait flipped)
            x = temp_y;
            y = LCD_V_RES - 1 - temp_x;
            break;
    }
}

/**
 * @brief Transform dimensions for rotated operations  
 * @implementation Currently handles dimension transformation on a per-function basis
 *                 for optimal performance. Most functions handle rotation internally.
 */
void FastGraphics::transformDimensions(int16_t& w, int16_t& h, int16_t x, int16_t y) {
    // For 90° and 270° rotations, we need to swap width and height for certain operations
    if (current_rotation == ROTATION_90 || current_rotation == ROTATION_270) {
        // Note: This is handled per-function basis for optimal performance
        // Most functions will handle this internally
    }
}

// =============================================================================
// PRIVATE HELPER FUNCTIONS
// =============================================================================

/**
 * @brief Draw a single pixel with bounds checking and rotation
 * @implementation Performs bounds checking against logical display size,
 *                 applies coordinate transformation, then draws to framebuffer.
 * @performance O(1) operation with minimal overhead for bounds checking
 */
inline void FastGraphics::pixel(int16_t x, int16_t y, uint16_t color) {
    // Check bounds against logical display size
    if (x >= 0 && x < display_width && y >= 0 && y < display_height) {
        // Transform coordinates based on rotation
        transformCoordinates(x, y);
        
        // Now check against physical display bounds and draw
        if (x >= 0 && x < LCD_H_RES && y >= 0 && y < LCD_V_RES) {
            frame_buffer[y * LCD_H_RES + x] = color;
        }
    }
}

// =============================================================================
// CORE DRAWING FUNCTIONS
// =============================================================================

/**
 * @brief Clear the entire screen with a solid color
 * @implementation Uses fillRect for the entire display area, which provides
 *                 optimal performance through vectorized clearing operations.
 */
void FastGraphics::clear(uint16_t color) {
    fillRect(0, 0, display_width, display_height, color);
}

/**
 * @brief Draw a filled rectangle
 * @implementation Optimized for performance with different paths for rotated vs non-rotated.
 *                 Non-rotated uses fast memory operations, rotated uses pixel-by-pixel.
 * @performance Non-rotated: ~O(w*h/4) due to 4-pixel unrolled loop
 *              Rotated: O(w*h) due to coordinate transformation per pixel
 */
void FastGraphics::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    // Input validation using logical display dimensions
    if (x >= display_width || y >= display_height || w <= 0 || h <= 0) return;
    
    // Clip to logical screen boundaries
    if (x < 0) { w += x; x = 0; }
    if (y < 0) { h += y; y = 0; }  
    if (x + w > display_width) w = display_width - x;
    if (y + h > display_height) h = display_height - y;
    
    // For rotations, we need to draw pixel by pixel to handle coordinate transformation
    // This is less optimal but ensures correctness for all rotations
    if (current_rotation != ROTATION_0) {
        for (int16_t row = 0; row < h; row++) {
            for (int16_t col = 0; col < w; col++) {
                pixel(x + col, y + row, color);
            }
        }
    } else {
        // Optimized path for no rotation - direct framebuffer access
        for (int16_t row = 0; row < h; row++) {
            uint16_t* ptr = &frame_buffer[(y + row) * LCD_H_RES + x];
            int16_t pixels = w;
            
            // Unrolled loop for maximum performance - 4 pixels at a time
            while (pixels >= 4) {
                *ptr++ = color; *ptr++ = color; *ptr++ = color; *ptr++ = color;
                pixels -= 4;
            }
            // Handle remaining pixels
            while (pixels > 0) {
                *ptr++ = color;
                pixels--;
            }
        }
    }
}

/**
 * @brief Draw a filled circle
 * @implementation Uses optimized midpoint circle algorithm with horizontal line fills.
 *                 Draws filled lines instead of individual pixels for performance.
 * @performance O(r) where r is radius - very efficient due to 8-way symmetry
 * @algorithm Modified Bresenham circle algorithm optimized for filled circles
 */
void FastGraphics::fillCircle(int16_t x0, int16_t y0, int16_t radius, uint16_t color) {
    if (radius <= 0) return;
    
    int16_t x = 0;
    int16_t y = radius;
    int16_t decision = 1 - radius;
    
    // Draw center line
    fillRect(x0 - radius, y0, 2 * radius + 1, 1, color);
    
    while (x < y) {
        if (decision < 0) {
            decision += 2 * x + 3;
        } else {
            decision += 2 * (x - y) + 5;
            // Draw horizontal lines for filled circle
            fillRect(x0 - x, y0 + y, 2 * x + 1, 1, color);
            fillRect(x0 - x, y0 - y, 2 * x + 1, 1, color);
            y--;
        }
        x++;
        if (x <= y) {
            fillRect(x0 - y, y0 + x, 2 * y + 1, 1, color);
            fillRect(x0 - y, y0 - x, 2 * y + 1, 1, color);
        }
    }
}

/**
 * @brief Draw a line between two points
 * @implementation Optimized with special cases for horizontal/vertical lines.
 *                 Uses Bresenham algorithm for diagonal lines.
 * @performance Horizontal/Vertical: O(1) using fillRect
 *              Diagonal: O(max(dx,dy)) using Bresenham algorithm
 */
void FastGraphics::line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {
    // Handle special cases (much faster)
    if (y0 == y1) {
        // Horizontal line - use optimized fillRect
        if (x0 > x1) { int16_t temp = x0; x0 = x1; x1 = temp; }
        fillRect(x0, y0, x1 - x0 + 1, 1, color);
        return;
    }
    if (x0 == x1) {
        // Vertical line - use optimized fillRect
        if (y0 > y1) { int16_t temp = y0; y0 = y1; y1 = temp; }
        fillRect(x0, y0, 1, y1 - y0 + 1, color);
        return;
    }
    
    // Bresenham line algorithm for diagonal lines
    int16_t dx = abs(x1 - x0);
    int16_t dy = abs(y1 - y0);
    int16_t sx = (x0 < x1) ? 1 : -1;
    int16_t sy = (y0 < y1) ? 1 : -1;
    int16_t err = dx - dy;
    
    int16_t x = x0, y = y0;
    while (true) {
        pixel(x, y, color);
        if (x == x1 && y == y1) break;
        
        int16_t e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x += sx; }
        if (e2 < dx) { err += dx; y += sy; }
    }
}

// =============================================================================
// SHAPE OUTLINE FUNCTIONS
// =============================================================================

/**
 * @brief Draw a rectangle outline
 * @implementation Draws four separate lines to form rectangle outline.
 *                 More efficient than drawing individual pixels.
 */
void FastGraphics::rect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    line(x, y, x + w - 1, y, color);                    // Top
    line(x, y + h - 1, x + w - 1, y + h - 1, color);   // Bottom
    line(x, y, x, y + h - 1, color);                    // Left
    line(x + w - 1, y, x + w - 1, y + h - 1, color);   // Right
}

/**
 * @brief Draw a circle outline
 * @implementation Uses midpoint circle algorithm with 8-way symmetry for efficiency.
 *                 Only calculates 1/8 of circle and mirrors to other octants.
 * @performance O(r) where r is radius - very efficient
 */
void FastGraphics::circle(int16_t x0, int16_t y0, int16_t radius, uint16_t color) {
    if (radius <= 0) return;
    
    int16_t x = 0, y = radius;
    int16_t decision = 1 - radius;
    
    while (x <= y) {
        // Draw all 8 octants using symmetry
        pixel(x0 + x, y0 + y, color); pixel(x0 - x, y0 + y, color);
        pixel(x0 + x, y0 - y, color); pixel(x0 - x, y0 - y, color);
        pixel(x0 + y, y0 + x, color); pixel(x0 - y, y0 + x, color);
        pixel(x0 + y, y0 - x, color); pixel(x0 - y, y0 - x, color);
        
        if (decision < 0) {
            decision += 2 * x + 3;
        } else {
            decision += 2 * (x - y) + 5;
            y--;
        }
        x++;
    }
}

// =============================================================================
// TEXT FUNCTIONS
// =============================================================================

/**
 * @brief Draw a single character at specified position
 * @implementation Renders character from 8x8 font bitmap data with optional scaling.
 *                 Optimized paths for size=1 vs larger sizes.
 * @performance Size 1: O(64) - direct pixel operations
 *              Size >1: O(64 * size²) - uses fillRect for scaled pixels
 */
void FastGraphics::drawChar(int16_t x, int16_t y, char c, uint16_t color, uint16_t bg, uint8_t size) {
    if (c < 0 || c > 127) return;
    
    const uint8_t* char_data = font8x8_basic[c];
    
    if (size == 1) {
        // Optimized path for normal size characters
        for (int8_t row = 0; row < 8; row++) {
            uint8_t line = char_data[row];
            for (int8_t col = 0; col < 8; col++) {
                if (line & (1 << col)) {
                    pixel(x + col, y + row, color);
                } else if (bg != color) {
                    pixel(x + col, y + row, bg);
                }
            }
        }
    } else {
        // Scaled character rendering using fillRect for efficiency
        for (int8_t row = 0; row < 8; row++) {
            uint8_t line = char_data[row];
            for (int8_t col = 0; col < 8; col++) {
                if (line & (1 << col)) {
                    fillRect(x + col * size, y + row * size, size, size, color);
                } else if (bg != color) {
                    fillRect(x + col * size, y + row * size, size, size, bg);
                }
            }
        }
    }
}

/**
 * @brief Draw text at specified position
 * @implementation Renders string character by character, handling newlines and
 *                 carriage returns. Cursor advances automatically.
 */
void FastGraphics::text(int16_t x, int16_t y, const char* str, uint16_t color, uint16_t bg, uint8_t size) {
    int16_t cursor_x = x, cursor_y = y;
    
    while (*str) {
        if (*str == '\n') {
            cursor_y += size * 8;
            cursor_x = x;
        } else if (*str != '\r') {
            drawChar(cursor_x, cursor_y, *str, color, bg, size);
            cursor_x += size * 8;
        }
        str++;
    }
}

// =============================================================================
// ADVANCED TEXT FUNCTIONS
// =============================================================================

void FastGraphics::setCursor(int16_t x, int16_t y) {
    cursor_x = x;
    cursor_y = y;
}

void FastGraphics::setTextColor(uint16_t color, uint16_t bg) {
    text_color = color;
    text_bg_color = bg;
}

void FastGraphics::setTextSize(uint8_t size) {
    if (size >= 1 && size <= 10) {  // Reasonable size limits
        text_size = size;
    }
}

void FastGraphics::setTextWrap(bool wrap) {
    text_wrap = wrap;
}

void FastGraphics::setLineSpacing(int16_t spacing) {
    if (spacing >= 0 && spacing <= 20) {  // Reasonable limits
        line_spacing = spacing;
    }
}

int16_t FastGraphics::getLineSpacing() {
    return line_spacing;
}

void FastGraphics::getCursor(int16_t& x, int16_t& y) {
    x = cursor_x;
    y = cursor_y;
}

int16_t FastGraphics::getCursorX() {
    return cursor_x;
}

int16_t FastGraphics::getCursorY() {
    return cursor_y;
}

void FastGraphics::setTextArea(int16_t x, int16_t y, int16_t w, int16_t h) {
    text_area_x = x;
    text_area_y = y;
    text_area_w = w;
    text_area_h = h;
}

void FastGraphics::clearTextArea() {
    fillRect(text_area_x, text_area_y, text_area_w, text_area_h, text_bg_color);
    cursor_x = text_area_x;
    cursor_y = text_area_y;
}

/**
 * @brief Internal helper to advance cursor position
 * @implementation Moves cursor and handles automatic line wrapping when enabled.
 *                 Checks text area boundaries for wrapping decisions.
 */
void FastGraphics::advanceCursor(uint8_t char_width, uint8_t char_height) {
    cursor_x += char_width;
    
    // Check for line wrap
    if (text_wrap && cursor_x + char_width > text_area_x + text_area_w) {
        newLine();
    }
}

/**
 * @brief Move cursor to next line
 * @implementation Resets X to text area left edge, advances Y by character height
 *                 plus line spacing. Handles text area overflow by scrolling.
 */
void FastGraphics::newLine() {
    cursor_x = text_area_x;
    cursor_y += text_size * 8 + line_spacing;  // Character height + line spacing
    
    // Check if we've gone past the text area
    if (cursor_y + text_size * 8 > text_area_y + text_area_h) {
        // Scroll up by clearing and resetting to top
        clearTextArea();
    }
}

// =============================================================================
// PRINT FUNCTIONS - Support all common data types
// =============================================================================

/**
 * @brief Print a C-style string
 * @implementation Character-by-character rendering with special handling for
 *                 control characters like newline and carriage return.
 */
void FastGraphics::print(const char* str) {
    while (*str) {
        if (*str == '\n') {
            newLine();
        } else if (*str == '\r') {
            cursor_x = text_area_x;  // Carriage return
        } else if (*str >= 0 && *str <= 127) {
            drawChar(cursor_x, cursor_y, *str, text_color, text_bg_color, text_size);
            advanceCursor(text_size * 8, text_size * 8);
        }
        str++;
    }
}

void FastGraphics::print(String str) {
    print(str.c_str());
}

void FastGraphics::print(int number) {
    char buffer[12];  // Enough for 32-bit int
    sprintf(buffer, "%d", number);
    print(buffer);
}

void FastGraphics::print(unsigned int number) {
    char buffer[12];
    sprintf(buffer, "%u", number);
    print(buffer);
}

void FastGraphics::print(long number) {
    char buffer[20];  // Enough for 64-bit long
    sprintf(buffer, "%ld", number);
    print(buffer);
}

void FastGraphics::print(unsigned long number) {
    char buffer[20];
    sprintf(buffer, "%lu", number);
    print(buffer);
}

void FastGraphics::print(float number, int decimals) {
    char buffer[20];
    char format[10];
    sprintf(format, "%%.%df", decimals);
    sprintf(buffer, format, number);
    print(buffer);
}

void FastGraphics::print(double number, int decimals) {
    char buffer[25];
    char format[10];
    sprintf(format, "%%.%dlf", decimals);
    sprintf(buffer, format, number);
    print(buffer);
}

void FastGraphics::print(char character) {
    if (character == '\n') {
        newLine();
    } else if (character == '\r') {
        cursor_x = text_area_x;
    } else if (character >= 0 && character <= 127) {
        drawChar(cursor_x, cursor_y, character, text_color, text_bg_color, text_size);
        advanceCursor(text_size * 8, text_size * 8);
    }
}

void FastGraphics::print(bool value) {
    print(value ? "true" : "false");
}

// Println versions (add newline after printing)
void FastGraphics::println(const char* str) {
    print(str);
    print("\n");
}

void FastGraphics::println(String str) {
    print(str);
    print("\n");  
}

void FastGraphics::println(int number) {
    print(number);
    print("\n");
}

void FastGraphics::println(unsigned int number) {
    print(number);
    print("\n");
}

void FastGraphics::println(long number) {
    print(number);
    print("\n");
}

void FastGraphics::println(unsigned long number) {
    print(number);
    print("\n");
}

void FastGraphics::println(float number, int decimals) {
    print(number, decimals);
    print("\n");
}

void FastGraphics::println(double number, int decimals) {
    print(number, decimals);
    print("\n");
}

void FastGraphics::println(char character) {
    print(character);
    print("\n");
}

void FastGraphics::println(bool value) {
    print(value);
    print("\n");
}

/**
 * @brief Print text with automatic word wrapping
 * @implementation Intelligent word wrapping that preserves word boundaries.
 *                 Handles spaces, newlines, and tabs appropriately.
 * @algorithm Builds words in buffer, checks if they fit on current line,
 *           wraps to next line if needed while preserving word integrity.
 */
void FastGraphics::printWrapped(int16_t x, int16_t y, int16_t maxWidth, const char* str, uint16_t color, uint8_t size) {
    int16_t current_x = x;
    int16_t current_y = y;
    int16_t word_start_x = x;
    char word_buffer[50];  // Buffer for current word
    int word_len = 0;
    int16_t line_height = size * 8 + line_spacing;  // Include line spacing
    
    while (*str) {
        if (*str == ' ' || *str == '\n' || *str == '\t' || *(str + 1) == '\0') {
            // End of word - add current character if not space
            if (*str != ' ' && *str != '\n' && *str != '\t') {
                word_buffer[word_len++] = *str;
            }
            word_buffer[word_len] = '\0';
            
            // Check if word fits on current line
            int16_t word_width = word_len * size * 8;
            if (current_x + word_width > x + maxWidth && current_x > x) {
                // Word doesn't fit - move to next line
                current_x = x;
                current_y += line_height;
                word_start_x = x;
            }
            
            // Draw the word
            text(current_x, current_y, word_buffer, color, text_bg_color, size);
            current_x += word_width;
            
            // Handle different separators
            if (*str == ' ') {
                current_x += size * 8;  // Add space width
            } else if (*str == '\n') {
                current_x = x;
                current_y += line_height;
            } else if (*str == '\t') {
                current_x += size * 8 * 4;  // Tab = 4 spaces
            }
            
            // Reset word buffer
            word_len = 0;
            word_start_x = current_x;
        } else {
            // Add character to current word
            if (word_len < 49) {  // Prevent buffer overflow
                word_buffer[word_len++] = *str;
            }
        }
        str++;
    }
}

// Convenience text functions
void FastGraphics::textSmall(int16_t x, int16_t y, const char* str, uint16_t color) {
    text(x, y, str, color, COLOR_BLACK, 1);
}

void FastGraphics::textMedium(int16_t x, int16_t y, const char* str, uint16_t color) {
    text(x, y, str, color, COLOR_BLACK, 2);
}

void FastGraphics::textLarge(int16_t x, int16_t y, const char* str, uint16_t color) {
    text(x, y, str, color, COLOR_BLACK, 3);
}

// =============================================================================
// DEMO FUNCTION
// =============================================================================

/**
 * @brief Run a comprehensive graphics demonstration
 * @implementation Showcases various library capabilities including text formatting,
 *                 colors, text areas, and word wrapping. Useful for testing.
 */
void FastGraphics::demo() {
    clear(COLOR_BLACK);
    
    // Demo the new text features
    setCursor(10, 10);
    setTextColor(COLOR_WHITE);
    setTextSize(1);
    
    println("=== FastGraphics Text Demo ===");
    println();
    
    // Different colors and sizes
    setTextColor(COLOR_GREEN);
    print("Green text, ");
    setTextColor(COLOR_RED);
    print("Red text, ");
    setTextColor(COLOR_BLUE);
    println("Blue text");
    println();
    
    // Numbers and formatting
    setTextColor(COLOR_YELLOW);
    print("Temperature: ");
    print(25.6, 1);
    println(" C");
    
    print("Count: ");
    println(42);
    println();
    
    // Text wrapping demo
    setTextColor(COLOR_CYAN);
    printWrapped(10, cursor_y, display_width - 20, 
                "This is a long line that should automatically wrap to the next line when it reaches the edge of the display. Pretty cool, right?", 
                COLOR_CYAN, 1);
    
    setCursor(10, cursor_y + 40);
    
    // Text area demo
    setTextColor(COLOR_WHITE);
    fillRect(display_width - 200, 100, 180, 100, COLOR_BLUE);
    setTextArea(display_width - 190, 110, 160, 80);
    setCursor(display_width - 190, 110);
    setTextColor(COLOR_WHITE);
    
    println("Text Area:");
    println("Confined to");
    println("this blue box");
    println("with auto-wrap");
    
    // Reset text area to full screen
    setTextArea(0, 0, display_width, display_height);
}