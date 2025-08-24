#include "display_manager.h"

DisplayManager::DisplayManager() : isInitialized(false), needsFullRefresh(true), lastUpdateTime(0) {
    // Initialize display data
    currentData = {0};
    lastData = {0};
}

void DisplayManager::begin() {
    Serial.println("Initializing TFT display...");
    
    // Initialize TFT
    tft.init();
    tft.setRotation(TFT_ROTATION);
    
    // Turn on backlight
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);
    Serial.println("Backlight turned ON");
    
    // Clear screen
    tft.fillScreen(TFT_BLACK);
    Serial.println("Screen cleared");
    
    // Draw test pattern with different colors
    tft.fillScreen(TFT_RED);
    delay(1000);
    tft.fillScreen(TFT_GREEN);
    delay(1000);
    tft.fillScreen(TFT_BLUE);
    delay(1000);
    tft.fillScreen(TFT_BLACK);
    
    // Draw initial test pattern
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setFreeFont(&FreeSans12pt7b);
    tft.drawString("Weather Station", 10, 10);
    tft.drawString("Initializing...", 10, 40);
    tft.drawString("TFT Test OK!", 10, 70);
    
    isInitialized = true;
    Serial.println("TFT display initialized successfully");
}

void DisplayManager::update(const DisplayData& data) {
    if (!isInitialized) return;
    
    // Check if data has actually changed
    if (!hasDataChanged(data)) {
        // Only update time if it's been more than 30 seconds
        if (millis() - lastUpdateTime > 30000) {
            updateTimeOnly();
            lastUpdateTime = millis();
        }
        return;
    }
    
    // Data has changed, do full refresh
    currentData = data;
    lastData = data;
    lastUpdateTime = millis();
    
    clearScreen();
    drawIndoorData();
    drawOutdoorData();
    drawTime();
    drawBatteryStatus();
}

void DisplayManager::clear() {
    if (!isInitialized) return;
    clearScreen();
}

void DisplayManager::setBrightness(uint8_t brightness) {
    // Note: TFT_eSPI doesn't have built-in brightness control
    // This would need to be implemented with PWM if hardware supports it
}

void DisplayManager::setRotation(uint8_t rotation) {
    if (!isInitialized) return;
    tft.setRotation(rotation);
}



void DisplayManager::clearScreen() {
    tft.fillScreen(TFT_BLACK);
}

void DisplayManager::drawIndoorData() {
    // Indoor temperature and humidity (top section)
    tft.setFreeFont(&FreeSansBold18pt7b);
    tft.setTextColor(TFT_NAVY, TFT_BLACK);
    tft.drawFloat(currentData.tempIn, 1, 3, 3);
    tft.drawNumber(currentData.humiIn, 93, 3);
    
    // IAQ values
    tft.setFreeFont(&FreeSans12pt7b);
    tft.setTextColor(TFT_NAVY, TFT_BLACK);
    tft.drawNumber(currentData.iaq, 3, 35);
    tft.drawNumber(currentData.iaqAcc, 93, 35);
}

void DisplayManager::drawOutdoorData() {
    // Outdoor temperature and humidity (middle section)
    tft.setFreeFont(&FreeSansBold18pt7b);
    tft.setTextColor(TFT_NAVY, TFT_BLACK);
    tft.drawFloat(currentData.tempOut, 1, 3, 148);
    tft.drawNumber(currentData.humiOut, 93, 148);
    
    // Pressure
    tft.setFreeFont(&FreeSans12pt7b);
    tft.setTextColor(TFT_NAVY, TFT_BLACK);
    tft.drawNumber(currentData.press, 3, 148 + 32);
}

void DisplayManager::drawTime() {
    tft.setFreeFont(&FreeSans12pt7b);
    tft.setTextColor(TFT_DARKCYAN, TFT_BLACK);
    tft.drawString(currentData.timeString, -1, 78, 7);
}

void DisplayManager::drawBatteryStatus() {
    tft.setFreeFont(&FreeSans9pt7b);
    tft.setTextColor(TFT_DARKGREEN, TFT_BLACK);
    tft.drawFloat(currentData.batV, 1, 3, 170 + 42);
    tft.drawNumber(currentData.batP, 93, 170 + 42);
}

void DisplayManager::showError(const String& message) {
    if (!isInitialized) return;
    
    clearScreen();
    tft.setFreeFont(&FreeSans12pt7b);
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.drawString("ERROR", 10, 50, 2);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.drawString(message, 10, 80, 2);
}

void DisplayManager::showStatus(const String& message) {
    if (!isInitialized) return;
    
    clearScreen();
    tft.setFreeFont(&FreeSans12pt7b);
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    tft.drawString("STATUS", 10, 50, 2);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.drawString(message, 10, 80, 2);
}

bool DisplayManager::hasDataChanged(const DisplayData& newData) const {
    // Compare critical values that would affect the display
    return (newData.tempIn != lastData.tempIn ||
            newData.humiIn != lastData.humiIn ||
            newData.iaq != lastData.iaq ||
            newData.iaqAcc != lastData.iaqAcc ||
            newData.tempOut != lastData.tempOut ||
            newData.humiOut != lastData.humiOut ||
            newData.press != lastData.press ||
            newData.batV != lastData.batV ||
            newData.batP != lastData.batP);
}

void DisplayManager::updateTimeOnly() {
    if (!isInitialized) return;
    
    // Only update the time portion without clearing the screen
    drawTime();
}

void DisplayManager::updateTime() {
    if (!isInitialized) return;
    
    // Only update the time portion without clearing the screen
    updateTimeOnly();
}
