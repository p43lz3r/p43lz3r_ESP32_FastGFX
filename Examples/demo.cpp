/**
 * @file main.cpp
 * @brief ESP32-S3 FastGraphics Library Showcase Application
 * @author Gemini AI (based on user's library)
 * @date 2025
 */

 #include <Arduino.h>
 #include "display_config.h" // For panel_handle, frame_buffer, LCD_H_RES, LCD_V_RES
 #include "simple_touch.h"   // For touch_init, touch_touched, touch_last_x, touch_last_y, and ts
 #include "FastGraphics.h"   // The FastGraphics library

// --- Demo Scene Management ---
int currentScene = 0;
const int MAX_SCENES = 5; // Total number of scenes
unsigned long lastInteractionTime = 0;
const unsigned long INTERACTION_DEBOUNCE = 500; // ms

// --- For Rotation Demo Scene ---
ScreenRotation demoRotations[] = {ROTATION_0, ROTATION_90, ROTATION_180, ROTATION_270};
int currentRotationDemoIndex = 0;
const int MAX_ROTATION_DEMOS = sizeof(demoRotations) / sizeof(ScreenRotation);

// --- For Interactive Scene ---
const int NUM_TARGETS = 5;
struct TouchTarget {
    int16_t x, y, r;
    uint16_t color;
    uint16_t originalColor;
    bool hit;
};
TouchTarget touchTargets[NUM_TARGETS];
bool interactiveTargetsInitialized = false;

// Predefined colors for the palette scene
struct ColorEntry {
    const char* name;
    uint16_t color;
};
ColorEntry colorPalette[] = {
    {"BLACK", COLOR_BLACK}, {"RED", COLOR_RED}, {"GREEN", COLOR_GREEN},
    {"BLUE", COLOR_BLUE}, {"YELLOW", COLOR_YELLOW}, {"MAGENTA", COLOR_MAGENTA},
    {"CYAN", COLOR_CYAN}, {"WHITE", COLOR_WHITE}, {"ORANGE", COLOR_ORANGE},
    {"PURPLE", COLOR_PURPLE}, {"GRAY", COLOR_GRAY}
};
const int NUM_PALETTE_COLORS = sizeof(colorPalette) / sizeof(ColorEntry);


// --- Helper Functions ---

/**
 * @brief Draws a small scene indicator (e.g., "1/5") at the bottom right.
 */
void drawSceneIndicator() {
    char sceneText[10];
    sprintf(sceneText, "%d/%d", currentScene + 1, MAX_SCENES);
    int16_t indicatorX = FastGraphics::getWidth() - (strlen(sceneText) * 8 * 1) - 10; // Approx width + padding
    int16_t indicatorY = FastGraphics::getHeight() - (8 * 1) - 10; // Approx height + padding

    // Ensure it's on screen, especially after rotation
    if (indicatorX < 5) indicatorX = 5;
    if (indicatorY < 5) indicatorY = FastGraphics::getHeight() - 20; // Fallback if too high

    FastGraphics::text(indicatorX, indicatorY, sceneText, COLOR_GRAY, COLOR_BLACK, 1);
}

/**
 * @brief Helper to calculate text width for centering (approximate).
 * @param str The string.
 * @param size Text size multiplier.
 * @return Approximate width in pixels.
 */
int16_t approximateTextWidth(const char* str, uint8_t size) {
    if (!str) return 0;
    return strlen(str) * 8 * size;
}

// --- Scene Drawing Functions ---

