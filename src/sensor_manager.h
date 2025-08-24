#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include <Arduino.h>
#include <HardwareSerial.h>
#include "config.h"

struct SensorData {
    float temperature;
    float humidity;
    float pressure;  // Changed from int to float for better precision
    float altitudePressure;
    int iaq;
    int iaqAccuracy;
    int gas;
    int altitude;
    bool isValid;
    unsigned long timestamp;
};

class SensorManager {
private:
    HardwareSerial gySerial;
    uint16_t gyTemp1;
    int16_t gyTemp2;
    unsigned char gyRe_buf[30];
    unsigned char gyCounter;
    unsigned char gySign;
    SensorData currentData;
    
    void processSensorData();
    bool validateChecksum();
    void parseSensorValues();
    void initializeSensor();
    
public:
    SensorManager();
    
    void begin();
    void update();
    SensorData getData() const { return currentData; }
    bool hasNewData() const;
    void resetData();
    
    // Data validation
    bool isTemperatureValid(float temp) const;
    bool isHumidityValid(float humi) const;
    bool isPressureValid(float pres) const;
    bool isIAQValid(int iaq) const;
};

#endif // SENSOR_MANAGER_H
