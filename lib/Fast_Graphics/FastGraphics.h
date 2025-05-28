// FastGraphics.h - Fast Graphics Library for ESP32-S3 RGB Displays
// Optimized for speed and ease of use

#ifndef FAST_GRAPHICS_H
#define FAST_GRAPHICS_H

#include <Arduino.h>

// =============================================================================
// LIBRARY CONFIGURATION
// =============================================================================

// Display dimensions (adjust for your display)
#ifndef LCD_H_RES
#define LCD_H_RES 800
#endif

#ifndef LCD_V_RES  
#define LCD_V_RES 480
#endif

/**
 * @enum ScreenRotation
 * @brief Screen rotation options for display orientation
 * @details Defines the four possible screen orientations. ROTATION_0 and ROTATION_180 
 *          maintain landscape orientation, while ROTATION_90 and ROTATION_270 switch to portrait.
 */
enum ScreenRotation {
    ROTATION_0   = 0,  /**< Landscape (default): 800x480 */
    ROTATION_90  = 1,  /**< Portrait: 480x800 */
    ROTATION_180 = 2,  /**< Landscape flipped: 800x480 */
    ROTATION_270 = 3   /**< Portrait flipped: 480x800 */
};

// RGB565 color definitions
#define COLOR_BLACK   0x0000  /**< Black color (RGB565: 0x0000) */
#define COLOR_WHITE   0xFFFF  /**< White color (RGB565: 0xFFFF) */
#define COLOR_RED     0xF800  /**< Red color (RGB565: 0xF800) */
#define COLOR_GREEN   0x07E0  /**< Green color (RGB565: 0x07E0) */
#define COLOR_BLUE    0x001F  /**< Blue color (RGB565: 0x001F) */
#define COLOR_YELLOW  0xFFE0  /**< Yellow color (RGB565: 0xFFE0) */
#define COLOR_MAGENTA 0xF81F  /**< Magenta color (RGB565: 0xF81F) */
#define COLOR_CYAN    0x07FF  /**< Cyan color (RGB565: 0x07FF) */
#define COLOR_GRAY    0x8410  /**< Gray color (RGB565: 0x8410) */
#define COLOR_ORANGE  0xFD20  /**< Orange color (RGB565: 0xFD20) */
#define COLOR_PURPLE  0x801F  /**< Purple color (RGB565: 0x801F) */

// =============================================================================
// FASTGRAPHICS CLASS
// =============================================================================

/**
 * @class FastGraphics
 * @brief Fast graphics library for ESP32-S3 RGB displays
 * @details A high-performance graphics library optimized for ESP32-S3 with RGB parallel displays.
 *          Provides drawing primitives, text rendering, and advanced features like rotation and
 *          automatic text wrapping. All functions are static for easy access without instantiation.
 * 
 * @note This library requires a pre-allocated framebuffer in PSRAM
 * @note All coordinates use the logical display dimensions (affected by rotation)
 * 
 * @example
 * @code
 * // Initialize the library
 * FastGraphics::begin(frame_buffer);
 * 
 * // Clear screen and draw some shapes
 * FastGraphics::clear(COLOR_BLACK);
 * FastGraphics::fillRect(10, 10, 100, 50, COLOR_RED);
 * FastGraphics::text(120, 25, "Hello World!", COLOR_WHITE);
 * @endcode
 */
class FastGraphics {
public:
    // =============================================================================
    // LIBRARY INITIALIZATION
    // =============================================================================
    
    /**
     * @brief Initialize the FastGraphics library
     * @details Sets up the graphics library with the provided framebuffer. Must be called
     *          before using any other graphics functions. The framebuffer should be allocated
     *          in PSRAM with size LCD_H_RES * LCD_V_RES * sizeof(uint16_t).
     * 
     * @param framebuffer Pointer to pre-allocated framebuffer (RGB565 format)
     * 
     * @note The framebuffer must remain valid for the lifetime of graphics operations
     * @note Default settings: rotation=0, text_size=1, text_color=WHITE, text_wrap=true
     * 
     * @example
     * @code
     * uint16_t* fb = (uint16_t*)heap_caps_malloc(800*480*2, MALLOC_CAP_SPIRAM);
     * FastGraphics::begin(fb);
     * @endcode
     */
    static void begin(uint16_t* framebuffer);
    