void drawSceneWelcome() {
    FastGraphics::clear(COLOR_BLACK);
    uint16_t titleColor = COLOR_CYAN;
    uint16_t subtitleColor = COLOR_GREEN;
    uint16_t instructionColor = COLOR_YELLOW;

    FastGraphics::setTextSize(3);
    FastGraphics::text((FastGraphics::getWidth() - approximateTextWidth("FastGraphics", 3)) / 2, 30, "FastGraphics", titleColor, COLOR_BLACK, 3);

    FastGraphics::setTextSize(2);
    FastGraphics::text((FastGraphics::getWidth() - approximateTextWidth("Library Showcase!", 2)) / 2, 80, "Library Showcase!", subtitleColor, COLOR_BLACK, 2);

    FastGraphics::setTextSize(1);
    FastGraphics::text((FastGraphics::getWidth() - approximateTextWidth("Touch screen to advance", 1)) / 2, FastGraphics::getHeight() - 60, "Touch screen to advance", instructionColor, COLOR_BLACK, 1);

    // Primitives
    FastGraphics::fillRect(40, 140, 120, 60, COLOR_RED);
    FastGraphics::text(45, 160, "fillRect", COLOR_WHITE, COLOR_RED, 1);

    FastGraphics::rect(FastGraphics::getWidth() - 160, 140, 120, 60, COLOR_BLUE);
    FastGraphics::text(FastGraphics::getWidth() - 155, 160, "rect", COLOR_WHITE, COLOR_BLACK, 1);

    FastGraphics::fillCircle(100, 270, 40, COLOR_GREEN);
    FastGraphics::text(70, 265, "fillCircle", COLOR_BLACK, COLOR_GREEN, 1);

    FastGraphics::circle(FastGraphics::getWidth() - 100, 270, 40, COLOR_YELLOW);
    FastGraphics::text(FastGraphics::getWidth() - 130, 265, "circle", COLOR_BLACK, COLOR_BLACK, 1);

    FastGraphics::line(40, 350, FastGraphics::getWidth() - 40, FastGraphics::getHeight() - 100, COLOR_MAGENTA);
    FastGraphics::text(45, 355, "line", COLOR_WHITE, COLOR_BLACK, 1);

    drawSceneIndicator();
}

void drawSceneTextCapabilities() {
    FastGraphics::clear(COLOR_BLACK);
    FastGraphics::setCursor(10, 10);
    FastGraphics::setTextColor(COLOR_CYAN, COLOR_BLACK);
    FastGraphics::setTextSize(2);
    FastGraphics::println("Text Capabilities");
    FastGraphics::println();

    FastGraphics::setTextSize(1);
    FastGraphics::setTextColor(COLOR_WHITE);
    FastGraphics::textSmall(10, FastGraphics::getCursorY(), "Small Text (using textSmall)", COLOR_WHITE);
    FastGraphics::setCursor(10, FastGraphics::getCursorY() + 15);

    FastGraphics::setTextColor(COLOR_YELLOW);
    FastGraphics::textMedium(10, FastGraphics::getCursorY(), "Medium Text (textMedium)", COLOR_YELLOW);
    FastGraphics::setCursor(10, FastGraphics::getCursorY() + 25);

    FastGraphics::setTextColor(COLOR_ORANGE);
    FastGraphics::textLarge(10, FastGraphics::getCursorY(), "Large Text (textLarge)", COLOR_ORANGE);
    FastGraphics::setCursor(10, FastGraphics::getCursorY() + 35);
    FastGraphics::println();

    FastGraphics::setTextColor(COLOR_GREEN);
    FastGraphics::println("Cursor & Print Functions:");
    FastGraphics::setTextColor(COLOR_WHITE);
    FastGraphics::print("Integer: "); FastGraphics::println(12345);
    FastGraphics::print("Float: "); FastGraphics::println(3.14159f, 3); // Ensure 'f' for float
    FastGraphics::print("Boolean: "); FastGraphics::println(true);
    FastGraphics::print("Char: "); FastGraphics::println('X');
    FastGraphics::println();

    FastGraphics::setTextColor(COLOR_MAGENTA);
    FastGraphics::println("Wrapped Text (printWrapped):");
    FastGraphics::setLineSpacing(3); // A bit more spacing for wrapped text
    FastGraphics::printWrapped(10, FastGraphics::getCursorY(), FastGraphics::getWidth() - 20,
        "This is a demonstration of the printWrapped function. It intelligently wraps long lines of text at word boundaries, making content more readable within defined width constraints. Useful for dynamic content displays and user interfaces.",
        COLOR_WHITE, 1);

    drawSceneIndicator();
}

