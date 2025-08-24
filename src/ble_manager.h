#ifndef BLE_MANAGER_H
#define BLE_MANAGER_H

#include <NimBLEDevice.h>
#include "config.h"

struct OutdoorData {
    float temperature;
    float humidity;
    float pressure;
    float batteryVoltage;
    float batteryPercentage;
    bool isValid;
    unsigned long timestamp;
};

class BLEManager {
private:
    BLECharacteristic* pCharacteristic;
    OutdoorData currentData;
    bool isConnected;
    bool isInitialized;
    
    class CharacteristicCallbacks : public BLECharacteristicCallbacks {
    private:
        BLEManager* manager;
    public:
        CharacteristicCallbacks(BLEManager* mgr) : manager(mgr) {}
        void onWrite(BLECharacteristic* pCharacteristic) override;
    };
    
    void setupBLEServer();
    void parseOutdoorData(const std::string& data);
    bool validateDataLength(size_t length) const;
    
public:
    BLEManager();
    
    void begin();
    void update();
    OutdoorData getData() const { return currentData; }
    bool hasNewData() const;
    bool isBLEConnected() const { return isConnected; }
    bool isReady() const { return isInitialized; }
    
    // Data validation
    bool isOutdoorDataValid() const { return currentData.isValid; }
    void resetData();
};

#endif // BLE_MANAGER_H