    // =============================================================================
    // SCREEN ROTATION FUNCTIONS
    // =============================================================================
    
    /**
     * @brief Set the screen rotation
     * @details Changes the display orientation and updates logical screen dimensions.
     *          All subsequent drawing operations will use the new coordinate system.
     *          Rotation affects both drawing coordinates and text layout.
     * 
     * @param rotation The desired screen rotation (ROTATION_0, ROTATION_90, ROTATION_180, ROTATION_270)
     * 
     * @note ROTATION_0/180: 800x480 (landscape), ROTATION_90/270: 480x800 (portrait)
     * @note Existing content on screen is not automatically rotated
     * 
     * @example
     * @code
     * FastGraphics::setRotation(ROTATION_90);  // Switch to portrait mode
     * // Now screen is 480x800 instead of 800x480
     * @endcode
     */
    static void setRotation(ScreenRotation rotation);
    
    /**
     * @brief Get the current screen rotation
     * @return Current ScreenRotation value
     */
    static ScreenRotation getRotation();
    
    /**
     * @brief Get the current logical display width
     * @details Returns the width in the current rotation. For ROTATION_0/180 this is LCD_H_RES,
     *          for ROTATION_90/270 this is LCD_V_RES.
     * 
     * @return Current display width in pixels
     */
    static int16_t getWidth();
    
    /**
     * @brief Get the current logical display height
     * @details Returns the height in the current rotation. For ROTATION_0/180 this is LCD_V_RES,
     *          for ROTATION_90/270 this is LCD_H_RES.
     * 
     * @return Current display height in pixels
     */
    static int16_t getHeight();
    
    // =============================================================================
    // CORE DRAWING FUNCTIONS
    // =============================================================================
    
    /**
     * @brief Clear the entire screen with a solid color
     * @param color RGB565 color value (default: COLOR_BLACK)
     * 
     * @note This is equivalent to fillRect(0, 0, getWidth(), getHeight(), color)
     * @note Very fast operation, optimized for full-screen clearing
     * 
     * @example
     * @code
     * FastGraphics::clear();                 // Clear to black
     * FastGraphics::clear(COLOR_BLUE);       // Clear to blue
     * @endcode
     */
    static void clear(uint16_t color = COLOR_BLACK);
    
    /**
     * @brief Draw a filled rectangle
     * @details Draws a solid rectangle at the specified position and size.
     *          Coordinates are clipped to screen boundaries automatically.
     * 
     * @param x Left edge X coordinate
     * @param y Top edge Y coordinate  
     * @param w Width in pixels
     * @param h Height in pixels
     * @param color RGB565 fill color
     * 
     * @note Negative coordinates are clipped to screen edges
     * @note Zero or negative width/height are ignored
     * @note Optimized with unrolled loops for maximum speed
     * 
     * @example
     * @code
     * FastGraphics::fillRect(10, 20, 100, 50, COLOR_RED);    // Red rectangle
     * FastGraphics::fillRect(0, 0, 50, 50, COLOR_GREEN);     // Green square at origin
     * @endcode
     */
    static void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    
    /**
     * @brief Draw a filled circle
     * @details Draws a solid circle using an optimized midpoint circle algorithm.
     *          The circle is drawn using horizontal line fills for maximum performance.
     * 
     * @param x0 Center X coordinate
     * @param y0 Center Y coordinate
     * @param radius Circle radius in pixels
     * @param color RGB565 fill color
     * 
     * @note Circles with radius <= 0 are ignored
     * @note Automatically clipped to screen boundaries
     * @note Uses Bresenham-based algorithm for perfect circles
     * 
     * @example
     * @code
     * FastGraphics::fillCircle(100, 100, 50, COLOR_BLUE);    // Blue circle
     * FastGraphics::fillCircle(200, 150, 25, COLOR_YELLOW);  // Smaller yellow circle
     * @endcode
     */
    static void fillCircle(int16_t x0, int16_t y0, int16_t radius, uint16_t color);
    
