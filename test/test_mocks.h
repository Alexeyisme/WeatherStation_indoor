#ifndef TEST_MOCKS_H
#define TEST_MOCKS_H

#include <Arduino.h>
#include <WiFi.h>
#include <HardwareSerial.h>

// Mock classes for testing hardware-dependent components

// Mock WiFi class for testing
class MockWiFi {
public:
    static bool connected;
    static String ipAddress;
    static int rssi;
    static wl_status_t status;
    
    static void setConnected(bool state) {
        connected = state;
        status = state ? WL_CONNECTED : WL_DISCONNECTED;
    }
    
    static void setIPAddress(const String& ip) {
        ipAddress = ip;
    }
    
    static void setRSSI(int signal) {
        rssi = signal;
    }
    
    static bool isConnected() {
        return connected;
    }
    
    static String getIPAddress() {
        return ipAddress;
    }
    
    static int getRSSI() {
        return rssi;
    }
    
    static wl_status_t getStatus() {
        return status;
    }
    
    static void reset() {
        connected = false;
        ipAddress = "0.0.0.0";
        rssi = -100;
        status = WL_DISCONNECTED;
    }
};

// Mock HardwareSerial for testing
class MockHardwareSerial : public HardwareSerial {
public:
    MockHardwareSerial() : HardwareSerial(0) {}
    
    void begin(unsigned long baud, uint32_t config = SERIAL_8N1, int8_t rxPin = -1, int8_t txPin = -1, bool invert = false, unsigned long timeout_ms = 20000UL) {
        // Mock implementation
    }
    
    int available() override {
        return mockDataAvailable;
    }
    
    int read() override {
        if (mockDataIndex < mockDataLength) {
            return mockData[mockDataIndex++];
        }
        return -1;
    }
    
    size_t write(uint8_t data) override {
        return 1;
    }
    
    size_t write(const uint8_t* buffer, size_t size) override {
        return size;
    }
    
    void setMockData(const uint8_t* data, size_t length) {
        mockData = data;
        mockDataLength = length;
        mockDataIndex = 0;
        mockDataAvailable = length;
    }
    
    void clearMockData() {
        mockData = nullptr;
        mockDataLength = 0;
        mockDataIndex = 0;
        mockDataAvailable = 0;
    }
    
    void setAvailable(int count) {
        mockDataAvailable = count;
    }

private:
    const uint8_t* mockData = nullptr;
    size_t mockDataLength = 0;
    size_t mockDataIndex = 0;
    int mockDataAvailable = 0;
};

// Mock TFT display for testing
class MockTFT {
public:
    static bool initialized;
    static uint8_t rotation;
    static uint8_t brightness;
    static bool screenCleared;
    static String lastText;
    static int lastX, lastY;
    static uint16_t lastColor;
    
    static void init() {
        initialized = true;
    }
    
    static void setRotation(uint8_t rot) {
        rotation = rot;
    }
    
    static void setBrightness(uint8_t bright) {
        brightness = bright;
    }
    
    static void fillScreen(uint16_t color) {
        screenCleared = true;
        lastColor = color;
    }
    
    static void drawString(const String& text, int x, int y) {
        lastText = text;
        lastX = x;
        lastY = y;
    }
    
    static void reset() {
        initialized = false;
        rotation = 0;
        brightness = 255;
        screenCleared = false;
        lastText = "";
        lastX = 0;
        lastY = 0;
        lastColor = 0;
    }
};

// Mock BLE device for testing
class MockBLEDevice {
public:
    static bool initialized;
    static bool serverStarted;
    static bool characteristicCreated;
    static String deviceName;
    static String serviceUUID;
    static String characteristicUUID;
    
    static void init(const String& name) {
        initialized = true;
        deviceName = name;
    }
    
    static void startServer() {
        serverStarted = true;
    }
    
    static void createCharacteristic(const String& service, const String& characteristic) {
        serviceUUID = service;
        characteristicUUID = characteristic;
        characteristicCreated = true;
    }
    
    static void reset() {
        initialized = false;
        serverStarted = false;
        characteristicCreated = false;
        deviceName = "";
        serviceUUID = "";
        characteristicUUID = "";
    }
};

// Mock time functions for testing
class MockTime {
public:
    static unsigned long currentTime;
    static String timezone;
    static bool synced;
    
    static void setCurrentTime(unsigned long time) {
        currentTime = time;
    }
    
    static unsigned long getCurrentTime() {
        return currentTime;
    }
    
    static void setTimezone(const String& tz) {
        timezone = tz;
    }
    
    static String getTimezone() {
        return timezone;
    }
    
    static void setSynced(bool state) {
        synced = state;
    }
    
    static bool isSynced() {
        return synced;
    }
    
    static void reset() {
        currentTime = 0;
        timezone = "UTC";
        synced = false;
    }
};

// Mock preferences for testing
class MockPreferences {
public:
    static String storedValues[10];
    static String storedKeys[10];
    static int storedCount;
    
    static void begin(const String& name, bool readOnly = false) {
        // Mock implementation
    }
    
    static void putString(const String& key, const String& value) {
        if (storedCount < 10) {
            storedKeys[storedCount] = key;
            storedValues[storedCount] = value;
            storedCount++;
        }
    }
    
    static String getString(const String& key, const String& defaultValue = "") {
        for (int i = 0; i < storedCount; i++) {
            if (storedKeys[i] == key) {
                return storedValues[i];
            }
        }
        return defaultValue;
    }
    
    static void clear() {
        storedCount = 0;
        for (int i = 0; i < 10; i++) {
            storedKeys[i] = "";
            storedValues[i] = "";
        }
    }
    
    static void reset() {
        clear();
    }
};

// Initialize static members
bool MockWiFi::connected = false;
String MockWiFi::ipAddress = "0.0.0.0";
int MockWiFi::rssi = -100;
wl_status_t MockWiFi::status = WL_DISCONNECTED;

bool MockTFT::initialized = false;
uint8_t MockTFT::rotation = 0;
uint8_t MockTFT::brightness = 255;
bool MockTFT::screenCleared = false;
String MockTFT::lastText = "";
int MockTFT::lastX = 0;
int MockTFT::lastY = 0;
uint16_t MockTFT::lastColor = 0;

bool MockBLEDevice::initialized = false;
bool MockBLEDevice::serverStarted = false;
bool MockBLEDevice::characteristicCreated = false;
String MockBLEDevice::deviceName = "";
String MockBLEDevice::serviceUUID = "";
String MockBLEDevice::characteristicUUID = "";

unsigned long MockTime::currentTime = 0;
String MockTime::timezone = "UTC";
bool MockTime::synced = false;

String MockPreferences::storedValues[10];
String MockPreferences::storedKeys[10];
int MockPreferences::storedCount = 0;

#endif // TEST_MOCKS_H
