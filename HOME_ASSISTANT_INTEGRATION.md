# Home Assistant Integration Guide

## Overview

The WeatherStation Indoor provides a comprehensive REST API for seamless Home Assistant integration. This guide demonstrates how to set up sensors for temperature, humidity, pressure, air quality, and outdoor data with practical examples and troubleshooting tips.

## Prerequisites

- **WeatherStation Indoor**: Fully configured and operational
- **Home Assistant**: Installed and accessible on your network
- **Network Access**: Both devices connected to the same local network
- **Device Discovery**: WeatherStation accessible via `weatherstation.local` or IP address

## API Endpoint

The WeatherStation Indoor provides real-time sensor data via the REST API:

```
GET http://weatherstation.local/api/status
```

### Response Format

```json
{
  "timestamp": 1640995200000,
  "status": "running",
  "indoor": {
    "temperature": 22.5,
    "humidity": 45.2,
    "pressure": 1013.25,
    "iaq": 25,
    "iaq_accuracy": 3,
    "gas": 12345,
    "altitude": 100
  },
  "outdoor": {
    "temperature": 18.3,
    "humidity": 62.1,
    "pressure": 1012.8,
    "battery_voltage": 3.7,
    "battery_percentage": 85.5
  },
  "time": {
    "current": "14:30:25",
    "date": "2024-01-01",
    "datetime": "2024-01-01 14:30:25"
  },
  "wifi": {
    "connected": true,
    "ip": "192.168.1.100",
    "rssi": -45
  }
}
```

## Configuration

### Basic Sensor Setup

Add this configuration to your `configuration.yaml`:

```yaml
rest:
  resource: http://weatherstation.local/api/status
  sensor:
    # Indoor Environmental Sensors
    - name: "Living Room Temperature"
      value_template: "{{ value_json['indoor']['temperature'] | round(1) }}"
      device_class: temperature
      unit_of_measurement: "°C"
      state_class: measurement
      
    - name: "Living Room Humidity"
      value_template: "{{ value_json['indoor']['humidity'] | round(1) }}"
      device_class: humidity
      unit_of_measurement: "%"
      state_class: measurement
      
    - name: "Living Room Pressure"
      value_template: "{{ value_json['indoor']['pressure'] | round(1) }}"
      device_class: pressure
      unit_of_measurement: "hPa"
      state_class: measurement
      
    - name: "Living Room Air Quality"
      value_template: "{{ value_json['indoor']['iaq'] }}"
      device_class: aqi
      state_class: measurement
      
    # Outdoor Sensors (if BLE sensor is connected)
    - name: "Outdoor Temperature"
      value_template: "{{ value_json['outdoor']['temperature'] | round(1) }}"
      device_class: temperature
      unit_of_measurement: "°C"
      state_class: measurement
      
    - name: "Outdoor Humidity"
      value_template: "{{ value_json['outdoor']['humidity'] | round(1) }}"
      device_class: humidity
      unit_of_measurement: "%"
      state_class: measurement
      
    - name: "Outdoor Battery"
      value_template: "{{ value_json['outdoor']['battery_percentage'] }}"
      device_class: battery
      unit_of_measurement: "%"
      state_class: measurement
```

### Advanced Sensor Configuration

For comprehensive monitoring, add these additional sensors:

```yaml
    # Additional Indoor Sensors
    - name: "Living Room Gas Resistance"
      value_template: "{{ value_json['indoor']['gas'] }}"
      unit_of_measurement: "kΩ"
      state_class: measurement
      
    - name: "Living Room Altitude"
      value_template: "{{ value_json['indoor']['altitude'] }}"
      unit_of_measurement: "m"
      state_class: measurement
      
    - name: "Living Room IAQ Accuracy"
      value_template: "{{ value_json['indoor']['iaq_accuracy'] }}"
      state_class: measurement
      
    # Additional Outdoor Sensors
    - name: "Outdoor Pressure"
      value_template: "{{ value_json['outdoor']['pressure'] | round(1) }}"
      device_class: pressure
      unit_of_measurement: "hPa"
      state_class: measurement
      
    - name: "Outdoor Battery Voltage"
      value_template: "{{ value_json['outdoor']['battery_voltage'] | round(2) }}"
      device_class: voltage
      unit_of_measurement: "V"
      state_class: measurement
      
    # System Status Sensors
    - name: "WeatherStation Status"
      value_template: "{{ value_json['status'] }}"
      
    - name: "WeatherStation WiFi Signal"
      value_template: "{{ value_json['wifi']['rssi'] }}"
      unit_of_measurement: "dBm"
      state_class: measurement
```