    /**
     * @brief Draw a line between two points
     * @details Draws a line using Bresenham's line algorithm. Optimized special cases
     *          for horizontal and vertical lines provide maximum performance.
     * 
     * @param x0 Starting point X coordinate
     * @param y0 Starting point Y coordinate
     * @param x1 Ending point X coordinate
     * @param y1 Ending point Y coordinate
     * @param color RGB565 line color
     * 
     * @note Horizontal and vertical lines use optimized fillRect calls
     * @note Diagonal lines use Bresenham algorithm for pixel-perfect results
     * @note All coordinates are automatically clipped to screen bounds
     * 
     * @example
     * @code
     * FastGraphics::line(0, 0, 100, 100, COLOR_WHITE);       // Diagonal line
     * FastGraphics::line(50, 0, 50, 200, COLOR_RED);         // Vertical line
     * FastGraphics::line(0, 100, 300, 100, COLOR_GREEN);     // Horizontal line
     * @endcode
     */
    static void line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
    
    // =============================================================================
    // SHAPE OUTLINE FUNCTIONS
    // =============================================================================
    
    /**
     * @brief Draw a rectangle outline
     * @details Draws only the outline of a rectangle using four line segments.
     *          More efficient than drawing four separate lines.
     * 
     * @param x Left edge X coordinate
     * @param y Top edge Y coordinate
     * @param w Width in pixels
     * @param h Height in pixels
     * @param color RGB565 outline color
     * 
     * @note For filled rectangles, use fillRect() instead
     * @note Width and height should be >= 2 for visible outline
     * 
     * @example
     * @code
     * FastGraphics::rect(10, 10, 100, 50, COLOR_WHITE);      // White outline
     * FastGraphics::rect(150, 75, 80, 80, COLOR_CYAN);       // Cyan square outline
     * @endcode
     */
    static void rect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    
    /**
     * @brief Draw a circle outline
     * @details Draws only the outline of a circle using an optimized midpoint algorithm.
     *          Uses 8-way symmetry for efficiency.
     * 
     * @param x0 Center X coordinate
     * @param y0 Center Y coordinate
     * @param radius Circle radius in pixels
     * @param color RGB565 outline color
     * 
     * @note For filled circles, use fillCircle() instead
     * @note Radius <= 0 circles are ignored
     * @note Very efficient due to 8-fold symmetry optimization
     * 
     * @example
     * @code
     * FastGraphics::circle(200, 200, 75, COLOR_YELLOW);      // Yellow circle outline
     * FastGraphics::circle(400, 300, 25, COLOR_MAGENTA);     // Small magenta circle
     * @endcode
     */
    static void circle(int16_t x0, int16_t y0, int16_t radius, uint16_t color);
    
    // =============================================================================
    // BASIC TEXT FUNCTIONS
    // =============================================================================
    
