#include "time_manager.h"
// #include "web_server_manager.h" // Removed - using IoTWebUIManager instead
#include "IoTWebUIManager.h"

TimeManager::TimeManager() : isInitialized(false), isSynced(false) {
}

void TimeManager::begin() {
    waitForTimeSync();
    loadTimezoneFromConfig();
    isInitialized = true;
    isSynced = true;
    
    Serial.println("Time manager initialized");
    Serial.println("UTC: " + UTC.dateTime());
    Serial.println("Local time: " + DateTime.dateTime());
}

void TimeManager::update() {
    // ezTime handles updates automatically
    // This method can be used for periodic time checks if needed
}

void TimeManager::waitForTimeSync() {
    Serial.println("Waiting for time sync...");
    waitForSync();
    Serial.println("Time synchronized successfully");
}

String TimeManager::getCurrentTime() {
    if (!isReady()) return "00:00";
    return DateTime.dateTime("H:i");
}

String TimeManager::getCurrentDate() {
    if (!isReady()) return "1970-01-01";
    return DateTime.dateTime("Y-m-d");
}

String TimeManager::getCurrentDateTime() {
    if (!isReady()) return "1970-01-01 00:00:00";
    return DateTime.dateTime();
}

String TimeManager::getFormattedTime(const String& format) {
    if (!isReady()) return "00:00";
    return DateTime.dateTime(format);
}

void TimeManager::setTimezone(const String& timezone) {
    DateTime.setLocation(timezone);
    Serial.println("Timezone set to: " + timezone);
}

String TimeManager::getTimezone() const {
    return TIMEZONE_LOCATION; // ezTime doesn't have getLocation method
}

void TimeManager::loadTimezoneFromConfig() {
    // Access the global web interface manager to get config values
    extern IoTWebUIManager* webManager;
    if (webManager) {
        String timezone = webManager->getConfigValue("timezone", TIMEZONE_LOCATION);
        DateTime.setLocation(timezone);
        Serial.println("Loaded timezone from config: " + timezone);
    } else {
        DateTime.setLocation(TIMEZONE_LOCATION);
        Serial.println("Using default timezone: " + String(TIMEZONE_LOCATION));
    }
}
