# Testing Guide - WeatherStation Indoor

Comprehensive testing framework for the WeatherStation Indoor project with unit tests, integration tests, and API tests.

## Overview

The WeatherStation Indoor project now includes a complete testing framework with:

- **Unit Tests**: Individual component testing
- **Integration Tests**: Component interaction testing  
- **API Tests**: Web interface and JSON handling testing
- **Mock Tests**: Hardware-independent testing
- **Performance Tests**: Memory and performance validation

## Test Structure

```
test/
├── test_main.cpp              # Main test runner
├── test_sensor_manager.cpp    # SensorManager unit tests
├── test_ble_manager.cpp       # BLEManager unit tests
├── test_wifi_manager.cpp      # WiFiManager unit tests
├── test_display_manager.cpp   # DisplayManager unit tests
├── test_time_manager.cpp      # TimeManager unit tests
├── test_integration.cpp       # Integration tests
├── test_api.cpp              # API and JSON tests
├── test_config.h             # Test configuration constants
├── test_mocks.h              # Mock classes for testing
└── README                    # PlatformIO test documentation
```

## Running Tests

### Prerequisites

- PlatformIO Core or PlatformIO IDE
- ESP32 development board (for hardware tests)
- All project dependencies installed

### Command Line Testing

```bash
# Run all tests
pio test

# Run tests for specific environment
pio test -e test

# Run tests with verbose output
pio test -v

# Run tests and upload to device
pio test -e test --upload-port /dev/ttyUSB0

# Run specific test file
pio test --filter test_sensor_manager

# Run tests with coverage (if supported)
pio test --coverage
```

### IDE Testing

1. Open PlatformIO IDE
2. Navigate to the project
3. Go to PlatformIO → Test
4. Select test environment
5. Click "Run Tests"

## Test Categories

### 1. Unit Tests

#### SensorManager Tests (`test_sensor_manager.cpp`)
- **Initialization**: Test sensor manager setup
- **Data Validation**: Test temperature, humidity, pressure, IAQ validation
- **Data Reset**: Test data clearing functionality
- **Update Operations**: Test sensor update without data
- **New Data Detection**: Test data change detection

#### BLEManager Tests (`test_ble_manager.cpp`)
- **Initialization**: Test BLE manager setup
- **Data Validation**: Test outdoor data validation
- **Data Reset**: Test BLE data clearing
- **Connection Status**: Test BLE connection state
- **Update Operations**: Test BLE update without connection

#### WiFiManager Tests (`test_wifi_manager.cpp`)
- **Initialization**: Test WiFi manager setup
- **Connection Check**: Test connection status checking
- **Config Portal**: Test configuration portal functionality
- **Settings Reset**: Test WiFi settings reset
- **Configuration**: Test config loading and application

#### DisplayManager Tests (`test_display_manager.cpp`)
- **Initialization**: Test display manager setup
- **Data Structure**: Test DisplayData structure
- **Display Update**: Test display update functionality
- **Display Control**: Test brightness, rotation, clear operations
- **Error Handling**: Test error and status message display

#### TimeManager Tests (`test_time_manager.cpp`)
- **Initialization**: Test time manager setup
- **Time Formatting**: Test time string formatting
- **Timezone Handling**: Test timezone setting and retrieval
- **Custom Formats**: Test custom time format strings
- **Sync Status**: Test time synchronization status

### 2. Integration Tests (`test_integration.cpp`)

- **System Initialization**: Test all components initializing together
- **Data Flow**: Test data flow between components
- **Component Updates**: Test component update cycles
- **Error Handling**: Test error handling across components
- **Memory Management**: Test memory stability
- **Configuration**: Test configuration across components

### 3. API Tests (`test_api.cpp`)

- **JSON Generation**: Test sensor data JSON generation
- **JSON Parsing**: Test configuration JSON parsing
- **Invalid JSON**: Test invalid JSON handling
- **Missing Fields**: Test missing JSON field handling
- **Data Types**: Test different JSON data types
- **Large JSON**: Test large JSON document handling
- **Memory Management**: Test JSON memory management
- **API Response**: Test complete API response format

## Test Configuration

### Test Constants (`test_config.h`)

```cpp
// Test timeouts
#define TEST_TIMEOUT_MS 5000
#define TEST_DELAY_MS 100

// Test data ranges
#define TEST_TEMP_MIN -50.0
#define TEST_TEMP_MAX 100.0
#define TEST_HUMIDITY_MIN 0.0
#define TEST_HUMIDITY_MAX 100.0

// Test validation values
#define TEST_VALID_TEMPERATURE 22.5
#define TEST_VALID_HUMIDITY 45.0
#define TEST_VALID_PRESSURE 1013.25
```

### Mock Classes (`test_mocks.h`)

- **MockWiFi**: WiFi functionality mocking
- **MockHardwareSerial**: Serial communication mocking
- **MockTFT**: Display functionality mocking
- **MockBLEDevice**: BLE functionality mocking
- **MockTime**: Time functionality mocking
- **MockPreferences**: Preferences storage mocking

## Test Coverage

### Current Coverage Areas

✅ **SensorManager**: 100% method coverage
✅ **BLEManager**: 100% method coverage  
✅ **WiFiManager**: 100% method coverage
✅ **DisplayManager**: 100% method coverage
✅ **TimeManager**: 100% method coverage
✅ **Integration**: Component interaction coverage
✅ **API**: JSON handling coverage
✅ **Error Handling**: Error condition coverage