    /**
     * @brief Draw text at specified position
     * @details Renders text using the built-in 8x8 pixel font with optional scaling.
     *          Supports basic ASCII characters (0-127). Text can have background color.
     * 
     * @param x Left edge X coordinate for text
     * @param y Top edge Y coordinate for text
     * @param str Null-terminated string to draw
     * @param color RGB565 text color
     * @param bg RGB565 background color (default: COLOR_BLACK)
     * @param size Text scaling factor 1-10 (default: 1)
     * 
     * @note Each character is 8x8 pixels at size=1
     * @note Size 2 = 16x16 pixels per character, etc.
     * @note Newline characters (\n) move to next line
     * @note Characters outside ASCII 0-127 range are ignored
     * 
     * @example
     * @code
     * FastGraphics::text(10, 10, "Hello", COLOR_WHITE);                    // Basic text
     * FastGraphics::text(10, 30, "Large", COLOR_RED, COLOR_BLUE, 2);      // 2x size with blue background
     * FastGraphics::text(10, 70, "Line1\nLine2", COLOR_GREEN);            // Multi-line text
     * @endcode
     */
    static void text(int16_t x, int16_t y, const char* str, uint16_t color, uint16_t bg = COLOR_BLACK, uint8_t size = 1);
    
    /**
     * @brief Draw small text (size 1)
     * @details Convenience function for drawing text at size 1 with black background.
     * 
     * @param x Left edge X coordinate
     * @param y Top edge Y coordinate
     * @param str Text string to draw
     * @param color RGB565 text color
     * 
     * @see text() for full parameter control
     */
    static void textSmall(int16_t x, int16_t y, const char* str, uint16_t color);
    
    /**
     * @brief Draw medium text (size 2)
     * @details Convenience function for drawing text at size 2 with black background.
     * 
     * @param x Left edge X coordinate
     * @param y Top edge Y coordinate
     * @param str Text string to draw
     * @param color RGB565 text color
     * 
     * @see text() for full parameter control
     */
    static void textMedium(int16_t x, int16_t y, const char* str, uint16_t color);
    
    /**
     * @brief Draw large text (size 3)
     * @details Convenience function for drawing text at size 3 with black background.
     * 
     * @param x Left edge X coordinate
     * @param y Top edge Y coordinate
     * @param str Text string to draw
     * @param color RGB565 text color
     * 
     * @see text() for full parameter control
     */
    static void textLarge(int16_t x, int16_t y, const char* str, uint16_t color);
    
    // =============================================================================
    // ADVANCED TEXT CURSOR FUNCTIONS
    // =============================================================================
    
    /**
     * @brief Set the text cursor position
     * @details Sets the position where the next print() or println() operation will begin.
     *          The cursor automatically advances as text is printed.
     * 
     * @param x X coordinate for cursor
     * @param y Y coordinate for cursor
     * 
     * @note Cursor position is in pixels, not character cells
     * @note Used by all print() and println() functions
     * 
     * @example
     * @code
     * FastGraphics::setCursor(100, 50);
     * FastGraphics::print("Text starts here");
     * @endcode
     */
    static void setCursor(int16_t x, int16_t y);
    
    /**
     * @brief Set text color and optional background color
     * @details Sets the colors used by print() and println() functions.
     *          Background color is used to fill behind each character.
     * 
     * @param color RGB565 text foreground color
     * @param bg RGB565 text background color (default: COLOR_BLACK)
     * 
     * @note Setting bg = color creates invisible text (useful for spacing)
     * @note Color settings persist until changed
     * 
     * @example
     * @code
     * FastGraphics::setTextColor(COLOR_WHITE);                    // White text, black background
     * FastGraphics::setTextColor(COLOR_RED, COLOR_YELLOW);        // Red text on yellow background
     * @endcode
     */
    static void setTextColor(uint16_t color, uint16_t bg = COLOR_BLACK);
    
    /**
     * @brief Set text scaling size
     * @details Sets the text scaling factor for print() and println() functions.
     *          Size 1 = 8x8 pixels per character, size 2 = 16x16, etc.
     * 
     * @param size Text scaling factor (1-10, default: 1)
     * 
     * @note Values outside 1-10 range are ignored
     * @note Larger sizes consume more memory and drawing time
     * 
     * @example
     * @code
     * FastGraphics::setTextSize(1);    // Small text
     * FastGraphics::setTextSize(3);    // Large text (24x24 pixels per char)
     * @endcode
     */
    static void setTextSize(uint8_t size);
    
