# WeatherStation Indoor 🌡️

A comprehensive indoor weather monitoring system featuring web interface, BLE connectivity, and seamless Home Assistant integration.

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
   git clone <repository>
   cd WeatherStation_indoor
   cp src/secrets.h.template src/secrets.h
   # Edit src/secrets.h with your WiFi credentials
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
- Verify I2C device detection at addresses 0x76/0x77
- Test WiFi connectivity and web interface accessibility
- Confirm BLE communication with outdoor station
- Validate sensor data accuracy and stability

## License

This project is licensed under the MIT License - see [LICENSE](LICENSE) for details.

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md) for contribution guidelines and development setup.

## Changelog

See [CHANGELOG.md](CHANGELOG.md) for version history and updates.