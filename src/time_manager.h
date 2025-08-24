#ifndef TIME_MANAGER_H
#define TIME_MANAGER_H

#include <Arduino.h>
#include <ezTime.h>
#include "config.h"

// Forward declaration
class WebServerManager;

class TimeManager {
private:
    Timezone DateTime;
    bool isInitialized;
    bool isSynced;
    
    void waitForTimeSync();
    
public:
    TimeManager();
    
    void begin();
    void update();
    
    // Time access
    String getCurrentTime();
    String getCurrentDate();
    String getCurrentDateTime();
    String getFormattedTime(const String& format);
    
    // Status
    bool isReady() const { return isInitialized && isSynced; }
    bool isTimeSynced() const { return isSynced; }
    
    // Timezone
    void setTimezone(const String& timezone);
    String getTimezone() const;
    
    // Configuration
    void loadTimezoneFromConfig();
};

#endif // TIME_MANAGER_H