    /**
     * @brief Enable or disable automatic text wrapping
     * @details Controls whether text automatically wraps to the next line when reaching
     *          the right edge of the text area.
     * 
     * @param wrap true to enable wrapping, false to disable
     * 
     * @note When disabled, text extends beyond screen boundaries
     * @note Wrapping uses the current text area boundaries
     * @note Default: enabled (true)
     * 
     * @example
     * @code
     * FastGraphics::setTextWrap(true);     // Enable automatic wrapping
     * FastGraphics::setTextWrap(false);    // Disable wrapping (text may go off-screen)
     * @endcode
     */
    static void setTextWrap(bool wrap);
    
    /**
     * @brief Set additional spacing between text lines
     * @details Sets extra pixels added between lines when text wraps or when using
     *          println(). Improves readability for dense text.
     * 
     * @param spacing Additional pixels between lines (0-20, default: 2)
     * 
     * @note Values outside 0-20 range are ignored
     * @note This is in addition to the character height
     * @note Line height = (text_size * 8) + line_spacing
     * 
     * @example
     * @code
     * FastGraphics::setLineSpacing(0);     // Tight line spacing
     * FastGraphics::setLineSpacing(4);     // More readable spacing
     * FastGraphics::setLineSpacing(10);    // Very loose spacing
     * @endcode
     */
    static void setLineSpacing(int16_t spacing);
    
    /**
     * @brief Get current line spacing setting
     * @return Current line spacing in pixels
     */
    static int16_t getLineSpacing();
    
    /**
     * @brief Get current cursor position
     * @details Returns the current text cursor position by reference.
     * 
     * @param x Reference to store current X coordinate
     * @param y Reference to store current Y coordinate
     * 
     * @example
     * @code
     * int16_t x, y;
     * FastGraphics::getCursor(x, y);
     * Serial.printf("Cursor at: %d, %d\n", x, y);
     * @endcode
     */
    static void getCursor(int16_t& x, int16_t& y);
    
    /**
     * @brief Get current cursor X coordinate
     * @return Current cursor X position in pixels
     */
    static int16_t getCursorX();
    
    /**
     * @brief Get current cursor Y coordinate
     * @return Current cursor Y position in pixels
     */
    static int16_t getCursorY();
    
    // =============================================================================
    // PRINT FUNCTIONS (Serial.print-like interface)
    // =============================================================================
    
    /**
     * @brief Print a C-style string
     * @details Prints text at the current cursor position using current text settings.
     *          Cursor automatically advances. Supports newline (\n) and carriage return (\r).
     * 
     * @param str Null-terminated string to print
     * 
     * @note Handles \n (newline) and \r (carriage return) characters
     * @note Cursor advances automatically after each character
     * @note Text wrapping applies if enabled
     * 
     * @example
     * @code
     * FastGraphics::print("Hello ");
     * FastGraphics::print("World!");     // Results in "Hello World!"
     * @endcode
     */
    static void print(const char* str);
    
    /**
     * @brief Print an Arduino String
     * @param str Arduino String object to print
     * @see print(const char*)
     */
    static void print(String str);
    
    /**
     * @brief Print a signed integer
     * @param number Integer value to print
     * 
     * @example
     * @code
     * FastGraphics::print(-123);    // Prints "-123"
     * FastGraphics::print(42);      // Prints "42"
     * @endcode
     */
    static void print(int number);
    
    /**
     * @brief Print an unsigned integer
     * @param number Unsigned integer value to print
     */
    static void print(unsigned int number);
    
    /**
     * @brief Print a long integer
     * @param number Long integer value to print
     */
    static void print(long number);
    
    /**
     * @brief Print an unsigned long integer  
     * @param number Unsigned long integer value to print
     */
    static void print(unsigned long number);
    