## Installation Steps

1. **Add Configuration**: Edit your `configuration.yaml` file and add the REST sensor configuration above
2. **Validate Configuration**: Use Home Assistant's configuration validation feature
3. **Restart Home Assistant**: Navigate to Settings → System → Restart
4. **Verify Sensors**: Check Settings → Devices & Services → Entities for newly created sensors
5. **Test Functionality**: Monitor sensor values and ensure they update correctly

## Troubleshooting

### Common Issues

**"Entity not available" or "Unknown" state**
- Verify WeatherStation is operational: `http://weatherstation.local`
- Test API endpoint directly: `http://weatherstation.local/api/status`
- Check network connectivity between devices
- Try using IP address instead of hostname if DNS resolution fails

**Invalid sensor values or "None" states**
- Check the API response format for data consistency
- Verify sensor data is being read correctly on the WeatherStation
- Add error handling to templates for missing data
- Check for JSON parsing errors in Home Assistant logs

**Outdoor sensors showing "Unknown" or missing data**
- Ensure outdoor sensor is powered and within BLE range (10-20m)
- Check BLE connection status on WeatherStation web interface
- Verify outdoor sensor is actively sending data
- Restart both indoor and outdoor stations if needed

**Slow sensor updates or timeouts**
- Check network latency between Home Assistant and WeatherStation
- Verify WiFi signal strength and stability
- Consider reducing REST sensor scan intervals if needed
- Monitor Home Assistant logs for timeout errors

### Debug Configuration

Add this debug sensor to troubleshoot API issues:

```yaml
rest:
  resource: http://weatherstation.local/api/status
  sensor:
    - name: "WeatherStation Debug"
      value_template: "{{ value_json | tojson }}"
      scan_interval: 30
```

### Advanced Error Handling

Implement robust error handling for unreliable networks:

```yaml
rest:
  resource: http://weatherstation.local/api/status
  sensor:
    - name: "Living Room Temperature"
      value_template: >
        {% set temp = value_json['indoor']['temperature'] %}
        {% if temp is number and temp > -50 and temp < 100 %}
          {{ temp | round(1) }}
        {% else %}
          {{ none }}
        {% endif %}
      device_class: temperature
      unit_of_measurement: "°C"
      scan_interval: 60
```

## Example Automations

### High Temperature Alert
```yaml
automation:
  - alias: "High Temperature Alert"
    description: "Notify when indoor temperature exceeds 25°C"
    trigger:
      platform: numeric_state
      entity_id: sensor.living_room_temperature
      above: 25
    condition:
      - condition: time
        after: "08:00:00"
        before: "22:00:00"
    action:
      - service: notify.mobile_app
        data:
          title: "🌡️ High Temperature Alert"
          message: "Living room temperature is {{ states('sensor.living_room_temperature') }}°C"
```

### Poor Air Quality Notification
```yaml
automation:
  - alias: "Poor Air Quality Alert"
    description: "Notify when air quality deteriorates"
    trigger:
      platform: numeric_state
      entity_id: sensor.living_room_air_quality
      above: 100
    action:
      - service: notify.mobile_app
        data:
          title: "⚠️ Poor Air Quality"
          message: "Air quality index is {{ states('sensor.living_room_air_quality') }}"
```

### Outdoor Battery Low Warning
```yaml
automation:
  - alias: "Outdoor Battery Low"
    description: "Warn when outdoor sensor battery is low"
    trigger:
      platform: numeric_state
      entity_id: sensor.outdoor_battery
      below: 20
    action:
      - service: notify.mobile_app
        data:
          title: "🔋 Outdoor Sensor Battery Low"
          message: "Outdoor sensor battery is {{ states('sensor.outdoor_battery') }}%"
```

