# Contributing to WeatherStation Indoor

Thank you for your interest in contributing to the WeatherStation Indoor project!

## How to Contribute

### Reporting Issues
- Use clear, descriptive titles
- Include steps to reproduce the problem
- Provide hardware setup information
- Include serial output and error messages

### Suggesting Features
- Describe the use case and benefits
- Consider impact on existing functionality
- Explain how it fits into the project scope

### Code Contributions
- Fork the repository
- Create a feature branch
- Make your changes
- Test thoroughly on actual hardware
- Submit a pull request

## Development Setup

### Prerequisites
- PlatformIO IDE or PlatformIO Core
- ESP32 development board
- GY-MCU680 sensor
- LilyGo T-Display (optional)

### Local Development
1. **Fork and clone** the repository
2. **Install dependencies**: `pio lib install`
3. **Configure hardware**:
   - GY-MCU680 sensor: pins 25 (RX) and 27 (TX)
   - T-Display: standard SPI connections
4. **Build and upload**: `pio run -t upload`

### Testing Requirements
- [ ] Code compiles without warnings
- [ ] GY-MCU680 sensor readings are accurate
- [ ] WiFi connection works properly
- [ ] Web interface is accessible
- [ ] BLE connection to outdoor station succeeds
- [ ] T-Display shows data correctly (if used)

## Code Style Guidelines

### C++ Style
- Follow Google C++ Style Guide
- Use meaningful variable and function names
- Add comments for complex logic
- Keep functions focused and small

### Naming Conventions
- **Classes**: PascalCase (e.g., `SensorManager`)
- **Functions**: camelCase (e.g., `getSensorData()`)
- **Constants**: UPPER_SNAKE_CASE (e.g., `SERIAL_BAUD_RATE`)
- **Files**: snake_case (e.g., `sensor_manager.h`)

## Hardware Testing

### Required Hardware
- ESP32 with WiFi and BLE support
- GY-MCU680 environmental sensor
- LilyGo T-Display (optional)
- Outdoor WeatherStation for BLE testing

### Testing Checklist
- [ ] Serial communication with GY-MCU680
- [ ] T-Display rendering (if used)
- [ ] WiFi connection and web interface
- [ ] BLE scanning and connection
- [ ] Data transmission and API responses

### Testing Commands
```bash
pio run          # Build project
pio run -t upload # Upload to device
pio device monitor # Monitor serial output
```

## Pull Request Process

### Before Submitting
1. **Test thoroughly** on actual hardware
2. **Update documentation** if needed
3. **Follow code style** guidelines
4. **Add tests** for new functionality

### Pull Request Template
- **Description**: What does this PR do?
- **Hardware tested**: What hardware was used for testing?
- **Testing**: What tests were performed?

## Community Guidelines

- Be respectful and constructive
- Use clear, professional language
- Test changes before submitting
- Help other contributors

Thank you for contributing to WeatherStation Indoor! 🚀
