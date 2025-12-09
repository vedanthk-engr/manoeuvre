# Hardware Pinout (locked for Phase 1)

## ESP32 → MPU6050 (I2C)
- SDA → GPIO 21
- SCL → GPIO 22
- VCC → 3.3V
- GND → GND

## Touch Sensors (TTP223)
- Touch 1 OUT → GPIO 4
- Touch 2 OUT → GPIO 15
- VCC → 3.3V
- GND → GND

## LED
- LED+ → 220Ω resistor → GPIO 2
- LED− → GND

Notes:
- GPIO 2 has an onboard LED on many ESP32 boards; external LED also OK.
- All sensors power from 3.3V (no 5V). Short wires on IMU to reduce noise.