### Temperature Difference Monitoring
```yaml
automation:
  - alias: "Large Temperature Difference"
    description: "Alert when indoor/outdoor temperature difference is significant"
    trigger:
      platform: numeric_state
      entity_id: sensor.temperature_difference
      above: 15
    action:
      - service: notify.mobile_app
        data:
          title: "🌡️ Large Temperature Difference"
          message: "Indoor/outdoor difference is {{ states('sensor.temperature_difference') }}°C"
```

## Advanced Configuration

### Custom Templates

Create derived sensors for additional insights:

```yaml
template:
  - sensor:
      - name: "Temperature Difference"
        state: "{{ states('sensor.living_room_temperature') | float - states('sensor.outdoor_temperature') | float | round(1) }}"
        unit_of_measurement: "°C"
        
      - name: "Humidity Difference"
        state: "{{ states('sensor.living_room_humidity') | float - states('sensor.outdoor_humidity') | float | round(1) }}"
        unit_of_measurement: "%"
        
      - name: "Pressure Trend"
        state: >
          {% set current = states('sensor.living_room_pressure') | float %}
          {% set previous = states('sensor.living_room_pressure') | float %}
          {% if current > previous %}
            Rising
          {% elif current < previous %}
            Falling
          {% else %}
            Stable
          {% endif %}
```

### Data Validation and Filtering

Implement robust data validation:

```yaml
rest:
  resource: http://weatherstation.local/api/status
  sensor:
    - name: "Living Room Temperature Validated"
      value_template: >
        {% set temp = value_json['indoor']['temperature'] %}
        {% if temp is number and temp > -50 and temp < 100 %}
          {{ temp | round(1) }}
        {% else %}
          {{ none }}
        {% endif %}
      device_class: temperature
      unit_of_measurement: "°C"
      scan_interval: 60
```

### Custom Device Classes

Create custom device classes for specialized monitoring:

```yaml
template:
  - sensor:
      - name: "Air Quality Status"
        state: >
          {% set iaq = states('sensor.living_room_air_quality') | int %}
          {% if iaq <= 50 %}
            Excellent
          {% elif iaq <= 100 %}
            Good
          {% elif iaq <= 150 %}
            Moderate
          {% elif iaq <= 200 %}
            Poor
          {% else %}
            Very Poor
          {% endif %}
```

## Security Considerations

- **Local Network Only**: API is accessible only on local network
- **No Authentication**: Consider implementing authentication for external access
- **Network Segmentation**: Ensure proper network isolation if needed
- **Reverse Proxy**: Use reverse proxy with SSL for external access
- **Firewall Rules**: Configure firewall to restrict access as needed

## Performance Optimization

### Scan Intervals
- **Default**: 30 seconds for most sensors
- **Temperature/Humidity**: 60 seconds for stable readings
- **Battery**: 300 seconds for infrequent updates
- **System Status**: 60 seconds for monitoring

### Memory Management
- Monitor Home Assistant memory usage with many REST sensors
- Consider using binary sensors for status indicators
- Implement proper error handling to prevent memory leaks

## Support and Maintenance

### Regular Monitoring
- **Daily**: Check sensor values for reasonableness
- **Weekly**: Verify API response times and reliability
- **Monthly**: Review automation performance and adjust as needed

### Troubleshooting Resources
1. **WeatherStation Logs**: Check serial monitor for device issues
2. **Home Assistant Logs**: Review REST sensor errors and timeouts
3. **Network Diagnostics**: Test connectivity and latency
4. **API Testing**: Use browser or curl to test endpoints directly

### Getting Help
If you encounter persistent issues:
1. Check WeatherStation serial output for errors
2. Test API directly: `curl http://weatherstation.local/api/status`
3. Verify network connectivity between devices
4. Review Home Assistant logs for REST sensor errors
5. Consult the troubleshooting guide in the main documentation

This comprehensive configuration will provide you with robust environmental monitoring in Home Assistant with reliable data collection and intelligent automation capabilities! 🌡️📱🏠
