#include <Arduino.h>
#include "config.h"
#ifdef __has_include
    #if __has_include("secrets.h")
        #include "secrets.h"
    #endif
#endif

// Manager modules
#include "wifi_manager.h"
#include "sensor_manager.h"
#include "display_manager.h"
#include "ble_manager.h"
// #include "web_server_manager.h" // Removed - using IoTWebUIManager instead
#include "time_manager.h"

// Enhanced web interface
#include <WebServer.h>
#include <Preferences.h>
#include "IoTWebUI.h"
#include "IoTWebUIManager.h"

// Global managers
WeatherStationWiFiManager wifiManager;
SensorManager sensorManager;
DisplayManager displayManager;
BLEManager bleManager;
// WebServerManager webServerManager; // Removed - using IoTWebUIManager instead
TimeManager timeManager;

// Enhanced web interface manager
IoTWebUIManager* webManager = nullptr;

// Application state
bool systemReady = false;
unsigned long lastDisplayUpdate = 0;
const unsigned long DISPLAY_UPDATE_INTERVAL = 2000; // 2 seconds

// Forward declarations
String generateSensorDataJSON();
void handleConfigSave(const String& data);
void setupCustomNavigation();
String generateHomeContent();
String generateConfigContent();
void resetHandler();


void setup()
{
  Serial.begin(SERIAL_BAUD_RATE);
  delay(100);
  
  Serial.println("WeatherStation Indoor Starting...");

  // Initialize enhanced web interface manager first so Preferences are available
  WebServer* webServer = new WebServer(80);
  webManager = new IoTWebUIManager(webServer, new Preferences(), "WeatherStation", "weatherconfig");
  webManager->begin();
  
  // Register reset endpoint
  webServer->on(ENDPOINT_RESET, resetHandler);





  // Now connect to WiFi using any stored credentials
  if (!wifiManager.connectToWiFi()) {
    Serial.println("Failed to connect to WiFi, starting config portal");
    wifiManager.startConfigPortalIfNeeded();
  }
  
  // Start the web server
  Serial.println("Starting web server on port 80...");
  webServer->begin();
  Serial.println("Web server started successfully!");
  
  // Set up custom navigation for weather station
  setupCustomNavigation();
  
  // Set up callbacks for sensor data and configuration
  webManager->setSensorDataCallback(generateSensorDataJSON);
  webManager->setConfigSaveCallback(handleConfigSave);
  webManager->setHomeContentCallback(generateHomeContent);
  webManager->setConfigContentCallback(generateConfigContent);
  

  

  
  // Apply configurations
  wifiManager.applyConfiguration();

  // Initialize time synchronization
  timeManager.begin();
  
  // Debug mDNS setup
  Serial.println("Setting up mDNS...");
  if (MDNS.begin("weatherstation")) {
    Serial.println("mDNS responder started");
    Serial.println("Address: weatherstation.local");
  } else {
    Serial.println("Error setting up mDNS responder!");
  }

  // Initialize display
  displayManager.begin();

  // Initialize sensor
  sensorManager.begin();

  // Initialize BLE server
  bleManager.begin();

  systemReady = true;
  Serial.println("WeatherStation Indoor Ready!");
}

void loop()
{
  if (!systemReady) return;

  // Update all managers
  wifiManager.checkConnection();
  sensorManager.update();
  bleManager.update();
  if (webManager) {
    webManager->handleClient(); // Use enhanced web interface manager
  }


  timeManager.update();

  // Update display with rate limiting
  unsigned long currentTime = millis();
  if (currentTime - lastDisplayUpdate >= DISPLAY_UPDATE_INTERVAL) {
    SensorData sensorData = sensorManager.getData();
    OutdoorData outdoorData = bleManager.getData();
    
    DisplayData displayData = {
      .tempIn = sensorData.temperature,
      .humiIn = sensorData.humidity,
      .iaq = sensorData.iaq,
      .iaqAcc = sensorData.iaqAccuracy,
      .tempOut = outdoorData.temperature,
      .humiOut = outdoorData.humidity,
      .press = outdoorData.pressure,
      .batV = outdoorData.batteryVoltage,
      .batP = outdoorData.batteryPercentage,
      .timeString = timeManager.getCurrentTime()
    };
    
    // Debug output
    Serial.println("Updating display...");
    Serial.println("Indoor: " + String(displayData.tempIn) + "°C, " + String(displayData.humiIn) + "%");
    Serial.println("Outdoor: " + String(displayData.tempOut) + "°C, " + String(displayData.humiOut) + "%");
    Serial.println("Time: " + displayData.timeString);
    
    displayManager.update(displayData);
    lastDisplayUpdate = currentTime;
  }
}