void drawSceneColorPalette() {
    FastGraphics::clear(COLOR_BLACK);
    FastGraphics::setTextColor(COLOR_CYAN, COLOR_BLACK);
    FastGraphics::setTextSize(2);
    FastGraphics::text((FastGraphics::getWidth() - approximateTextWidth("Color Palette", 2)) / 2, 10, "Color Palette", COLOR_CYAN, COLOR_BLACK, 2);

    int numCols = 3;
    int swatchWidth = (FastGraphics::getWidth() - (numCols + 1) * 10) / numCols; // 10px spacing
    int swatchHeight = 40;
    int startX = 10;
    int startY = 60;
    int currentX = startX;
    int currentY = startY;

    for (int i = 0; i < NUM_PALETTE_COLORS; ++i) {
        FastGraphics::fillRect(currentX, currentY, swatchWidth, swatchHeight, colorPalette[i].color);
        FastGraphics::setTextSize(1);
        uint16_t textColor = (colorPalette[i].color == COLOR_BLACK || colorPalette[i].color == COLOR_BLUE || colorPalette[i].color == COLOR_PURPLE || colorPalette[i].color == COLOR_RED) ? COLOR_WHITE : COLOR_BLACK;
        FastGraphics::text(currentX + 5, currentY + swatchHeight / 2 - 4, colorPalette[i].name, textColor, colorPalette[i].color, 1);

        currentX += swatchWidth + 10; // 10px spacing
        if (i % numCols == numCols - 1 || i == NUM_PALETTE_COLORS -1) { // Move to next row
            currentX = startX;
            currentY += swatchHeight + 10;
        }
        if (currentY + swatchHeight > FastGraphics::getHeight() - 30) break;
    }
    drawSceneIndicator();
}

void drawSceneRotationDemo() {
    FastGraphics::setRotation(demoRotations[currentRotationDemoIndex]);
    FastGraphics::clear(COLOR_BLACK); // Clear AFTER rotation

    FastGraphics::setTextColor(COLOR_CYAN, COLOR_BLACK);
    FastGraphics::setTextSize(2);
    FastGraphics::text(10, 10, "Screen Rotation", COLOR_CYAN, COLOR_BLACK, 2); // Positioned for new orientation

    FastGraphics::setTextSize(1);
    FastGraphics::setTextColor(COLOR_WHITE);

    char rotationInfo[60];
    const char* rotName = "";
    switch (demoRotations[currentRotationDemoIndex]) {
        case ROTATION_0:   rotName = "0 (Landscape)"; break;
        case ROTATION_90:  rotName = "90 (Portrait)"; break;
        case ROTATION_180: rotName = "180 (Flipped Landscape)"; break;
        case ROTATION_270: rotName = "270 (Flipped Portrait)"; break;
    }
    sprintf(rotationInfo, "Current: %s (%dx%d)", rotName, FastGraphics::getWidth(), FastGraphics::getHeight());
    FastGraphics::text(10, 50, rotationInfo, COLOR_WHITE, COLOR_BLACK, 1);

    FastGraphics::setTextColor(COLOR_YELLOW);
    FastGraphics::text(10, 80, "Touch to cycle rotation.", COLOR_YELLOW, COLOR_BLACK, 1);
    if (currentRotationDemoIndex == MAX_ROTATION_DEMOS - 1) {
        FastGraphics::text(10, 100, "Next touch advances to next scene.", COLOR_GREEN, COLOR_BLACK, 1);
    }

    // Asymmetric object (Arrow pointing "logical" right)
    int cx = FastGraphics::getWidth() / 2;
    int cy = FastGraphics::getHeight() / 2;
    FastGraphics::fillRect(cx - 50, cy - 10, 100, 20, COLOR_RED); // Body
    FastGraphics::line(cx + 50, cy, cx + 20, cy - 20, COLOR_RED); // Head top
    FastGraphics::line(cx + 50, cy, cx + 20, cy + 20, COLOR_RED); // Head bottom
    FastGraphics::text(cx - 45, cy - 6, "TEXT", COLOR_WHITE, COLOR_RED, 1);

    drawSceneIndicator(); // Draws based on current rotation
}