    /**
     * @brief Print a floating point number
     * @details Prints a float with specified decimal precision.
     * 
     * @param number Float value to print
     * @param decimals Number of decimal places (default: 2)
     * 
     * @example
     * @code
     * FastGraphics::print(3.14159);       // Prints "3.14"
     * FastGraphics::print(3.14159, 4);    // Prints "3.1416"
     * FastGraphics::print(3.14159, 0);    // Prints "3"
     * @endcode
     */
    static void print(float number, int decimals = 2);
    
    /**
     * @brief Print a double precision number
     * @param number Double value to print  
     * @param decimals Number of decimal places (default: 2)
     * @see print(float, int)
     */
    static void print(double number, int decimals = 2);
    
    /**
     * @brief Print a single character
     * @details Prints one character at the cursor position. Handles special characters
     *          like newline and carriage return.
     * 
     * @param character Character to print
     * 
     * @example
     * @code
     * FastGraphics::print('A');      // Prints "A"
     * FastGraphics::print('\n');     // Moves to next line
     * @endcode
     */
    static void print(char character);
    
    /**
     * @brief Print a boolean value
     * @param value Boolean value to print
     * 
     * @note Prints "true" or "false"
     * 
     * @example
     * @code
     * FastGraphics::print(true);     // Prints "true"
     * FastGraphics::print(false);    // Prints "false"
     * @endcode
     */
    static void print(bool value);
    
    // =============================================================================
    // PRINTLN FUNCTIONS (Print + newline)
    // =============================================================================
    
    /**
     * @brief Print a string followed by newline
     * @param str String to print (default: empty string for blank line)
     * 
     * @example
     * @code
     * FastGraphics::println("Line 1");
     * FastGraphics::println("Line 2");
     * FastGraphics::println();           // Blank line
     * @endcode
     */
    static void println(const char* str = "");
    
    /** @brief Print Arduino String + newline */ 
    static void println(String str);
    
    /** @brief Print integer + newline */
    static void println(int number);
    
    /** @brief Print unsigned integer + newline */
    static void println(unsigned int number);
    
    /** @brief Print long + newline */
    static void println(long number);
    
    /** @brief Print unsigned long + newline */
    static void println(unsigned long number);
    
    /** @brief Print float + newline */
    static void println(float number, int decimals = 2);
    
    /** @brief Print double + newline */
    static void println(double number, int decimals = 2);
    
    /** @brief Print character + newline */
    static void println(char character);
    
    /** @brief Print boolean + newline */
    static void println(bool value);
    
    // =============================================================================
    // TEXT AREA FUNCTIONS
    // =============================================================================
    
    /**
     * @brief Set a confined text area for printing
     * @details Defines a rectangular region where text operations are confined.
     *          Text wrapping and scrolling occur within this area only.
     * 
     * @param x Left edge of text area
     * @param y Top edge of text area
     * @param w Width of text area
     * @param h Height of text area
     * 
     * @note All print() functions respect this boundary
     * @note Text that exceeds the area height will scroll or reset to top
     * @note Default text area is the entire screen
     * 
     * @example
     * @code
     * // Create a text box in upper-left corner
     * FastGraphics::setTextArea(10, 10, 200, 100);
     * FastGraphics::setCursor(10, 10);
     * FastGraphics::println("This text is confined");
     * FastGraphics::println("to the defined area");
     * @endcode
     */
    static void setTextArea(int16_t x, int16_t y, int16_t w, int16_t h);
    
    /**
     * @brief Clear the current text area
     * @details Fills the current text area with the background color and resets
     *          the cursor to the top-left of the text area.
     * 
     * @note Uses current text background color for clearing
     * @note Cursor is reset to text area origin
     * 
     * @example
     * @code
     * FastGraphics::setTextArea(50, 50, 200, 100);
     * FastGraphics::println("Some text...");
     * FastGraphics::clearTextArea();  // Clear and reset cursor
     * FastGraphics::println("New text from top");
     * @endcode
     */
    static void clearTextArea();
    
