# p43lz3r ESP32 FastGFX library

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Platform: ESP32-S3](https://img.shields.io/badge/Platform-ESP32--S3-blue.svg)](https://www.espressif.com/en/products/socs/esp32-s3)
[![Display: RGB Parallel](https://img.shields.io/badge/Display-RGB%20Parallel-green.svg)](https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/api-reference/peripherals/lcd.html)

A **fast and lightweight graphics library** for ESP32-S3 microcontrollers with RGB parallel displays. Optimized for performance and ease of use, FastGraphics provides essential drawing primitives and an advanced text system perfect for embedded applications.

## 🚀 Key Features

### **Core Graphics**
- **High-performance drawing** with optimized algorithms
- **Multiple screen rotations** (0°, 90°, 180°, 270°)
- **Efficient memory usage** with PSRAM framebuffer
- **RGB565 color support** with predefined color constants

### **Advanced Text System**
- **Arduino-style print functions** (`print()`, `println()`)
- **Multiple data types** (int, float, String, bool, etc.)
- **Intelligent word wrapping** with configurable line spacing
- **Text areas** for confined output regions
- **Scalable fonts** (1x to 10x sizing)

### **Drawing Primitives**
- **Filled shapes**: rectangles, circles
- **Outline shapes**: rectangles, circles, lines
- **Individual pixel control**
- **Full-screen clearing**

## 📱 Supported Hardware

- **Microcontroller**: ESP32-S3 (with PSRAM)
- **Display**: RGB parallel interface (tested with 800x480)
- **Touch**: GT911 capacitive touch controller
- **Memory**: Minimum 768KB PSRAM for framebuffer

## 🎯 Perfect For

- **IoT dashboards** and status displays
- **Environmental monitoring** stations
- **Data logging** interfaces
- **System diagnostic** panels
- **Educational projects** and prototyping

## 📦 Quick Start

### Installation
1. Copy `FastGraphics.h` and `FastGraphics.cpp` to your project
2. Include the display configuration files
3. Initialize your RGB display and allocate framebuffer

### Basic Usage
```cpp
#include "FastGraphics.h"

// Initialize (after display setup)
FastGraphics::begin(frame_buffer);

// Clear screen
FastGraphics::clear(COLOR_BLACK);

// Draw shapes
FastGraphics::fillRect(10, 10, 100, 50, COLOR_RED);
FastGraphics::fillCircle(200, 100, 30, COLOR_BLUE);
FastGraphics::line(0, 0, 100, 100, COLOR_WHITE);

// Text output (Arduino-style)
FastGraphics::setCursor(10, 200);
FastGraphics::setTextColor(COLOR_GREEN);
FastGraphics::setTextSize(2);
FastGraphics::println("Hello World!");

// Print sensor data
FastGraphics::print("Temperature: ");
FastGraphics::print(23.5, 1);
FastGraphics::println(" °C");

// Update display
esp_lcd_panel_draw_bitmap(panel_handle, 0, 0, 800, 480, frame_buffer);
```

### Advanced Text Features
```cpp
// Configure text area
FastGraphics::setTextArea(50, 50, 300, 200);
FastGraphics::setLineSpacing(4);  // Extra spacing for readability

// Word wrapping
FastGraphics::printWrapped(10, 100, 400, 
    "This long text will automatically wrap at word boundaries "
    "when it reaches the specified width.", 
    COLOR_CYAN, 1);

// Multiple data types
FastGraphics::println("System Status:");
FastGraphics::print("Uptime: ");
FastGraphics::print(millis() / 1000);
FastGraphics::println(" seconds");
FastGraphics::print("Memory: ");
FastGraphics::print(85.7, 1);
FastGraphics::println("% free");
```

## 🎨 Available Colors

```cpp
COLOR_BLACK, COLOR_WHITE, COLOR_RED, COLOR_GREEN, COLOR_BLUE,
COLOR_YELLOW, COLOR_MAGENTA, COLOR_CYAN, COLOR_GRAY, 
COLOR_ORANGE, COLOR_PURPLE
```

## 📐 Screen Rotation

```cpp
FastGraphics::setRotation(ROTATION_0);    // 800x480 landscape
FastGraphics::setRotation(ROTATION_90);   // 480x800 portrait
FastGraphics::setRotation(ROTATION_180);  // 800x480 landscape flipped
FastGraphics::setRotation(ROTATION_270);  // 480x800 portrait flipped
```

## 🔧 Configuration

The library uses several configurable parameters:

```cpp
// Display dimensions (adjust for your display)
#define LCD_H_RES 800
#define LCD_V_RES 480

// Text settings
FastGraphics::setTextSize(2);        // 1-10x scaling
FastGraphics::setLineSpacing(4);     // Extra pixels between lines
FastGraphics::setTextWrap(true);     // Enable word wrapping
```

## 📊 Performance

- **Optimized algorithms**: Bresenham line/circle drawing
- **Unrolled loops**: 4-pixel batching for fills
- **Smart rotation**: Different code paths for performance
- **Memory efficient**: Direct framebuffer access

### Benchmarks (800x480 display)
- **Full screen clear**: ~2ms
- **Large rectangle fill**: ~1ms
- **Text rendering**: ~0.5ms per character
- **Circle fill (50px radius)**: ~3ms

## ✅ What's Included

### **Core Drawing**
- ✅ Filled rectangles and circles
- ✅ Rectangle and circle outlines  
- ✅ Lines (horizontal, vertical, diagonal)
- ✅ Individual pixel control
- ✅ Screen clearing

### **Text System**
- ✅ Built-in 8x8 pixel font
- ✅ Scalable text (1x to 10x)
- ✅ Multiple colors and backgrounds
- ✅ Print functions for all data types
- ✅ Word wrapping with line spacing
- ✅ Text areas and cursor control

### **Advanced Features**
- ✅ Screen rotation (4 orientations)
- ✅ RGB565 color system
- ✅ PSRAM framebuffer support
- ✅ Touch integration example
- ✅ Comprehensive documentation

## 🚧 What We Need to Add

### **Basic Shapes**
- **Triangles** (filled and outline)
- **Rounded rectangles** with configurable corner radius
- **Ellipses** and oval shapes
- **Polygons** with arbitrary vertices

### **Bitmap Support**  
- **Image loading** from flash/SD card
- **Bitmap blitting** with transparency
- **Image scaling** and rotation
- **Sprite handling** for animations

### **Text Utilities**
- **Text measurement** functions (`getTextWidth()`, `getTextHeight()`)
- **Font loading** from files
- **Multiple font sizes** and styles
- **Text alignment** (center, right, justify)

### **Advanced Graphics**
- **Anti-aliased** lines and shapes
- **Gradient fills** (linear, radial)
- **Pattern fills** and textures
- **Alpha blending** and transparency

### **Performance Enhancements**
- **DMA integration** for large operations
- **Hardware acceleration** utilization
- **Double buffering** support
- **Clipping regions** for complex layouts

### **Memory Optimization**
- **Palette mode** for reduced memory usage
- **Tiled rendering** for large displays
- **Compressed fonts** and images
- **Configurable framebuffer** sizes

## 🔬 Architecture

```
FastGraphics Library
├── Core Drawing Engine
│   ├── Coordinate transformation
│   ├── Bounds checking
│   └── Optimized algorithms
├── Text Rendering System
│   ├── Font management
│   ├── Layout engine
│   └── Print functions
└── Display Interface
    ├── Framebuffer management
    ├── Rotation handling
    └── Hardware abstraction
```

## 📁 File Structure

```
FastGraphics/
├── src/
│   ├── FastGraphics.h          # Main library header
│   ├── FastGraphics.cpp        # Implementation
│   ├── display_config.h        # Display configuration
│   ├── display_config.cpp      # Display initialization
│   ├── simple_touch.h          # Touch controller interface
│   └── simple_touch.cpp        # Touch implementation
├── examples/
│   ├── basic_demo/             # Simple usage example
│   ├── sensor_display/         # IoT dashboard example
│   └── touch_drawing/          # Interactive drawing
├── docs/
│   ├── API_Reference.md        # Complete API documentation
│   └── Hardware_Setup.md       # Wiring and configuration
└── README.md                   # This file
```

## 🎯 Use Cases

### **Industrial Monitoring**
```cpp
// Display sensor readings with status colors
FastGraphics::setTextColor(temp > 50 ? COLOR_RED : COLOR_GREEN);
FastGraphics::print("Temperature: ");
FastGraphics::print(temp, 1);
FastGraphics::println(" °C");
```

### **IoT Dashboard**
```cpp
// Multi-line status display
FastGraphics::println("System Status:");
FastGraphics::setTextColor(wifi_connected ? COLOR_GREEN : COLOR_RED);
FastGraphics::println(wifi_connected ? "WiFi: Connected" : "WiFi: Disconnected");
FastGraphics::setTextColor(COLOR_BLUE);
FastGraphics::print("Uptime: ");
FastGraphics::print(uptime_hours);
FastGraphics::println(" hours");
```

### **Data Logging Display**
```cpp
// Tabular data with formatting
FastGraphics::setCursor(10, 100);
FastGraphics::println("Sensor Log:");
for (int i = 0; i < log_entries; i++) {
    FastGraphics::print(log_time[i]);
    FastGraphics::print("  ");
    FastGraphics::print(log_value[i], 2);
    FastGraphics::println(" units");
}
```

## 🤝 Contributing

We welcome contributions! Areas where help is needed:

1. **Performance optimization** - DMA integration, hardware acceleration
2. **New features** - Image support, advanced shapes, fonts
3. **Platform support** - Other ESP32 variants, different displays
4. **Documentation** - Examples, tutorials, API improvements
5. **Testing** - Different hardware configurations, edge cases

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- **ESP-IDF** for RGB LCD peripheral support
- **Arduino community** for inspiration on the print() interface
- **LVGL project** for embedded graphics best practices
- **Contributors** who help improve this library


---

**FastGraphics** - Simple, fast, and powerful graphics for embedded systems! 🚀
