// Configuration file for WeatherStation Indoor
// This file contains default configuration values
// For sensitive data, create src/secrets.h and it will be used instead

#ifndef CONFIG_H
#define CONFIG_H

// Hardware Configuration
#define SERIAL_BAUD_RATE 115200

// WiFi Configuration
#define WIFI_HOSTNAME "weatherstation"
#define WIFI_AP_SSID "WeatherStation"
#define WIFI_AP_PASSWORD "12345678"
#define WIFI_CONFIG_TIMEOUT 180  // 3 minutes

// Web Server Configuration
#define WEB_SERVER_PORT 80
#define ENDPOINT_GET "/get"
#define ENDPOINT_RESET "/reset"

// BLE Configuration
#define BLE_SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define BLE_CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define BLE_DEVICE_NAME "Weather Station Indoor"

// GY-MCU680 Sensor Configuration
#define GY_RXD_PIN 25
#define GY_TXD_PIN 27
#define GY_BAUD_RATE 9600
#define ALT_CORRECTION 0

// Display Configuration - LilyGo T-Display
#define TFT_ROTATION 0
#define TFT_BL   4   // Backlight pin for LilyGo T-Display

// Time Configuration
#define TIMEZONE_LOCATION "Europe/Riga"

// Data Array Size
#define OUTDOOR_VALUES_COUNT 5

#endif // CONFIG_H