void initializeInteractiveTargets() {
    if (!interactiveTargetsInitialized) {
        for(int i=0; i<NUM_TARGETS; ++i) {
            touchTargets[i].x = random(30 + 20, FastGraphics::getWidth() - 30 - 20); // Radius + padding
            touchTargets[i].y = random(80 + 20, FastGraphics::getHeight() - 30 - 20); // Below title, radius + padding
            touchTargets[i].r = random(15, 35);
            // Ensure color is not black for visibility
            do {
                touchTargets[i].originalColor = colorPalette[random(0, NUM_PALETTE_COLORS -1)].color;
            } while (touchTargets[i].originalColor == COLOR_BLACK);
            touchTargets[i].color = touchTargets[i].originalColor;
            touchTargets[i].hit = false;
        }
        interactiveTargetsInitialized = true;
    }
}

void resetInteractiveTargets() {
     for(int i=0; i<NUM_TARGETS; ++i) {
        touchTargets[i].color = touchTargets[i].originalColor;
        touchTargets[i].hit = false;
    }
}

void drawSceneInteractive() {
    // Ensure targets are initialized for the current screen dimensions (especially after rotation)
    if(FastGraphics::getRotation() != ROTATION_0 && currentScene == 4) { // If coming from rotation scene
         interactiveTargetsInitialized = false; // Force re-init
    }
    FastGraphics::clear(COLOR_BLACK);
    initializeInteractiveTargets();


    FastGraphics::setTextColor(COLOR_CYAN, COLOR_BLACK);
    FastGraphics::setTextSize(2);
    FastGraphics::text((FastGraphics::getWidth() - approximateTextWidth("Interactive Demo", 2)) / 2, 10, "Interactive Demo", COLOR_CYAN, COLOR_BLACK, 2);

    FastGraphics::setTextSize(1);
    FastGraphics::setTextColor(COLOR_YELLOW);
    FastGraphics::text(10, 45, "Touch the circles! Touch empty space to advance.", COLOR_YELLOW, COLOR_BLACK, 1);

    for(int i=0; i<NUM_TARGETS; ++i) {
        FastGraphics::fillCircle(touchTargets[i].x, touchTargets[i].y, touchTargets[i].r, touchTargets[i].color);
        if (touchTargets[i].hit) {
            FastGraphics::circle(touchTargets[i].x, touchTargets[i].y, touchTargets[i].r + 3, COLOR_WHITE); // Highlight hit
        }
    }

    // Display last touch coordinate for feedback
    if (ts.isTouched) {
         char touchCoords[30];
         sprintf(touchCoords, "Touch: %03d, %03d", touch_last_x, touch_last_y);
         FastGraphics::text(10, FastGraphics::getHeight() - 40, touchCoords, COLOR_WHITE, COLOR_BLACK, 1);
         FastGraphics::fillCircle(touch_last_x, touch_last_y, 3, COLOR_RED); // Small dot for precise touch point
    }
    drawSceneIndicator();
}

// --- Main Application Logic ---

void drawCurrentSceneAndUpdateDisplay() {
    static int previousSceneForRotationReset = -1;

    // Reset rotation if we just left the rotation scene
    if (previousSceneForRotationReset == 3 && currentScene != 3) { // Scene 3 is rotation demo
        if (FastGraphics::getRotation() != ROTATION_0) {
            FastGraphics::setRotation(ROTATION_0);
            // No need to clear here, the new scene will do it.
            Serial.println("Rotation reset to ROTATION_0 for new scene.");
        }
        currentRotationDemoIndex = 0; // Reset rotation demo state
    }

    // If not in interactive scene, ensure its state is ready for next entry
    if (currentScene != 4) {
        interactiveTargetsInitialized = false; // Allows re-initialization with current screen dimensions
    } else {
        // Ensure interactive scene is initialized if just entered
        if(!interactiveTargetsInitialized) initializeInteractiveTargets();
    }


    switch (currentScene) {
        case 0: drawSceneWelcome(); break;
        case 1: drawSceneTextCapabilities(); break;
        case 2: drawSceneColorPalette(); break;
        case 3: drawSceneRotationDemo(); break;
        case 4: drawSceneInteractive(); break;
        default: // Should not happen
            currentScene = 0;
            drawSceneWelcome();
            break;
    }
    previousSceneForRotationReset = currentScene;

    // Actually send the framebuffer to the display
    if (panel_handle && frame_buffer) {
        esp_lcd_panel_draw_bitmap(panel_handle, 0, 0, LCD_H_RES, LCD_V_RES, frame_buffer);
    }
}

