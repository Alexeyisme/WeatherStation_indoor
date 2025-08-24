# Hardware Setup - WeatherStation Indoor

Complete hardware assembly guide for the indoor weather monitoring station with detailed connection instructions.

## Required Components

- **ESP32 Development Board** (any ESP32 with WiFi/BLE capabilities)
- **GY-MCU680 Environmental Sensor** (temperature, humidity, pressure, IAQ)
- **LilyGo T-Display** (optional - for local data display)
- **USB Cable** (for power and programming)
- **Breadboard/Jumper Wires** (for secure connections)
- **3.3V Power Supply** (if not using USB power)

## Pin Connections

### I2C Bus (Shared)
Both GY-MCU680 and T-Display share the same I2C bus for efficient communication:

| Component | ESP32 Pin | Description |
|-----------|-----------|-------------|
| **GY-MCU680 SDA** | GPIO 21 | I2C data line |
| **GY-MCU680 SCL** | GPIO 22 | I2C clock line |
| **T-Display SDA** | GPIO 21 | I2C data line (shared) |
| **T-Display SCL** | GPIO 22 | I2C clock line (shared) |

### T-Display SPI Connections
The T-Display uses SPI for high-speed graphics rendering:

| Component | ESP32 Pin | Description |
|-----------|-----------|-------------|
| **T-Display MOSI** | GPIO 19 | SPI data line |
| **T-Display SCLK** | GPIO 18 | SPI clock line |
| **T-Display CS** | GPIO 5 | SPI chip select |
| **T-Display DC** | GPIO 16 | Display data/command control |
| **T-Display RST** | GPIO 23 | Display reset signal |
| **T-Display BL** | GPIO 4 | Display backlight control |

### Power Connections
| Component | Connection | Description |
|-----------|------------|-------------|
| **ESP32 VIN** | USB 5V | Main power input |
| **ESP32 GND** | USB GND | Ground connection |
| **GY-MCU680 VCC** | ESP32 3.3V | Sensor power supply |
| **GY-MCU680 GND** | ESP32 GND | Sensor ground |
| **T-Display VCC** | ESP32 3.3V | Display power supply |
| **T-Display GND** | ESP32 GND | Display ground |

## Assembly Steps

### 1. Prepare ESP32
- Ensure your ESP32 board has I2C and SPI support
- Verify pin assignments for GPIO 21, 22 (I2C) and GPIO 18, 19, 5, 16, 23, 4 (SPI)
- Confirm the board can operate on 5V USB power
- Check that all required pins are accessible and not used by other functions

### 2. Connect GY-MCU680 Sensor
1. **SDA Connection**: Connect GY-MCU680 SDA to ESP32 GPIO 21
2. **SCL Connection**: Connect GY-MCU680 SCL to ESP32 GPIO 22
3. **Power Connection**: Connect GY-MCU680 VCC to ESP32 3.3V
4. **Ground Connection**: Connect GY-MCU680 GND to ESP32 GND
5. **Verify Connections**: Ensure all connections are secure and properly seated

### 3. Connect T-Display (Optional)
1. **I2C Connections**: Connect T-Display SDA to ESP32 GPIO 21 (shared with sensor)
2. **I2C Clock**: Connect T-Display SCL to ESP32 GPIO 22 (shared with sensor)
3. **SPI Data**: Connect T-Display MOSI to ESP32 GPIO 19
4. **SPI Clock**: Connect T-Display SCLK to ESP32 GPIO 18
5. **Chip Select**: Connect T-Display CS to ESP32 GPIO 5
6. **Data/Command**: Connect T-Display DC to ESP32 GPIO 16
7. **Reset**: Connect T-Display RST to ESP32 GPIO 23
8. **Backlight**: Connect T-Display BL to ESP32 GPIO 4
9. **Power Connection**: Connect T-Display VCC to ESP32 3.3V
10. **Ground Connection**: Connect T-Display GND to ESP32 GND

### 4. Power Supply Setup
1. **USB Connection**: Connect USB cable to ESP32 for power and programming
2. **Power Verification**: Check that power LED is illuminated
3. **Voltage Check**: Verify 3.3V is available at power pins
4. **Secure Connections**: Ensure all connections are mechanically secure

## Testing

### I2C Device Verification
Upload this I2C scanner to verify connections:

```cpp
#include <Wire.h>

void setup() {
  Wire.begin(21, 22);  // SDA=21, SCL=22
  Serial.begin(115200);
  
  Serial.println("I2C Scanner - WeatherStation Indoor");
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

### Expected Results
- **GY-MCU680**: Should appear at address 0x76 or 0x77
- **T-Display**: Should appear at address 0x3C (if I2C is used for configuration)

### T-Display Test
Test display functionality with this basic sketch:

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
  
  Serial.println("Display test complete");
}

void loop() {
  delay(1000);
}
```

## Troubleshooting

### Common Issues
- **No I2C Devices**: Check SDA/SCL connections, power supply, and wire quality
- **Wrong Address**: Some sensors use different I2C addresses; check sensor documentation
- **Power Issues**: Ensure stable 3.3V supply with adequate current capacity
- **Loose Connections**: Verify all wires are securely connected and properly seated
- **Display Issues**: Check SPI connections and backlight control

### Testing Checklist
- [ ] I2C devices detected at correct addresses
- [ ] GY-MCU680 sensor readings are within reasonable ranges
- [ ] T-Display shows boot messages and test patterns
- [ ] WiFi connection establishes successfully
- [ ] Web interface is accessible via browser
- [ ] BLE communication with outdoor station works
- [ ] All sensor data appears in web interface

### Diagnostic Commands
```bash
# Build and upload test sketches
pio run -t upload

# Monitor serial output
pio device monitor

# Check device connectivity
ping weatherstation.local
```

## Safety Notes

- **Power Safety**: Use proper USB power supply rated for ESP32 requirements
- **Connection Safety**: Double-check all connections before powering on
- **Component Protection**: Handle sensors and display carefully to avoid damage
- **Static Protection**: Use anti-static precautions when handling components
- **Heat Management**: Ensure adequate ventilation for long-term operation

## Performance Optimization

### Connection Quality
- Use high-quality jumper wires for reliable connections
- Keep wire lengths as short as possible
- Avoid routing signal wires near power lines
- Use proper grounding for noise reduction

### Power Considerations
- Ensure stable 3.3V supply with adequate current capacity
- Consider using external power supply for long-term operation
- Monitor power consumption during operation
- Implement power-saving features if needed

## Maintenance

### Regular Checks
- **Monthly**: Verify all connections and sensor readings
- **Quarterly**: Clean components and check for corrosion
- **Annually**: Update firmware and verify all functionality

### Long-term Considerations
- **Component Aging**: Monitor sensor accuracy over time
- **Connection Wear**: Check for loose connections periodically
- **Environmental Factors**: Protect from moisture and extreme temperatures
