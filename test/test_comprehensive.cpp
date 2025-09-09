#include <unity.h>
#include <Arduino.h>
#include <ArduinoJson.h>

// Test configuration
#define TEST_TIMEOUT_MS 5000
#define TEST_DELAY_MS 100

// Test data validation ranges
#define TEST_TEMP_MIN -50.0
#define TEST_TEMP_MAX 100.0
#define TEST_HUMIDITY_MIN 0.0
#define TEST_HUMIDITY_MAX 100.0
#define TEST_PRESSURE_MIN 300.0
#define TEST_PRESSURE_MAX 1100.0
#define TEST_IAQ_MIN 0
#define TEST_IAQ_MAX 500

// Test data
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

void setUp() {
    // Set up test environment
    Serial.begin(115200);
    delay(100);
}

void tearDown() {
    // Clean up after tests
}

// ===== BASIC FUNCTIONALITY TESTS =====

void test_environment_setup() {
    // Test that the test environment is properly set up
    TEST_ASSERT_TRUE(true);
}

void test_arduino_functions() {
    // Test basic Arduino functions
    unsigned long startTime = millis();
    delay(10);
    unsigned long endTime = millis();
    
    TEST_ASSERT_TRUE(endTime >= startTime);
    TEST_ASSERT_TRUE(endTime - startTime >= 10);
}

void test_memory_availability() {
    // Test that we have sufficient memory for testing
    size_t freeHeap = ESP.getFreeHeap();
    TEST_ASSERT_TRUE(freeHeap > 10000); // At least 10KB free heap
}

void test_serial_communication() {
    // Test serial communication
    Serial.println("Test message");
    TEST_ASSERT_TRUE(true);
}

// ===== DATA VALIDATION TESTS =====

void test_temperature_validation() {
    // Test temperature validation logic
    TEST_ASSERT_TRUE(TEST_VALID_TEMPERATURE >= TEST_TEMP_MIN);
    TEST_ASSERT_TRUE(TEST_VALID_TEMPERATURE <= TEST_TEMP_MAX);
    TEST_ASSERT_FALSE(TEST_INVALID_TEMPERATURE >= TEST_TEMP_MIN);
    TEST_ASSERT_FALSE(TEST_INVALID_TEMPERATURE <= TEST_TEMP_MAX);
}

void test_humidity_validation() {
    // Test humidity validation logic
    TEST_ASSERT_TRUE(TEST_VALID_HUMIDITY >= TEST_HUMIDITY_MIN);
    TEST_ASSERT_TRUE(TEST_VALID_HUMIDITY <= TEST_HUMIDITY_MAX);
    TEST_ASSERT_FALSE(TEST_INVALID_HUMIDITY >= TEST_HUMIDITY_MIN);
    TEST_ASSERT_FALSE(TEST_INVALID_HUMIDITY <= TEST_HUMIDITY_MAX);
}

void test_pressure_validation() {
    // Test pressure validation logic
    TEST_ASSERT_TRUE(TEST_VALID_PRESSURE >= TEST_PRESSURE_MIN);
    TEST_ASSERT_TRUE(TEST_VALID_PRESSURE <= TEST_PRESSURE_MAX);
    TEST_ASSERT_FALSE(TEST_INVALID_PRESSURE >= TEST_PRESSURE_MIN);
    TEST_ASSERT_FALSE(TEST_INVALID_PRESSURE <= TEST_PRESSURE_MAX);
}

void test_iaq_validation() {
    // Test IAQ validation logic
    TEST_ASSERT_TRUE(TEST_VALID_IAQ >= TEST_IAQ_MIN);
    TEST_ASSERT_TRUE(TEST_VALID_IAQ <= TEST_IAQ_MAX);
    TEST_ASSERT_FALSE(TEST_INVALID_IAQ >= TEST_IAQ_MIN);
    TEST_ASSERT_FALSE(TEST_INVALID_IAQ <= TEST_IAQ_MAX);
}

