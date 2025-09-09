# WeatherStation Indoor 🌡️

[![PlatformIO](https://img.shields.io/badge/PlatformIO-ESP32-blue.svg)](https://platformio.org/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Arduino](https://img.shields.io/badge/Arduino-Compatible-green.svg)](https://www.arduino.cc/)

A comprehensive indoor weather monitoring system featuring web interface, BLE connectivity, and seamless Home Assistant integration. Built for ESP32 with modular architecture and extensive testing framework.

## Table of Contents

- [Features](#features)
- [Hardware Requirements](#hardware-requirements)
- [Quick Start](#quick-start)
- [Configuration](#configuration)
- [Home Assistant Integration](#home-assistant-integration)
- [Pin Connections](#pin-connections)
- [Sensors](#sensors)
- [Web Interface](#web-interface)
- [Architecture](#architecture)
- [Troubleshooting](#troubleshooting)
- [Development](#development)
- [License](#license)
- [Contributing](#contributing)
- [Changelog](#changelog)

## Project Status

![Build Status](https://img.shields.io/badge/Build-Passing-brightgreen)
![Test Coverage](https://img.shields.io/badge/Test%20Coverage-75--80%25-orange)
![Version](https://img.shields.io/badge/Version-1.1.0-blue)

**Current Status**: Production Ready ✅  
**Test Coverage**: 75-80% (Target: 85-90%)  
**Documentation**: Complete  
**CI/CD**: Configured  

## Features

- **Environmental Monitoring**: Real-time temperature, humidity, pressure, and air quality (IAQ) using GY-MCU680 sensor
- **Outdoor Integration**: BLE communication with outdoor weather station for complete environmental data
- **Web Interface**: Modern, responsive web UI with JSON API powered by IoT-WebUI
- **Local Display**: T-Display integration for immediate sensor data visualization
- **Home Assistant**: Full REST API integration with comprehensive sensor support
- **WiFi Management**: Intuitive WiFi configuration portal with fallback modes
- **Time Synchronization**: Automatic timezone-aware time synchronization

## Hardware Requirements

- **ESP32 Development Board** (any ESP32 with WiFi/BLE capabilities)
- **GY-MCU680 Environmental Sensor** (temperature, humidity, pressure, IAQ)
- **LilyGo T-Display** (optional - for local data display)
- **Power Supply**: USB or 5V power source
- **Breadboard/Jumper Wires**: For sensor connections

## Quick Start

1. **Clone and configure**:
   ```bash
   git clone https://github.com/yourusername/WeatherStation_indoor.git
   cd WeatherStation_indoor
   cp src/secrets.h.template src/secrets.h
   # Edit src/secrets.h with your WiFi credentials and custom settings
   ```

2. **Connect hardware** (see [HARDWARE_SETUP.md](HARDWARE_SETUP.md)):
   - GY-MCU680 sensor: I2C connections to GPIO 21 (SDA) and GPIO 22 (SCL)
   - T-Display: Standard SPI connections as configured in platformio.ini

3. **Build and upload**:
   ```bash
   pio run -t upload
   ```

4. **Access the device**:
   - Web Interface: `http://weatherstation.local` or `http://[IP_ADDRESS]`
   - API Endpoint: `http://weatherstation.local/api/status`

## Configuration

### Configuration System

The project uses a two-tier configuration system:

- **`src/config.h`**: Default configuration values (hardware pins, timeouts, etc.)
- **`src/secrets.h`**: User-specific settings (WiFi credentials, timezone, etc.)

#### Creating Your Configuration

1. **Copy the template**:
   ```bash
   cp src/secrets.h.template src/secrets.h
   ```

2. **Edit `src/secrets.h`** with your settings:
   ```cpp
   // WiFi Configuration
   #define WIFI_HOSTNAME "myweatherstation"
   #define WIFI_AP_SSID "MyWeatherStation"
   #define WIFI_AP_PASSWORD "mysecretpassword"
   
   // Timezone Configuration
   #define TIMEZONE_LOCATION "America/New_York"
   
   // Hardware Configuration (if needed)
   #define GY_RXD_PIN 25  // Use different GPIO pin if needed
   #define GY_TXD_PIN 27  // Use different GPIO pin if needed
   ```

3. **Build and upload** - your custom settings will override the defaults

> **Security Note**: The `secrets.h` file is automatically ignored by git to protect your sensitive information. Never commit this file to version control.

### WiFi Setup
- **First Boot**: Creates WiFi portal at `WeatherStation_AP`
- **Configuration**: Connect to portal and enter WiFi credentials
- **Fallback**: Device automatically reverts to AP mode if connection fails
- **Reconfiguration**: Access `/config` endpoint to modify settings

### BLE Integration
- **Service UUID**: `4fafc201-1fb5-459e-8fcc-c5c9c331914b`
- **Characteristic UUID**: `beb5483e-36e1-4688-b7f5-ea07361b26a8`
- **Data Format**: 20-byte binary packet (temperature, humidity, pressure, battery voltage, battery percentage)
- **Range**: Up to 10-20 meters depending on environmental conditions

### API Response Format
```json
{
  "timestamp": 1234567890,
  "status": "running",
  "indoor": {
    "temperature": 22.5,
    "humidity": 45.2,
    "pressure": 1013.25,
    "iaq": 25,
    "iaq_accuracy": 3,
    "gas": 12345,
    "altitude": 100.5
  },
  "outdoor": {
    "temperature": 18.3,
    "humidity": 65.1,
    "pressure": 1012.8,
    "battery_voltage": 3.85,
    "battery_percentage": 85.2
  },
  "time": {
    "current": "14:30:25",
    "date": "2024-01-15",
    "datetime": "2024-01-15T14:30:25"
  },
  "wifi": {
    "connected": true,
    "ip": "192.168.1.100",
    "rssi": -45
  }
}
```

## Home Assistant Integration

See [HOME_ASSISTANT_INTEGRATION.md](HOME_ASSISTANT_INTEGRATION.md) for complete setup instructions.

### Quick Configuration
```yaml
rest:
  resource: http://weatherstation.local/api/status
  sensor:
    - name: "Living Room Temperature"
      value_template: "{{ value_json['indoor']['temperature'] | round(1) }}"
      device_class: temperature
      unit_of_measurement: "°C"
```

## Pin Connections

| Component | ESP32 Pin | Description |
|-----------|-----------|-------------|
| **GY-MCU680 SDA** | GPIO 21 | I2C data line |
| **GY-MCU680 SCL** | GPIO 22 | I2C clock line |
| **T-Display MOSI** | GPIO 19 | SPI data line |
| **T-Display SCLK** | GPIO 18 | SPI clock line |
| **T-Display CS** | GPIO 5 | SPI chip select |
| **T-Display DC** | GPIO 16 | Display data/command |
| **T-Display RST** | GPIO 23 | Display reset |
| **T-Display BL** | GPIO 4 | Display backlight |

## Sensors

### Indoor Sensor (GY-MCU680)
- **Temperature**: -40°C to +85°C (±0.3°C accuracy)
- **Humidity**: 0-100% RH (±2% accuracy)
- **Pressure**: 300-1100 hPa (±1 hPa accuracy)
- **IAQ**: Indoor Air Quality Index (0-500 scale)
- **Gas**: VOC gas resistance (kΩ)
- **Altitude**: Calculated altitude based on atmospheric pressure

### Outdoor Sensor (via BLE)
- **Temperature**: Real-time outdoor temperature
- **Humidity**: Outdoor relative humidity
- **Pressure**: Atmospheric pressure
- **Battery**: Voltage and percentage monitoring

## Web Interface

### Features
- Real-time sensor data with automatic refresh
- Responsive design optimized for mobile and desktop
- Configuration panel for runtime settings adjustment
- System status monitoring and diagnostics
- JSON API for programmatic data access

### API Endpoints
- `GET /api/status` - Get current sensor readings and system status
- `GET /config` - Access configuration interface
- `GET /reset` - Reset device or WiFi settings

## Architecture

### System Overview
```
┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│   GY-MCU680     │    │   ESP32 Board   │    │  Outdoor Station│
│   Sensor        │◄──►│   (Indoor)      │◄──►│   (BLE Client)  │
│   (I2C)         │    │                 │    │                 │
└─────────────────┘    └─────────────────┘    └─────────────────┘
                              │
                              ▼
                    ┌─────────────────┐
                    │   T-Display     │
                    │   (SPI)         │
                    └─────────────────┘
                              │
                              ▼
                    ┌─────────────────┐
                    │  Web Interface  │
                    │  (WiFi)         │
                    └─────────────────┘
                              │
                              ▼
                    ┌─────────────────┐
                    │ Home Assistant  │
                    │ (REST API)      │
                    └─────────────────┘
```

### Modular Design
The project employs a modular architecture with specialized manager classes:

- **SensorManager**: GY-MCU680 sensor communication and data processing
- **DisplayManager**: T-Display rendering and user interface management
- **BLEManager**: BLE server for outdoor sensor communication
- **WiFiManager**: WiFi connection management and configuration portal
- **TimeManager**: Time synchronization and formatting utilities
- **IoTWebUIManager**: Web interface and API management

### Project Structure
```
src/
├── main.cpp              # Main application logic and initialization
├── config.h              # Configuration constants and pin definitions
├── secrets.h             # WiFi credentials (not included in repository)
├── sensor_manager.cpp    # GY-MCU680 sensor interface and data processing
├── display_manager.cpp   # T-Display interface and UI rendering
├── ble_manager.cpp       # BLE server for outdoor sensor communication
├── wifi_manager.cpp      # WiFi connection and configuration management
└── time_manager.cpp      # Time synchronization and formatting
```

## Troubleshooting

### Common Issues
- **WiFi Connection**: Verify credentials in `src/secrets.h` and check network compatibility
- **Sensor Readings**: Confirm I2C connections and power supply stability
- **BLE Issues**: Ensure outdoor station is powered and within range
- **Web Interface**: Verify device accessibility via IP address or hostname

### Debug Information
- **Serial Monitor**: 115200 baud for comprehensive debug output
- **Web Interface**: Status page displays connection details and sensor data
- **API Endpoint**: `/api/status` provides real-time system status

## Development

### Building
```bash
pio run          # Build project
pio run -t upload # Upload to device
pio device monitor # Monitor serial output
```

### Testing

The project includes a comprehensive testing framework with unit tests, integration tests, and API tests. Built with Unity testing framework and optimized for ESP32 development.

#### Testing Framework Features
- ✅ **Unity Test Framework**: Industry-standard C testing framework
- ✅ **Mock Hardware**: Hardware-independent testing with mock classes
- ✅ **CI/CD Integration**: GitHub Actions workflow for automated testing
- ✅ **PlatformIO Integration**: Native PlatformIO test environment
- ✅ **NimBLE Optimized**: Reduced memory footprint for testing
- ✅ **Comprehensive Coverage**: 75-80% current coverage with roadmap to 85-90%

#### Quick Test Commands
```bash
# Run all tests
./run_tests.sh all

# Run specific test
./run_tests.sh specific test_sensor_manager

# Run tests with coverage
./run_tests.sh coverage

# Run tests on hardware
./run_tests.sh hardware /dev/ttyUSB0
```

#### Manual Testing
- Verify I2C device detection at addresses 0x76/0x77
- Test WiFi connectivity and web interface accessibility
- Confirm BLE communication with outdoor station
- Validate sensor data accuracy and stability

#### Test Coverage
- **Unit Tests**: 20 test cases covering basic functionality and data validation
- **Integration Tests**: Component interaction testing with mock hardware
- **API Tests**: JSON parsing, generation, and error handling
- **Mock Tests**: Hardware-independent testing framework
- **Current Coverage**: 75-80% (Target: 85-90%)

See [TESTING.md](TESTING.md) for complete testing documentation.

## Key Achievements

- 🏆 **Production Ready**: Stable, tested, and documented system
- 🧪 **Comprehensive Testing**: 75-80% test coverage with professional testing framework
- 📚 **Complete Documentation**: Detailed setup guides, troubleshooting, and API documentation
- 🔧 **Modular Architecture**: Clean, maintainable code with separation of concerns
- 🌐 **Home Assistant Ready**: Seamless integration with popular home automation platform
- 📱 **Responsive Web UI**: Modern interface optimized for all devices
- 🔄 **CI/CD Pipeline**: Automated testing and deployment workflow
- 💾 **Memory Optimized**: NimBLE optimizations for efficient resource usage

## License

This project is licensed under the MIT License - see [LICENSE](LICENSE) for details.

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md) for contribution guidelines and development setup.

## Changelog

See [CHANGELOG.md](CHANGELOG.md) for version history and updates.