    /**
     * @brief Print text with automatic word wrapping
     * @details Renders text within a specified width with intelligent word wrapping.
     *          Words are kept intact and wrapped at word boundaries when possible.
     *          Includes improved line spacing for better readability.
     * 
     * @param x Left edge X coordinate
     * @param y Top edge Y coordinate  
     * @param maxWidth Maximum width for text wrapping
     * @param str Text string to print with wrapping
     * @param color RGB565 text color
     * @param size Text scaling factor (default: 1)
     * 
     * @note Respects word boundaries - won't break words in middle
     * @note Handles spaces, newlines, and tabs appropriately
     * @note Uses current line spacing setting for vertical spacing
     * @note More intelligent than basic text wrapping
     * 
     * @example
     * @code
     * FastGraphics::printWrapped(10, 50, 300, 
     *     "This long sentence will automatically wrap at word boundaries "
     *     "when it reaches the specified maximum width.",
     *     COLOR_WHITE, 1);
     * @endcode
     */
    static void printWrapped(int16_t x, int16_t y, int16_t maxWidth, const char* str, uint16_t color, uint8_t size = 1);
    
    // =============================================================================
    // UTILITY FUNCTIONS
    // =============================================================================
    
    /**
     * @brief Draw a single pixel
     * @details Sets a single pixel to the specified color. Coordinates are automatically
     *          transformed based on current rotation and clipped to screen boundaries.
     * 
     * @param x X coordinate of pixel
     * @param y Y coordinate of pixel
     * @param color RGB565 color value
     * 
     * @note Slower than batch drawing functions - use for single pixels only
     * @note Automatically handles coordinate transformation and bounds checking
     * @note Useful for custom drawing algorithms
     * 
     * @example
     * @code
     * // Draw individual pixels
     * FastGraphics::pixel(100, 100, COLOR_RED);
     * FastGraphics::pixel(101, 100, COLOR_GREEN);
     * FastGraphics::pixel(102, 100, COLOR_BLUE);
     * @endcode
     */
    static void pixel(int16_t x, int16_t y, uint16_t color);
    
    /**
     * @brief Run a comprehensive graphics demonstration
     * @details Displays various graphics capabilities including shapes, colors, text
     *          formatting, and advanced features. Useful for testing and showcasing
     *          library capabilities.
     * 
     * @note Clears the screen and draws demo content
     * @note Demonstrates text areas, colors, wrapping, and formatting
     * @note Good for verifying library functionality
     * 
     * @example
     * @code
     * FastGraphics::begin(framebuffer);
     * FastGraphics::demo();  // Show off the library capabilities
     * @endcode
     */
    static void demo();
    
private:
    // =============================================================================
    // PRIVATE MEMBERS
    // =============================================================================
    
    static uint16_t* frame_buffer;          /**< Pointer to RGB565 framebuffer */
    static ScreenRotation current_rotation; /**< Current screen rotation */
    static int16_t display_width;           /**< Current logical display width */
    static int16_t display_height;          /**< Current logical display height */
    
    // Text cursor and settings
    static int16_t cursor_x, cursor_y;                      /**< Current text cursor position */
    static uint16_t text_color, text_bg_color;              /**< Text foreground and background colors */
    static uint8_t text_size;                               /**< Text scaling factor */
    static bool text_wrap;                                  /**< Text wrapping enabled flag */
    static int16_t text_area_x, text_area_y, text_area_w, text_area_h; /**< Text area boundaries */
    static int16_t line_spacing;                            /**< Additional spacing between lines */
    
    // =============================================================================
    // PRIVATE HELPER FUNCTIONS
    // =============================================================================
    