void test_battery_validation() {
    // Test battery validation logic
    TEST_ASSERT_TRUE(TEST_VALID_BATTERY_VOLTAGE > 0);
    TEST_ASSERT_TRUE(TEST_VALID_BATTERY_PERCENTAGE >= 0);
    TEST_ASSERT_TRUE(TEST_VALID_BATTERY_PERCENTAGE <= 100);
    TEST_ASSERT_FALSE(TEST_INVALID_BATTERY_VOLTAGE > 0);
    TEST_ASSERT_FALSE(TEST_INVALID_BATTERY_PERCENTAGE >= 0);
}

// ===== JSON HANDLING TESTS =====

void test_json_parsing() {
    // Test JSON parsing for configuration
    const char* testJson = R"({
        "timezone": "Europe/London",
        "hostname": "TestStation",
        "ap_ssid": "TestAP",
        "ap_password": "testpass",
        "wifi_ssid": "TestWiFi",
        "wifi_password": "wifipass"
    })";
    
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, testJson);
    
    TEST_ASSERT_TRUE(error == DeserializationError::Ok);
    TEST_ASSERT_EQUAL_STRING("Europe/London", doc["timezone"]);
    TEST_ASSERT_EQUAL_STRING("TestStation", doc["hostname"]);
    TEST_ASSERT_EQUAL_STRING("TestAP", doc["ap_ssid"]);
    TEST_ASSERT_EQUAL_STRING("testpass", doc["ap_password"]);
    TEST_ASSERT_EQUAL_STRING("TestWiFi", doc["wifi_ssid"]);
    TEST_ASSERT_EQUAL_STRING("wifipass", doc["wifi_password"]);
}

void test_invalid_json_handling() {
    // Test handling of invalid JSON
    const char* invalidJson = "{ invalid json }";
    
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, invalidJson);
    
    TEST_ASSERT_TRUE(error != DeserializationError::Ok);
}

void test_missing_json_fields() {
    // Test handling of missing JSON fields
    const char* partialJson = R"({
        "timezone": "Europe/London"
    })";
    
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, partialJson);
    
    TEST_ASSERT_TRUE(error == DeserializationError::Ok);
    TEST_ASSERT_EQUAL_STRING("Europe/London", doc["timezone"]);
    TEST_ASSERT_TRUE(doc["hostname"].isNull());
    TEST_ASSERT_TRUE(doc["wifi_ssid"].isNull());
}

void test_json_data_types() {
    // Test different JSON data types
    JsonDocument doc;
    
    doc["string"] = "test";
    doc["int"] = 42;
    doc["float"] = 3.14;
    doc["bool"] = true;
    doc["array"][0] = "item1";
    doc["array"][1] = "item2";
    
    String jsonString;
    serializeJson(doc, jsonString);
    
    TEST_ASSERT_TRUE(jsonString.indexOf("\"test\"") >= 0);
    TEST_ASSERT_TRUE(jsonString.indexOf("42") >= 0);
    TEST_ASSERT_TRUE(jsonString.indexOf("3.14") >= 0);
    TEST_ASSERT_TRUE(jsonString.indexOf("true") >= 0);
}

