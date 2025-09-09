#ifndef TEST_CONFIG_H
#define TEST_CONFIG_H

// Test configuration for WeatherStation Indoor
// This file contains test-specific configuration values

// Test timeouts
#define TEST_TIMEOUT_MS 5000
#define TEST_DELAY_MS 100

// Test data ranges
#define TEST_TEMP_MIN -50.0
#define TEST_TEMP_MAX 100.0
#define TEST_HUMIDITY_MIN 0.0
#define TEST_HUMIDITY_MAX 100.0
#define TEST_PRESSURE_MIN 300.0
#define TEST_PRESSURE_MAX 1100.0
#define TEST_IAQ_MIN 0
#define TEST_IAQ_MAX 500

// Test WiFi credentials (dummy values)
#define TEST_WIFI_SSID "TestNetwork"
#define TEST_WIFI_PASSWORD "testpassword"

// Test BLE configuration
#define TEST_BLE_DEVICE_NAME "TestWeatherStation"
#define TEST_BLE_SERVICE_UUID "12345678-1234-1234-1234-123456789abc"
#define TEST_BLE_CHARACTERISTIC_UUID "87654321-4321-4321-4321-cba987654321"

// Test display configuration
#define TEST_DISPLAY_WIDTH 135
#define TEST_DISPLAY_HEIGHT 240
#define TEST_DISPLAY_ROTATION 0

// Test timezone
#define TEST_TIMEZONE "UTC"

// Test API endpoints
#define TEST_API_STATUS "/api/status"
#define TEST_API_CONFIG "/api/config"
#define TEST_API_RESET "/api/reset"

// Test data validation
#define TEST_VALID_TEMPERATURE 22.5
#define TEST_VALID_HUMIDITY 45.0
#define TEST_VALID_PRESSURE 1013.25
#define TEST_VALID_IAQ 25
#define TEST_VALID_BATTERY_VOLTAGE 3.7
#define TEST_VALID_BATTERY_PERCENTAGE 85.0

// Test error conditions
#define TEST_INVALID_TEMPERATURE -999.0
#define TEST_INVALID_HUMIDITY -1.0
#define TEST_INVALID_PRESSURE 0.0
#define TEST_INVALID_IAQ -1
#define TEST_INVALID_BATTERY_VOLTAGE 0.0
#define TEST_INVALID_BATTERY_PERCENTAGE -1.0

// Test JSON data
#define TEST_JSON_VALID "{\"temperature\":22.5,\"humidity\":45.0}"
#define TEST_JSON_INVALID "{ invalid json }"
#define TEST_JSON_EMPTY "{}"

// Test strings
#define TEST_STRING_SHORT "test"
#define TEST_STRING_LONG "This is a very long test string that exceeds normal length limits and should be handled properly by the system"
#define TEST_STRING_EMPTY ""
#define TEST_STRING_SPECIAL "test@#$%^&*()_+-=[]{}|;':\",./<>?"

// Test time formats
#define TEST_TIME_FORMAT_12H "%I:%M:%S %p"
#define TEST_TIME_FORMAT_24H "%H:%M:%S"
#define TEST_DATE_FORMAT "%Y-%m-%d"
#define TEST_DATETIME_FORMAT "%Y-%m-%d %H:%M:%S"

// Test memory limits
#define TEST_MAX_JSON_SIZE 4096
#define TEST_MAX_STRING_LENGTH 256
#define TEST_MAX_ARRAY_SIZE 100

// Test iteration counts
#define TEST_ITERATION_COUNT 10
#define TEST_STRESS_ITERATION_COUNT 100
#define TEST_MEMORY_TEST_ITERATIONS 50

// Test delays
#define TEST_SHORT_DELAY 10
#define TEST_MEDIUM_DELAY 100
#define TEST_LONG_DELAY 1000

// Test error messages
#define TEST_ERROR_SENSOR "Sensor Error"
#define TEST_ERROR_BLE "BLE Error"
#define TEST_ERROR_WIFI "WiFi Error"
#define TEST_ERROR_DISPLAY "Display Error"
#define TEST_ERROR_TIME "Time Error"

// Test status messages
#define TEST_STATUS_INITIALIZING "Initializing..."
#define TEST_STATUS_READY "Ready"
#define TEST_STATUS_RUNNING "Running"
#define TEST_STATUS_ERROR "Error"

#endif // TEST_CONFIG_H