// ===== ENHANCED WEB INTERFACE CALLBACKS =====

String generateSensorDataJSON() {
    JsonDocument doc;
    
    doc["timestamp"] = millis();
    doc["status"] = "running";
    
    // Get sensor data from global managers
    SensorData sensorData = sensorManager.getData();
    OutdoorData outdoorData = bleManager.getData();
    
    // Indoor sensor data
    doc["indoor"]["temperature"] = sensorData.temperature;
    doc["indoor"]["humidity"] = sensorData.humidity;
    doc["indoor"]["pressure"] = sensorData.pressure;
    doc["indoor"]["iaq"] = sensorData.iaq;
    doc["indoor"]["iaq_accuracy"] = sensorData.iaqAccuracy;
    doc["indoor"]["gas"] = sensorData.gas;
    doc["indoor"]["altitude"] = sensorData.altitude;
    
    // Outdoor sensor data (from BLE)
    doc["outdoor"]["temperature"] = outdoorData.temperature;
    doc["outdoor"]["humidity"] = outdoorData.humidity;
    doc["outdoor"]["pressure"] = outdoorData.pressure;
    doc["outdoor"]["battery_voltage"] = outdoorData.batteryVoltage;
    doc["outdoor"]["battery_percentage"] = outdoorData.batteryPercentage;
    
    // Time information
    doc["time"]["current"] = timeManager.getCurrentTime();
    doc["time"]["date"] = timeManager.getCurrentDate();
    doc["time"]["datetime"] = timeManager.getCurrentDateTime();
    
    // WiFi status
    doc["wifi"]["connected"] = WiFi.status() == WL_CONNECTED;
    doc["wifi"]["ip"] = WiFi.localIP().toString();
    doc["wifi"]["rssi"] = WiFi.RSSI();
    
    String jsonString;
    serializeJson(doc, jsonString);
    return jsonString;
}

void handleConfigSave(const String& data) {
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, data);
    
    if (error) {
        Serial.println("Failed to parse JSON");
        return;
    }
    
    // Save timezone
    if (doc.containsKey("timezone")) {
        String timezone = doc["timezone"].as<String>();
        webManager->setConfigValue("timezone", timezone);
        timeManager.setTimezone(timezone);
        Serial.println("Saved timezone: " + timezone);
    }
    
    // Save WiFi settings
    if (doc.containsKey("hostname")) {
        String hostname = doc["hostname"].as<String>();
        webManager->setConfigValue("hostname", hostname);
        Serial.println("Saved hostname: " + hostname);
    }
    
    if (doc.containsKey("ap_ssid")) {
        String apSSID = doc["ap_ssid"].as<String>();
        webManager->setConfigValue("ap_ssid", apSSID);
        Serial.println("Saved AP SSID: " + apSSID);
    }
    
    if (doc.containsKey("ap_password")) {
        String apPassword = doc["ap_password"].as<String>();
        webManager->setConfigValue("ap_password", apPassword);
        Serial.println("Saved AP password: [HIDDEN]");
    }
    
    // Save home WiFi credentials
    if (doc.containsKey("wifi_ssid")) {
        String wifiSSID = doc["wifi_ssid"].as<String>();
        webManager->setConfigValue("wifi_ssid", wifiSSID);
        Serial.println("Saved WiFi SSID: " + wifiSSID);
    }
    
    if (doc.containsKey("wifi_password")) {
        String wifiPassword = doc["wifi_password"].as<String>();
        webManager->setConfigValue("wifi_password", wifiPassword);
        Serial.println("Saved WiFi password: [HIDDEN]");
    }
    
    Serial.println("Configuration saved successfully");
}

// ===== CUSTOM NAVIGATION SETUP =====

void setupCustomNavigation() {
    if (webManager) {
        webManager->setNavLabels("Sensor Data", "System Status", "Configuration");
        webManager->setCustomNavigationLinks("");
    }
}

// ===== CUSTOM CONTENT GENERATORS =====

