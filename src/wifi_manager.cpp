#include "wifi_manager.h"
// #include "web_server_manager.h" // Removed - using IoTWebUIManager instead
#include "IoTWebUIManager.h"

WeatherStationWiFiManager::WeatherStationWiFiManager() 
    : isConnected(false) {
    setupWiFiManager();
}

void WeatherStationWiFiManager::setupWiFiManager() {
    wifiManager.setDebugOutput(true);
    wifiManager.setConfigPortalTimeout(WIFI_CONFIG_TIMEOUT);
    wifiManager.setCaptivePortalEnable(true);
    wifiManager.setHostname(WIFI_HOSTNAME);
    
    // Set callback for when entering config mode
    wifiManager.setAPCallback([this](WiFiManager *myWiFiManager) {
        this->onConfigMode();
    });
}

bool WeatherStationWiFiManager::connectToWiFi() {
    Serial.println("Attempting to connect to WiFi...");
    
    // Get stored WiFi credentials
    extern IoTWebUIManager* webManager;
    String wifiSSID = "";
    String wifiPassword = "";
    String hostname = WIFI_HOSTNAME;
    String apSSID = WIFI_AP_SSID;
    String apPassword = WIFI_AP_PASSWORD;
    
    if (webManager) {
        wifiSSID = webManager->getConfigValue("wifi_ssid", "");
        wifiPassword = webManager->getConfigValue("wifi_password", "");
        hostname = webManager->getConfigValue("hostname", WIFI_HOSTNAME);
        apSSID = webManager->getConfigValue("ap_ssid", WIFI_AP_SSID);
        apPassword = webManager->getConfigValue("ap_password", WIFI_AP_PASSWORD);
    }
    
    // Try to connect using stored credentials first
    if (wifiSSID.length() > 0 && wifiPassword.length() > 0) {
        Serial.println("Attempting to connect to stored WiFi: " + wifiSSID);
        WiFi.begin(wifiSSID.c_str(), wifiPassword.c_str());
        
        // Wait for connection with timeout
        int attempts = 0;
        while (WiFi.status() != WL_CONNECTED && attempts < 20) {
            delay(500);
            Serial.print(".");
            attempts++;
        }
        
        if (WiFi.status() == WL_CONNECTED) {
            Serial.println("\nWiFi connected successfully using stored credentials!");
            Serial.print("IP address: ");
            Serial.println(WiFi.localIP());
            
            // Setup mDNS
            if (!MDNS.begin(hostname.c_str())) {
                Serial.println("Error setting up mDNS responder. Continuing without mDNS.");
            } else {
                Serial.println("mDNS responder started");
                Serial.printf("Address: %s.local\n", hostname.c_str());
                MDNS.addService("http", "tcp", WEB_SERVER_PORT);
            }
            
            isConnected = true;
            return true;
        } else {
            Serial.println("\nFailed to connect using stored credentials, trying WiFiManager...");
        }
    }
    
    // Fall back to WiFiManager if no stored credentials or connection failed
    if (wifiManager.autoConnect(apSSID.c_str(), apPassword.c_str())) {
        Serial.println("WiFi connected successfully via WiFiManager!");
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
        
        // Setup mDNS
        if (!MDNS.begin(hostname.c_str())) {
            Serial.println("Error setting up mDNS responder. Continuing without mDNS.");
        } else {
            Serial.println("mDNS responder started");
            Serial.printf("Address: %s.local\n", hostname.c_str());
            MDNS.addService("http", "tcp", WEB_SERVER_PORT);
        }
        
        isConnected = true;
        return true;
    } else {
        Serial.println("Failed to connect to WiFi");
        isConnected = false;
        return false;
    }
}

void WeatherStationWiFiManager::checkConnection() {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi connection lost, attempting to reconnect...");
        isConnected = false;
        if (!connectToWiFi()) {
            Serial.println("Reconnection failed, starting config portal");
            startConfigPortalIfNeeded();
        }
    }
}

void WeatherStationWiFiManager::resetSettings() {
    Serial.println("Resetting WiFi settings...");
    wifiManager.resetSettings();
}

void WeatherStationWiFiManager::startConfigPortalIfNeeded() {
    if (!isConnected) {
        startConfigPortal();
    }
}

void WeatherStationWiFiManager::startConfigPortal() {
    Serial.println("Starting WiFi configuration portal...");
    Serial.println("Connect to WiFi network: " + String(WIFI_AP_SSID));
    Serial.println("Password: " + String(WIFI_AP_PASSWORD));
    Serial.println("Then navigate to: http://" + String(WIFI_HOSTNAME) + ".local");
    Serial.println("Or simply open any website - you'll be redirected automatically!");
    
    wifiManager.startConfigPortal(WIFI_AP_SSID, WIFI_AP_PASSWORD);
}

void WeatherStationWiFiManager::onConfigMode() {
    Serial.println("Entered WiFi config mode");
    Serial.print("AP IP address: ");
    Serial.println(WiFi.softAPIP());
}

void WeatherStationWiFiManager::loadConfigFromStorage() {
    // Access the global web interface manager to get config values
    extern IoTWebUIManager* webManager;
    
    String hostname = WIFI_HOSTNAME;
    String apSSID = WIFI_AP_SSID;
    String apPassword = WIFI_AP_PASSWORD;
    String wifiSSID = "";
    String wifiPassword = "";
    
    if (webManager) {
        hostname = webManager->getConfigValue("hostname", WIFI_HOSTNAME);
        apSSID = webManager->getConfigValue("ap_ssid", WIFI_AP_SSID);
        apPassword = webManager->getConfigValue("ap_password", WIFI_AP_PASSWORD);
        wifiSSID = webManager->getConfigValue("wifi_ssid", "");
        wifiPassword = webManager->getConfigValue("wifi_password", "");
    }
    
    // Apply the configuration
    wifiManager.setHostname(hostname.c_str());
    
    // Set WiFi credentials if available
    if (wifiSSID.length() > 0 && wifiPassword.length() > 0) {
        wifiManager.setWiFiAutoReconnect(true);
        // Note: WiFiManager will use these credentials for auto-connect
        Serial.println("Loaded WiFi credentials from storage");
    }
    
    Serial.println("Loaded WiFi config from storage:");
    Serial.println("Hostname: " + hostname);
    Serial.println("AP SSID: " + apSSID);
    Serial.println("AP Password: " + apPassword);
    Serial.println("WiFi SSID: " + (wifiSSID.length() > 0 ? wifiSSID : "Not set"));
}

void WeatherStationWiFiManager::applyConfiguration() {
    loadConfigFromStorage();
}