    /**
     * @brief Transform logical coordinates to physical framebuffer coordinates
     * @details Applies the current rotation transformation to convert logical screen
     *          coordinates to physical framebuffer coordinates.
     * 
     * @param x Reference to X coordinate (modified in-place)
     * @param y Reference to Y coordinate (modified in-place)
     * 
     * @note Called internally by all drawing functions
     * @note Handles all four rotation cases efficiently
     */
    static void transformCoordinates(int16_t& x, int16_t& y);
    
    /**
     * @brief Transform dimensions for rotated operations
     * @details Handles dimension transformations needed for certain rotated operations.
     * 
     * @param w Reference to width (may be modified)
     * @param h Reference to height (may be modified)
     * @param x X coordinate context
     * @param y Y coordinate context
     */
    static void transformDimensions(int16_t& w, int16_t& h, int16_t x, int16_t y);
    
    /**
     * @brief Draw a single character at specified position
     * @details Internal function to render one character from the 8x8 font.
     *          Handles scaling and background colors.
     * 
     * @param x Character X position
     * @param y Character Y position
     * @param c Character to draw (ASCII 0-127)
     * @param color Foreground color
     * @param bg Background color
     * @param size Scaling factor
     * 
     * @note Characters outside ASCII range 0-127 are ignored
     * @note Optimized for both size 1 and scaled characters
     */
    static void drawChar(int16_t x, int16_t y, char c, uint16_t color, uint16_t bg, uint8_t size);
    
    /**
     * @brief Advance text cursor after character output
     * @details Moves the cursor to the next character position and handles
     *          automatic line wrapping if enabled.
     * 
     * @param char_width Width of the character just printed
     * @param char_height Height of the character just printed
     * 
     * @note Handles text wrapping based on current text area
     * @note Called automatically by print functions
     */
    static void advanceCursor(uint8_t char_width, uint8_t char_height);
    
    /**
     * @brief Move cursor to next line
     * @details Moves cursor to the beginning of the next line, respecting
     *          text area boundaries and line spacing.
     * 
     * @note Includes line spacing in vertical advancement
     * @note Handles text area overflow by scrolling or resetting
     */
    static void newLine();
};

// =============================================================================
// CONVENIENCE MACROS (Optional - for even easier use)
// =============================================================================

/**
 * @def gfx_clear(color)
 * @brief Convenience macro for FastGraphics::clear()
 * @param color Fill color for screen clear
 */
#define gfx_clear(color)                FastGraphics::clear(color)

/**
 * @def gfx_fillRect(x,y,w,h,color)
 * @brief Convenience macro for FastGraphics::fillRect()
 * @param x Rectangle X coordinate
 * @param y Rectangle Y coordinate  
 * @param w Rectangle width
 * @param h Rectangle height
 * @param color Fill color
 */
#define gfx_fillRect(x,y,w,h,color)     FastGraphics::fillRect(x,y,w,h,color)

/**
 * @def gfx_fillCircle(x,y,r,color)
 * @brief Convenience macro for FastGraphics::fillCircle()
 * @param x Circle center X coordinate
 * @param y Circle center Y coordinate
 * @param r Circle radius
 * @param color Fill color
 */
#define gfx_fillCircle(x,y,r,color)     FastGraphics::fillCircle(x,y,r,color)

/**
 * @def gfx_line(x0,y0,x1,y1,color)
 * @brief Convenience macro for FastGraphics::line()
 * @param x0 Line start X coordinate
 * @param y0 Line start Y coordinate
 * @param x1 Line end X coordinate
 * @param y1 Line end Y coordinate
 * @param color Line color
 */
#define gfx_line(x0,y0,x1,y1,color)     FastGraphics::line(x0,y0,x1,y1,color)

/**
 * @def gfx_text(x,y,str,color)
 * @brief Convenience macro for FastGraphics::textSmall()
 * @param x Text X coordinate
 * @param y Text Y coordinate
 * @param str Text string
 * @param color Text color
 */
#define gfx_text(x,y,str,color)         FastGraphics::textSmall(x,y,str,color)

#endif // FAST_GRAPHICS_H