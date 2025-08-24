#include "ble_manager.h"

BLEManager::BLEManager() : pCharacteristic(nullptr), isConnected(false), isInitialized(false) {
    resetData();
}

void BLEManager::begin() {
    Serial.println("Starting BLE server!");
    BLEDevice::init(BLE_DEVICE_NAME);
    BLEDevice::setPower(ESP_PWR_LVL_P9);
    setupBLEServer();
    isInitialized = true;
    Serial.println("BLE server initialized");
}

void BLEManager::setupBLEServer() {
    BLEServer* pServer = BLEDevice::createServer();
    BLEService* pService = pServer->createService(BLE_SERVICE_UUID);
    
    pCharacteristic = pService->createCharacteristic(
        BLE_CHARACTERISTIC_UUID,
        NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE
    );
    
    pCharacteristic->setValue("");
    pCharacteristic->setCallbacks(new CharacteristicCallbacks(this));
    pService->start();
    
    BLEAdvertising* pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(BLE_SERVICE_UUID);
    pAdvertising->setScanResponse(true);
    BLEDevice::startAdvertising();
}

void BLEManager::update() {
    // BLE updates are handled via callbacks
    // This method can be used for periodic tasks if needed
}

bool BLEManager::hasNewData() const {
    return currentData.isValid && currentData.timestamp > 0;
}

void BLEManager::resetData() {
    currentData = {0};
    currentData.isValid = false;
    currentData.timestamp = 0;
}

bool BLEManager::validateDataLength(size_t length) const {
    return length == sizeof(float) * OUTDOOR_VALUES_COUNT;
}

void BLEManager::parseOutdoorData(const std::string& data) {
    if (!validateDataLength(data.length())) {
        Serial.println("Invalid BLE data length received");
        return;
    }
    
    // Parse the 5 float values from outdoor station
    memcpy(&currentData.temperature, data.data(), sizeof(float));
    memcpy(&currentData.humidity, data.data() + sizeof(float), sizeof(float));
    memcpy(&currentData.pressure, data.data() + 2 * sizeof(float), sizeof(float));
    memcpy(&currentData.batteryVoltage, data.data() + 3 * sizeof(float), sizeof(float));
    memcpy(&currentData.batteryPercentage, data.data() + 4 * sizeof(float), sizeof(float));
    
    currentData.isValid = true;
    currentData.timestamp = millis();
    
    Serial.print("Outdoor data updated: ");
    Serial.printf("T=%.1f, H=%.1f, P=%.1f, V=%.2f, %%%.1f\n",
                  currentData.temperature, currentData.humidity, 
                  currentData.pressure, currentData.batteryVoltage, 
                  currentData.batteryPercentage);
}

// CharacteristicCallbacks implementation
void BLEManager::CharacteristicCallbacks::onWrite(BLECharacteristic* pCharacteristic) {
    std::string serializedData = pCharacteristic->getValue();
    size_t receivedLength = serializedData.length();
    
    Serial.print("Received BLE data length: ");
    Serial.println(receivedLength);
    
    if (receivedLength == sizeof(float) * OUTDOOR_VALUES_COUNT) {
        manager->parseOutdoorData(serializedData);
    } else {
        Serial.println("Invalid BLE data length!");
        Serial.print("Raw bytes: ");
        for (size_t i = 0; i < receivedLength; ++i) {
            Serial.print((uint8_t)serializedData[i], HEX);
            Serial.print(" ");
        }
        Serial.println();
    }
}