String generateHomeContent() {
    String content = "";
    
    // Get sensor data
    SensorData sensorData = sensorManager.getData();
    OutdoorData outdoorData = bleManager.getData();
    
    String names[] = {"Temperature", "Humidity", "Pressure", "IAQ", "Outdoor Temp", "Outdoor Humidity"};
    float values[] = {
        sensorData.temperature,
        sensorData.humidity,
        sensorData.pressure,
        (float)sensorData.iaq,
        outdoorData.temperature,
        outdoorData.humidity
    };
    String units[] = {"°C", "%", "hPa", "", "°C", "%"};
    content += IoTWebUI::getDataGrid(names, values, units, 6, 1);
    
    String statusLabels[] = {"WiFi Status", "IP Address", "Free Heap", "Uptime", "Outdoor Battery"};
    String statusValues[] = {
        WiFi.status() == WL_CONNECTED ? "Connected" : "Disconnected",
        WiFi.localIP().toString(),
        String(ESP.getFreeHeap()) + " bytes",
        String(millis() / 1000) + "s",
        String(outdoorData.batteryPercentage) + "%"
    };
    content += IoTWebUI::getKeyValueList(statusLabels, statusValues, 5, "System Status");
    
    return content;
}

String generateConfigContent() {
    String content = "<form id='config-form'>";
    
    // Get current values
    String currentTimezone = webManager->getConfigValue("timezone", "Europe/Riga");
    String currentHostname = webManager->getConfigValue("hostname", "WeatherStation");
    String currentAPSSID = webManager->getConfigValue("ap_ssid", "WeatherStation");
    String currentAPPassword = webManager->getConfigValue("ap_password", "12345678");
    String currentWiFiSSID = webManager->getConfigValue("wifi_ssid", "");
    String currentWiFiPassword = webManager->getConfigValue("wifi_password", "");
    

    
    // Time settings section
    String timezoneSelect = "<select id='timezone' name='timezone'>";
    String timezones[] = {"Europe/Riga", "Europe/London", "Europe/Paris", "Europe/Berlin", 
                         "Europe/Moscow", "America/New_York", "America/Los_Angeles", 
                         "Asia/Tokyo", "UTC"};
    for (const String& tz : timezones) {
        timezoneSelect += "<option value='" + tz + "'";
        if (tz == currentTimezone) timezoneSelect += " selected";
        timezoneSelect += ">" + tz + "</option>";
    }
    timezoneSelect += "</select>";
    
    content += IoTWebUI::getSection("Time Settings",
        IoTWebUI::getFormGroup("Timezone", timezoneSelect, "select")
    );
    
    // WiFi settings section
    String wifiSection = IoTWebUI::getFormGroup("WiFi SSID", "", "text", currentWiFiSSID, "Your home WiFi network name");
    wifiSection += IoTWebUI::getFormGroup("WiFi Password", "", "password", currentWiFiPassword, "Your home WiFi password");
    content += IoTWebUI::getSection("Home WiFi Settings", wifiSection);
    
    // Device settings section
    String deviceSection = IoTWebUI::getFormGroup("Device Hostname", "", "text", currentHostname);
    deviceSection += IoTWebUI::getFormGroup("Access Point SSID", "", "text", currentAPSSID);
    deviceSection += IoTWebUI::getFormGroup("Access Point Password", "", "password", currentAPPassword);
    content += IoTWebUI::getSection("Device Settings", deviceSection);
    
    // Action buttons
    content += IoTWebUI::getButton("Save Configuration", "primary");
    content += IoTWebUI::getButton("Back to Home", "secondary", "window.location.href=\"/\"");

    content += "</form>";
    
    // Reset WiFi button outside the form to prevent form submission interference
    content += IoTWebUI::getButton("Reset WiFi", "danger", "if(confirm(&quot;Are you sure?&quot;)){window.location.href=&quot;/reset?erase=1&quot;}" );
    
    return content;
}

void resetHandler() {
    if (webManager && webManager->getServer()) {
        if (webManager->getServer()->hasArg("erase") && webManager->getServer()->arg("erase") == "1") {
            Serial.println("Erasing WiFi settings...");
            wifiManager.resetSettings();
            webManager->getServer()->send(200, "text/plain", "WiFi settings erased. Device will restart.");
            delay(1000);
            ESP.restart();
        } else if (webManager->getServer()->hasArg("restart") && webManager->getServer()->arg("restart") == "1") {
            Serial.println("Restarting device...");
            webManager->getServer()->send(200, "text/plain", "Device restarting...");
            delay(1000);
            ESP.restart();
        } else {
            Serial.println("Starting WiFi configuration portal...");
            webManager->getServer()->send(200, "text/plain", "Starting WiFi config portal. Connect to '" + String(WIFI_AP_SSID) + "' WiFi network, then visit http://" + String(WIFI_HOSTNAME) + ".local or any website.");
            delay(1000);
            wifiManager.startConfigPortalIfNeeded();
        }
    }
}