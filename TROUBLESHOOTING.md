# Troubleshooting Guide - WeatherStation Indoor

Comprehensive diagnostic guide for resolving common issues with the indoor weather station.

## Quick Diagnostic Steps

### 1. Serial Monitor Output
Connect to the device via serial monitor (115200 baud) and check for:
- Boot messages and initialization sequence
- WiFi connection status and IP assignment
- GY-MCU680 sensor reading errors or timeouts
- BLE connection attempts and outdoor station discovery
- Memory usage and system status

### 2. Web Interface Access
- **URL**: `http://weatherstation.local` or `http://[IP_ADDRESS]`
- **API**: `http://weatherstation.local/api/status`
- **Status Page**: Displays connection details, sensor data, and system health

### 3. LED Indicators
- **Power LED**: Should be illuminated when powered
- **WiFi LED**: Should blink during connection attempts, steady when connected

## Common Issues and Solutions

### WiFi Connection Problems

#### Cannot Connect to WiFi
**Symptoms**: Device remains in AP mode or connection attempts fail repeatedly

**Solutions**:
1. **Check Credentials**: Verify WiFi SSID and password in `src/secrets.h`
2. **Network Compatibility**: Ensure WiFi network is 2.4GHz (ESP32 doesn't support 5GHz)
3. **Signal Strength**: Move device closer to WiFi router or use WiFi extender
4. **Network Security**: Verify network uses WPA2 or WPA3 (avoid WEP)
5. **Reset WiFi**: Power cycle device or use reset button to clear stored credentials

#### Device Not Accessible on Network
**Symptoms**: Cannot access web interface or API endpoints

**Solutions**:
1. **Check IP Address**: Use serial monitor to see assigned IP address
2. **Network Access**: Ensure device and computer are on same network segment
3. **Firewall Issues**: Check if firewall is blocking connections to device
4. **Hostname Resolution**: Try IP address instead of hostname if DNS fails
5. **Router Settings**: Verify router allows device-to-device communication

### Sensor Issues

#### GY-MCU680 Not Working
**Symptoms**: No sensor readings, "Could not find sensor" messages, or invalid data

**Solutions**:
1. **Check I2C Wiring**: Verify SDA (GPIO 21) and SCL (GPIO 22) connections
2. **Power Supply**: Ensure stable 3.3V supply to sensor with adequate current
3. **I2C Address**: Verify sensor is detected at address 0x76 or 0x77
4. **Wire Quality**: Use high-quality jumper wires for reliable connections
5. **Sensor Placement**: Ensure sensor is not near heat sources or direct sunlight

#### Incorrect Sensor Readings
**Symptoms**: Unreasonable temperature, humidity, or pressure values

**Solutions**:
1. **Sensor Placement**: Position sensor away from heat sources, vents, or direct sunlight
2. **Calibration Period**: Allow sensor to stabilize for 15-30 minutes after power-up
3. **Environmental Factors**: Check for drafts, humidity sources, or temperature gradients
4. **Hardware Issues**: Verify sensor is not damaged or contaminated
5. **Reference Comparison**: Compare readings with known accurate thermometer

### Display Issues

#### T-Display Not Working
**Symptoms**: No display output, blank screen, or garbled content

**Solutions**:
1. **SPI Connections**: Verify all SPI pins (MOSI, SCLK, CS, DC, RST) are connected correctly
2. **Backlight Control**: Check if backlight pin (GPIO 4) is connected and powered
3. **Power Supply**: Ensure stable 3.3V supply to display with adequate current
4. **Library Configuration**: Verify TFT_eSPI library is properly configured for T-Display
5. **Reset Signal**: Ensure display reset pin is properly connected and functional

#### Display Shows Garbage or Corrupted Data
**Symptoms**: Display shows random characters, corrupted graphics, or incorrect colors

**Solutions**:
1. **SPI Configuration**: Check TFT_eSPI library configuration in platformio.ini
2. **Pin Definitions**: Verify correct pin definitions match your hardware setup
3. **Display Rotation**: Try different display rotation settings (0, 1, 2, 3)
4. **SPI Frequency**: Reduce SPI frequency if display is unstable
5. **Reset Display**: Power cycle the display or use hardware reset

### BLE Connection Issues

#### No Outdoor Data
**Symptoms**: Outdoor sensor data shows as 0, null, or missing entirely

**Solutions**:
1. **Check Outdoor Station**: Ensure outdoor station is powered and operational
2. **BLE Range**: Reduce distance between stations (maximum 10-20 meters)
3. **Interference**: Remove obstacles and check for BLE interference sources
4. **UUID Verification**: Ensure BLE UUIDs match between indoor and outdoor stations
5. **Power Management**: Check if outdoor station has adequate battery power

#### BLE Connection Fails or Times Out
**Symptoms**: "Failed to connect" messages, timeouts, or intermittent connections

**Solutions**:
1. **Power Cycle**: Restart both indoor and outdoor stations completely
2. **BLE Settings**: Check BLE power levels and scan parameters
3. **Distance**: Move stations closer together for better signal strength
4. **Reset BLE**: Clear BLE cache and restart BLE services
5. **Environmental Factors**: Check for metal objects or walls blocking signal

### Web Interface Issues

#### Page Not Loading
**Symptoms**: Web interface shows error, doesn't load, or displays blank page

**Solutions**:
1. **Browser Cache**: Clear browser cache, cookies, and local storage
2. **Different Browser**: Try different web browser or incognito mode
3. **Network Issues**: Check network connectivity and DNS resolution
4. **Device Reset**: Power cycle the ESP32 to restart web server
5. **Memory Issues**: Check for memory leaks or insufficient heap memory

#### API Not Responding
**Symptoms**: `/api/status` returns error, timeout, or invalid JSON

**Solutions**:
1. **Check URL**: Verify correct API endpoint and port
2. **JSON Format**: Validate response is proper JSON format
3. **Device Status**: Ensure device is running and not crashed
4. **Memory Issues**: Check for memory leaks or stack overflow
5. **Network Load**: Reduce concurrent connections if device is overloaded

## Diagnostic Tools

### Serial Communication Test
Use this code to test GY-MCU680 I2C communication:

```cpp
#include <Wire.h>

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22); // SDA=21, SCL=22
  
  Serial.println("GY-MCU680 I2C Test");
  Serial.println("Scanning for devices...");
  
  for (byte addr = 1; addr < 127; addr++) {
    Wire.beginTransmission(addr);
    byte error = Wire.endTransmission();
    if (error == 0) {
      Serial.printf("Device found at address 0x%02X\n", addr);
    }
  }
  Serial.println("Scan complete.");
}

void loop() {
  delay(1000);
}
```

### WiFi Scanner and Diagnostics
Check available WiFi networks and signal strength:

```cpp
#include <WiFi.h>

void setup() {
  Serial.begin(115200);
  
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  
  Serial.println("WiFi Scanner - WeatherStation Indoor");
  int n = WiFi.scanNetworks();
  
  if (n == 0) {
    Serial.println("No networks found");
  } else {
    Serial.printf("%d networks found:\n", n);
    for (int i = 0; i < n; ++i) {
      Serial.printf("%d: %s (%d dBm) %s\n", 
        i+1, 
        WiFi.SSID(i).c_str(), 
        WiFi.RSSI(i),
        WiFi.encryptionType(i) == WIFI_AUTH_OPEN ? "Open" : "Secured"
      );
    }
  }
}

void loop() {
  delay(1000);
}
```

### T-Display Test and Calibration
Test display functionality and verify connections:

```cpp
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

void setup() {
  Serial.begin(115200);
  
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  
  tft.drawString("WeatherStation", 10, 10);
  tft.drawString("Display Test", 10, 40);
  tft.drawString("Working!", 10, 70);
  
  // Test colors
  tft.fillRect(10, 100, 50, 30, TFT_RED);
  tft.fillRect(70, 100, 50, 30, TFT_GREEN);
  tft.fillRect(130, 100, 50, 30, TFT_BLUE);
  
  Serial.println("Display test complete");
}

void loop() {
  delay(1000);
}
```

### BLE Scanner
Test BLE functionality and discover nearby devices:

```cpp
#include <NimBLEDevice.h>

void setup() {
  Serial.begin(115200);
  
  NimBLEDevice::init("WeatherStation_Scanner");
  NimBLEScan* pBLEScan = NimBLEDevice::getScan();
  
  Serial.println("BLE Scanner - WeatherStation Indoor");
  Serial.println("Scanning for devices...");
  
  NimBLEScanResults results = pBLEScan->start(10);
  
  for (int i = 0; i < results.getCount(); i++) {
    NimBLEAdvertisedDevice device = results.getDevice(i);
    Serial.printf("Device: %s\n", device.toString().c_str());
  }
  
  Serial.println("Scan complete.");
}

void loop() {
  delay(1000);
}
```

## Error Messages Reference

| Error Message | Cause | Solution |
|---------------|-------|----------|
| "Could not find GY-MCU680" | I2C communication failure | Check SDA/SCL wiring and power supply |
| "WiFi connection failed" | Network credentials or signal issues | Verify WiFi settings and signal strength |
| "BLE connection timeout" | Outdoor station not found or out of range | Check outdoor station power and proximity |
| "Web server error" | Memory or configuration issue | Restart device and check memory usage |
| "Sensor reading failed" | I2C communication or sensor hardware issue | Verify connections and sensor power |
| "Display initialization failed" | SPI connection or library configuration issue | Check SPI wiring and library setup |

## Performance Optimization

### Memory Management
- **Heap Memory**: Monitor available heap memory via serial monitor
- **Stack Usage**: Check for stack overflow issues in complex operations
- **String Handling**: Use String objects sparingly, prefer char arrays
- **Dynamic Allocation**: Minimize dynamic memory allocation in loops

### Network Performance
- **WiFi Signal**: Ensure strong WiFi signal (-50 dBm or better)
- **Network Load**: Avoid heavy concurrent network traffic
- **DNS Resolution**: Use IP address instead of hostname for faster access
- **Connection Pooling**: Reuse connections when possible

### Power Management
- **USB Power**: Use adequate USB power supply (2A or higher)
- **Voltage Stability**: Ensure stable 3.3V supply for sensors
- **Sleep Modes**: Consider implementing sleep modes for battery operation
- **Component Power**: Monitor power consumption of all components

## Getting Help

If you're still experiencing issues after trying the solutions above:

1. **Check Documentation**: Review README and hardware setup guide thoroughly
2. **Serial Output**: Provide complete serial monitor output with timestamps
3. **Hardware Setup**: Verify all connections and component compatibility
4. **Network Environment**: Document your network setup and configuration
5. **Create Issue**: Include detailed problem description, steps to reproduce, and system information

## Maintenance

### Regular Checks
- **Monthly**: Verify WiFi connection stability and sensor reading accuracy
- **Quarterly**: Clean sensor components and check all connections
- **Annually**: Update firmware and verify all system components

### Software Updates
- **Monitor Releases**: Check for firmware updates and bug fixes
- **Test Updates**: Verify updates work with your specific hardware setup
- **Backup Configuration**: Keep working firmware version as backup
- **Documentation**: Review updated documentation for new features

### Hardware Maintenance
- **Connection Inspection**: Check for loose or corroded connections
- **Component Cleaning**: Clean sensors and display as needed
- **Power Supply**: Verify power supply stability and capacity
- **Environmental Protection**: Ensure adequate protection from moisture and dust
