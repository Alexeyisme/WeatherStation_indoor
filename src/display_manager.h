#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <TFT_eSPI.h>
#include <SPI.h>
#include "config.h"

struct DisplayData {
    float tempIn;
    float humiIn;
    int iaq;
    int iaqAcc;
    float tempOut;
    float humiOut;
    float press;
    float batV;
    float batP;
    String timeString;
};

class DisplayManager {
private:
    TFT_eSPI tft;
    DisplayData currentData;
    DisplayData lastData;
    bool isInitialized;
    bool needsFullRefresh;
    unsigned long lastUpdateTime;
    
    void drawIndoorData();
    void drawOutdoorData();
    void drawTime();
    void drawBatteryStatus();
    void clearScreen();
    bool hasDataChanged(const DisplayData& newData) const;
    void updateTimeOnly();
    
public:
    DisplayManager();
    
    void begin();
    void update(const DisplayData& data);
    void updateTime();
    void clear();
    void setBrightness(uint8_t brightness);
    void setRotation(uint8_t rotation);
    
    // Display status
    bool isReady() const { return isInitialized; }
    void showError(const String& message);
    void showStatus(const String& message);
};

#endif // DISPLAY_MANAGER_H