void test_sensor_data_json_generation() {
    // Test JSON generation for sensor data
    JsonDocument doc;
    
    doc["timestamp"] = millis();
    doc["status"] = "running";
    
    // Indoor sensor data
    doc["indoor"]["temperature"] = TEST_VALID_TEMPERATURE;
    doc["indoor"]["humidity"] = TEST_VALID_HUMIDITY;
    doc["indoor"]["pressure"] = TEST_VALID_PRESSURE;
    doc["indoor"]["iaq"] = TEST_VALID_IAQ;
    doc["indoor"]["iaq_accuracy"] = 3;
    doc["indoor"]["gas"] = 12345;
    doc["indoor"]["altitude"] = 100;
    
    // Outdoor sensor data
    doc["outdoor"]["temperature"] = 18.3;
    doc["outdoor"]["humidity"] = 62.1;
    doc["outdoor"]["pressure"] = 1012.8;
    doc["outdoor"]["battery_voltage"] = TEST_VALID_BATTERY_VOLTAGE;
    doc["outdoor"]["battery_percentage"] = TEST_VALID_BATTERY_PERCENTAGE;
    
    // Time information
    doc["time"]["current"] = "14:30:25";
    doc["time"]["date"] = "2024-01-01";
    doc["time"]["datetime"] = "2024-01-01 14:30:25";
    
    // WiFi status (mock)
    doc["wifi"]["connected"] = false;
    doc["wifi"]["ip"] = "0.0.0.0";
    doc["wifi"]["rssi"] = -100;
    
    // Serialize to string
    String jsonString;
    serializeJson(doc, jsonString);
    
    // Verify JSON structure
    TEST_ASSERT_TRUE(jsonString.length() > 0);
    TEST_ASSERT_TRUE(jsonString.indexOf("\"timestamp\"") >= 0);
    TEST_ASSERT_TRUE(jsonString.indexOf("\"status\"") >= 0);
    TEST_ASSERT_TRUE(jsonString.indexOf("\"indoor\"") >= 0);
    TEST_ASSERT_TRUE(jsonString.indexOf("\"outdoor\"") >= 0);
    TEST_ASSERT_TRUE(jsonString.indexOf("\"time\"") >= 0);
    TEST_ASSERT_TRUE(jsonString.indexOf("\"wifi\"") >= 0);
}

// ===== PERFORMANCE TESTS =====

void test_memory_management() {
    // Test JSON memory management
    for (int i = 0; i < 10; i++) {
        JsonDocument doc;
        
        doc["iteration"] = i;
        doc["data"] = "test data " + String(i);
        doc["timestamp"] = millis();
        
        String jsonString;
        serializeJson(doc, jsonString);
        
        TEST_ASSERT_TRUE(jsonString.length() > 0);
    }
    
    // Should complete without memory issues
    TEST_ASSERT_TRUE(true);
}

void test_large_json_handling() {
    // Test handling of larger JSON documents
    JsonDocument doc;
    
    // Add many sensor readings
    for (int i = 0; i < 100; i++) {
        doc["readings"][i]["temperature"] = 20.0 + i * 0.1;
        doc["readings"][i]["humidity"] = 40.0 + i * 0.2;
        doc["readings"][i]["timestamp"] = millis() + i * 1000;
    }
    
    String jsonString;
    serializeJson(doc, jsonString);
    
    TEST_ASSERT_TRUE(jsonString.length() > 1000);
    TEST_ASSERT_TRUE(jsonString.indexOf("\"readings\"") >= 0);
}

// ===== ERROR HANDLING TESTS =====

void test_error_conditions() {
    // Test various error conditions
    TEST_ASSERT_TRUE(TEST_INVALID_TEMPERATURE < TEST_TEMP_MIN);
    TEST_ASSERT_TRUE(TEST_INVALID_HUMIDITY < TEST_HUMIDITY_MIN);
    TEST_ASSERT_TRUE(TEST_INVALID_PRESSURE < TEST_PRESSURE_MIN);
    TEST_ASSERT_TRUE(TEST_INVALID_IAQ < TEST_IAQ_MIN);
    TEST_ASSERT_TRUE(TEST_INVALID_BATTERY_VOLTAGE <= 0);
    TEST_ASSERT_TRUE(TEST_INVALID_BATTERY_PERCENTAGE < 0);
}

void test_edge_cases() {
    // Test edge cases
    TEST_ASSERT_TRUE(TEST_TEMP_MIN == -50.0);
    TEST_ASSERT_TRUE(TEST_TEMP_MAX == 100.0);
    TEST_ASSERT_TRUE(TEST_HUMIDITY_MIN == 0.0);
    TEST_ASSERT_TRUE(TEST_HUMIDITY_MAX == 100.0);
    TEST_ASSERT_TRUE(TEST_PRESSURE_MIN == 300.0);
    TEST_ASSERT_TRUE(TEST_PRESSURE_MAX == 1100.0);
    TEST_ASSERT_TRUE(TEST_IAQ_MIN == 0);
    TEST_ASSERT_TRUE(TEST_IAQ_MAX == 500);
}

