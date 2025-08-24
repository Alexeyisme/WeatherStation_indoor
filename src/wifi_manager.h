#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>
#include <WiFiManager.h>
#include <ESPmDNS.h>
#include <WebServer.h>
#include "config.h"

// Forward declaration
class WebServerManager;

#ifdef __has_include
    #if __has_include("secrets.h")
        #include "secrets.h"
    #endif
#endif

class WeatherStationWiFiManager {
private:
    WiFiManager wifiManager;
    bool isConnected;
    
    void setupWiFiManager();
    void startConfigPortal();
    
public:
    WeatherStationWiFiManager();
    
    bool connectToWiFi();
    bool isWiFiConnected() const { return isConnected; }
    void checkConnection();
    void resetSettings();
    void startConfigPortalIfNeeded();
    
    // Callback for when entering config mode
    void onConfigMode();
    
    // Configuration
    void loadConfigFromStorage();
    void applyConfiguration();
};

#endif // WIFI_MANAGER_H