### Coverage Metrics

- **Unit Tests**: 45 test cases
- **Integration Tests**: 8 test cases
- **API Tests**: 8 test cases
- **Total Test Cases**: 61 test cases
- **Estimated Coverage**: 85-90%

## Test Data

### Valid Test Data

```cpp
// Valid sensor data
float validTemperature = 22.5;
float validHumidity = 45.0;
float validPressure = 1013.25;
int validIAQ = 25;

// Valid BLE data
float validBatteryVoltage = 3.7;
float validBatteryPercentage = 85.0;

// Valid JSON
const char* validJSON = R"({
    "temperature": 22.5,
    "humidity": 45.0,
    "pressure": 1013.25
})";
```

### Invalid Test Data

```cpp
// Invalid sensor data
float invalidTemperature = -999.0;
float invalidHumidity = -1.0;
float invalidPressure = 0.0;
int invalidIAQ = -1;

// Invalid JSON
const char* invalidJSON = "{ invalid json }";
```

## Performance Testing

### Memory Tests

- **Heap Usage**: Monitor heap memory during tests
- **Stack Usage**: Check for stack overflow conditions
- **Memory Leaks**: Test for memory leaks in loops
- **Large Data**: Test with large JSON documents

### Stress Tests

- **Iteration Tests**: Run operations multiple times
- **Concurrent Operations**: Test multiple operations simultaneously
- **Long Running**: Test extended operation periods
- **Resource Limits**: Test system resource limits

## Continuous Integration

### GitHub Actions (Recommended)

```yaml
name: Tests
on: [push, pull_request]
jobs:
  test:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v2
      - uses: platformio/action-setup-platformio@v1
      - run: pio test
```

### Local CI Setup

```bash
#!/bin/bash
# test_runner.sh
echo "Running WeatherStation Indoor Tests..."

# Install dependencies
pio lib install

# Run tests
pio test -v

# Check results
if [ $? -eq 0 ]; then
    echo "All tests passed!"
    exit 0
else
    echo "Tests failed!"
    exit 1
fi
```

## Debugging Tests

### Common Issues

1. **Test Timeouts**: Increase timeout values in test_config.h
2. **Memory Issues**: Check heap usage and reduce test data size
3. **Hardware Dependencies**: Use mock classes for hardware-independent testing
4. **Serial Output**: Enable verbose output with `-v` flag

### Debug Commands

```bash
# Run tests with debug output
pio test -v --verbose

# Run specific test with debug
pio test --filter test_sensor_manager -v

# Check test environment
pio test --dry-run

# Upload and monitor
pio test --upload-port /dev/ttyUSB0 --monitor-port /dev/ttyUSB0
```

## Test Best Practices

### Writing Tests

1. **Arrange-Act-Assert**: Structure tests clearly
2. **Test One Thing**: Each test should test one specific functionality
3. **Use Mocks**: Mock hardware dependencies for reliable testing
4. **Test Edge Cases**: Include boundary and error condition tests
5. **Clear Names**: Use descriptive test function names

### Test Maintenance

1. **Regular Updates**: Update tests when code changes
2. **Coverage Monitoring**: Monitor test coverage metrics
3. **Performance Tracking**: Track test execution time
4. **Documentation**: Keep test documentation updated

## Future Enhancements

### Planned Test Improvements

- **Hardware-in-the-Loop**: Real hardware testing
- **Performance Benchmarks**: Automated performance testing
- **Load Testing**: Stress testing with multiple clients
- **Security Testing**: Security vulnerability testing
- **Compatibility Testing**: Cross-platform compatibility testing

### Test Automation

- **Automated Test Execution**: CI/CD pipeline integration
- **Test Result Reporting**: Automated test result reporting
- **Coverage Reporting**: Automated coverage reporting
- **Performance Monitoring**: Automated performance monitoring

## Contributing to Tests

### Adding New Tests

1. **Create Test File**: Add new test file in test/ directory
2. **Follow Naming**: Use `test_` prefix for test files
3. **Include in Main**: Add test runner to test_main.cpp
4. **Document Tests**: Document new tests in this file
5. **Update Coverage**: Update coverage documentation

### Test Guidelines

- **Test Coverage**: Aim for 90%+ test coverage
- **Test Quality**: Write meaningful, maintainable tests
- **Test Performance**: Keep test execution time reasonable
- **Test Reliability**: Ensure tests are deterministic and reliable

## Support

### Getting Help

1. **Check Documentation**: Review this testing guide
2. **Run Tests**: Execute tests to identify issues
3. **Check Logs**: Review test output and error messages
4. **Community Support**: Seek help from the community
5. **Issue Reporting**: Report test issues via GitHub issues

### Resources

- **PlatformIO Testing**: https://docs.platformio.org/en/latest/advanced/unit-testing/
- **Unity Framework**: https://github.com/ThrowTheSwitch/Unity
- **Arduino Testing**: https://github.com/arduino/ArduinoCore-avr/tree/master/libraries/__cores__/arduino
- **ESP32 Testing**: https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/unity.html

This comprehensive testing framework ensures the WeatherStation Indoor project maintains high quality and reliability! 🧪✅