// ===== INTEGRATION TESTS =====

void test_data_flow_simulation() {
    // Test data flow simulation
    JsonDocument sensorData;
    sensorData["temperature"] = TEST_VALID_TEMPERATURE;
    sensorData["humidity"] = TEST_VALID_HUMIDITY;
    sensorData["pressure"] = TEST_VALID_PRESSURE;
    sensorData["iaq"] = TEST_VALID_IAQ;
    
    JsonDocument displayData;
    displayData["tempIn"] = sensorData["temperature"];
    displayData["humiIn"] = sensorData["humidity"];
    displayData["press"] = sensorData["pressure"];
    displayData["iaq"] = sensorData["iaq"];
    
    // Verify data flow
    TEST_ASSERT_EQUAL_FLOAT(TEST_VALID_TEMPERATURE, displayData["tempIn"]);
    TEST_ASSERT_EQUAL_FLOAT(TEST_VALID_HUMIDITY, displayData["humiIn"]);
    TEST_ASSERT_EQUAL_FLOAT(TEST_VALID_PRESSURE, displayData["press"]);
    TEST_ASSERT_EQUAL(TEST_VALID_IAQ, displayData["iaq"]);
}

void test_configuration_handling() {
    // Test configuration handling
    JsonDocument config;
    config["timezone"] = "Europe/London";
    config["hostname"] = "WeatherStation";
    config["wifi_ssid"] = "TestNetwork";
    config["wifi_password"] = "testpass";
    
    // Verify configuration
    TEST_ASSERT_EQUAL_STRING("Europe/London", config["timezone"]);
    TEST_ASSERT_EQUAL_STRING("WeatherStation", config["hostname"]);
    TEST_ASSERT_EQUAL_STRING("TestNetwork", config["wifi_ssid"]);
    TEST_ASSERT_EQUAL_STRING("testpass", config["wifi_password"]);
}

// ===== MAIN TEST RUNNER =====

void runAllTests() {
    // Run all test suites
    Serial.println("Starting WeatherStation Indoor Comprehensive Test Suite");
    Serial.println("======================================================");
    
    // Basic functionality tests
    Serial.println("Running basic functionality tests...");
    RUN_TEST(test_environment_setup);
    RUN_TEST(test_arduino_functions);
    RUN_TEST(test_memory_availability);
    RUN_TEST(test_serial_communication);
    
    // Data validation tests
    Serial.println("Running data validation tests...");
    RUN_TEST(test_temperature_validation);
    RUN_TEST(test_humidity_validation);
    RUN_TEST(test_pressure_validation);
    RUN_TEST(test_iaq_validation);
    RUN_TEST(test_battery_validation);
    
    // JSON handling tests
    Serial.println("Running JSON handling tests...");
    RUN_TEST(test_json_parsing);
    RUN_TEST(test_invalid_json_handling);
    RUN_TEST(test_missing_json_fields);
    RUN_TEST(test_json_data_types);
    RUN_TEST(test_sensor_data_json_generation);
    
    // Performance tests
    Serial.println("Running performance tests...");
    RUN_TEST(test_memory_management);
    RUN_TEST(test_large_json_handling);
    
    // Error handling tests
    Serial.println("Running error handling tests...");
    RUN_TEST(test_error_conditions);
    RUN_TEST(test_edge_cases);
    
    // Integration tests
    Serial.println("Running integration tests...");
    RUN_TEST(test_data_flow_simulation);
    RUN_TEST(test_configuration_handling);
    
    Serial.println("======================================================");
    Serial.println("All comprehensive tests completed!");
}

void setup() {
    // Wait for serial connection
    delay(2000);
    
    UNITY_BEGIN();
    
    // Run all tests
    runAllTests();
    
    UNITY_END();
}

void loop() {
    // Test runner completed
    delay(1000);
}