void setup() {
    Serial.begin(115200);
    Serial.println("ESP32-S3 FastGraphics Demo Starting...");
    randomSeed(analogRead(A0)); // Use a floating analog pin for random seed

    // Initialize touch controller
    touch_init();
    Serial.println(ts.isTouched ? "Touch controller seems active." : "Touch controller state unknown at init.");


    // Initialize display and framebuffer
    if (!initialize_display_and_framebuffer()) {
        Serial.println("FATAL: Display initialization FAILED! Halting.");
        while (1) { delay(1000); }
    }
    Serial.println("Display initialized.");

    // Initialize graphics library
    FastGraphics::begin(frame_buffer);
    FastGraphics::setRotation(ROTATION_0); // Ensure default rotation
    FastGraphics::setTextWrap(true);       // Default for most text
    FastGraphics::setLineSpacing(2);       // Default line spacing
    Serial.println("FastGraphics library initialized.");

    currentRotationDemoIndex = 0; // Start rotation demo from first rotation
    interactiveTargetsInitialized = false; // Ensure targets are fresh for first interactive scene

    drawCurrentSceneAndUpdateDisplay();
    lastInteractionTime = millis();
    Serial.println("Initial scene drawn. Touch screen to navigate.");
}

void loop() {
    bool sceneNeedsRedraw = false;
    bool justTouched = touch_touched(); // Poll touch once per loop

    if (justTouched && (millis() - lastInteractionTime > INTERACTION_DEBOUNCE)) {
        Serial.printf("Touch detected at: %d, %d. Current scene: %d\n", touch_last_x, touch_last_y, currentScene + 1);
        lastInteractionTime = millis();

        if (currentScene == 3) { // Rotation Demo Scene
            currentRotationDemoIndex++;
            if (currentRotationDemoIndex >= MAX_ROTATION_DEMOS) {
                currentRotationDemoIndex = 0; // Prepare for next entry into rotation scene
                currentScene = (currentScene + 1) % MAX_SCENES; // Advance to next main scene
                // Rotation will be reset by drawCurrentSceneAndUpdateDisplay's logic
            }
            sceneNeedsRedraw = true;
        } else if (currentScene == 4) { // Interactive Demo Scene
            bool targetHit = false;
            for(int i=0; i<NUM_TARGETS; ++i) {
                int dx = touchTargets[i].x - touch_last_x;
                int dy = touchTargets[i].y - touch_last_y;
                if (dx*dx + dy*dy < touchTargets[i].r * touchTargets[i].r) {
                    touchTargets[i].color = colorPalette[random(0, NUM_PALETTE_COLORS -1)].color; // Change to random color
                    while(touchTargets[i].color == COLOR_BLACK) touchTargets[i].color = colorPalette[random(0, NUM_PALETTE_COLORS -1)].color; // ensure not black
                    touchTargets[i].hit = true;
                    targetHit = true;
                    break; // Only one hit per touch
                }
            }
            if (!targetHit) { // If no target was hit, advance to next scene
                currentScene = (currentScene + 1) % MAX_SCENES;
                resetInteractiveTargets(); // Reset for next time
                interactiveTargetsInitialized = false;
            }
            sceneNeedsRedraw = true;
        } else { // All other scenes
            currentScene = (currentScene + 1) % MAX_SCENES;
            sceneNeedsRedraw = true;
             // If moving to interactive scene, reset its state
            if (currentScene == 4) {
                resetInteractiveTargets();
                interactiveTargetsInitialized = false;
            }
        }
    } else if (justTouched && currentScene == 4) {
        // For interactive scene, allow quick touch feedback even within debounce for scene change
        // This redraws the touch point but doesn't necessarily change targets yet
        sceneNeedsRedraw = true;
    }


    if (sceneNeedsRedraw) {
        drawCurrentSceneAndUpdateDisplay();
        Serial.printf("Redrawing scene %d.\n", currentScene + 1);
    }

    delay(30); // Short delay to prevent excessive polling
}
