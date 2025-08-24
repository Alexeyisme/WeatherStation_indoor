#include "sensor_manager.h"

SensorManager::SensorManager() 
    : gySerial(1), gyCounter(0), gySign(0) {
    resetData();
}

void SensorManager::begin() {
    gySerial.begin(GY_BAUD_RATE, SERIAL_8N1, GY_RXD_PIN, GY_TXD_PIN);
    delay(4000);
    initializeSensor();
}

void SensorManager::initializeSensor() {
    // Initialize GY-MCU680 sensor
    gySerial.write(0XA5);
    gySerial.write(0X55);
    gySerial.write(0X3F);
    gySerial.write(0X39);
    delay(100);

    gySerial.write(0XA5);
    gySerial.write(0X56);
    gySerial.write(0X02);
    gySerial.write(0XFD);
    delay(100);
    
    Serial.println("GY-MCU680 sensor initialized");
}

void SensorManager::update() {
    while (gySerial.available()) {
        gyRe_buf[gyCounter] = (unsigned char)gySerial.read();

        if (gyCounter == 0 && gyRe_buf[0] != 0x5A) {
            Serial.println("Invalid sensor data header");
            return;
        }
        if (gyCounter == 1 && gyRe_buf[1] != 0x5A) {
            gyCounter = 0;
            return;
        }
        
        gyCounter++;
        if (gyCounter == 20) {
            gyCounter = 0;
            gySign = 1;
        }
    }

    if (gySign) {
        gySign = 0;
        if (gyRe_buf[0] == 0x5A && gyRe_buf[1] == 0x5A) {
            if (validateChecksum()) {
                parseSensorValues();
            }
        }
    }
}

bool SensorManager::validateChecksum() {
    unsigned char sum = 0;
    for (int i = 0; i < 19; i++) {
        sum += gyRe_buf[i];
    }
    return sum == gyRe_buf[19];
}

void SensorManager::parseSensorValues() {
    // Parse temperature
    gyTemp2 = (gyRe_buf[4] << 8) | gyRe_buf[5];
    currentData.temperature = (float)gyTemp2 / 100;
    
    // Parse humidity
    gyTemp1 = (gyRe_buf[6] << 8) | gyRe_buf[7];
    currentData.humidity = (float)gyTemp1 / 100;
    
    // Parse pressure (24-bit value in Pa, convert to hPa)
    uint32_t pressurePa = ((uint32_t)gyRe_buf[8] << 16) | 
                          ((uint16_t)gyRe_buf[9] << 8) | 
                          gyRe_buf[10];
    currentData.pressure = (float)pressurePa / 100.0;  // Convert Pa to hPa
    currentData.altitudePressure = currentData.pressure + (ALT_CORRECTION / 100.0);
    
    // Parse IAQ
    currentData.iaqAccuracy = (gyRe_buf[11] & 0xf0) >> 4;
    currentData.iaq = ((gyRe_buf[11] & 0x0F) << 8) | gyRe_buf[12];
    
    // Parse gas
    currentData.gas = ((uint32_t)gyRe_buf[13] << 24) | 
                     ((uint32_t)gyRe_buf[14] << 16) | 
                     ((uint16_t)gyRe_buf[15] << 8) | 
                     gyRe_buf[16];
    
    // Parse altitude
    currentData.altitude = (gyRe_buf[17] << 8) | gyRe_buf[18];
    
    // Validate data
    currentData.isValid = isTemperatureValid(currentData.temperature) &&
                         isHumidityValid(currentData.humidity) &&
                         isPressureValid(currentData.pressure) &&
                         isIAQValid(currentData.iaq);
    
    currentData.timestamp = millis();
    
    if (currentData.isValid) {
        Serial.printf("Sensor data updated - Temp: %.1f°C, Humidity: %.1f%%, Pressure: %.1f hPa, IAQ: %d\n", 
                     currentData.temperature, currentData.humidity, currentData.pressure, currentData.iaq);
    } else {
        Serial.println("Invalid sensor data detected");
    }
}

bool SensorManager::hasNewData() const {
    return currentData.isValid && currentData.timestamp > 0;
}

void SensorManager::resetData() {
    currentData = {0};
    currentData.isValid = false;
    currentData.timestamp = 0;
}

bool SensorManager::isTemperatureValid(float temp) const {
    return temp >= -40.0 && temp <= 80.0;
}

bool SensorManager::isHumidityValid(float humi) const {
    return humi >= 0.0 && humi <= 100.0;
}

bool SensorManager::isPressureValid(float pres) const {
    return pres >= 300.0 && pres <= 1250.0; // 300-1250 hPa
}

bool SensorManager::isIAQValid(int iaq) const {
    return iaq >= 0 && iaq <= 500;
}
