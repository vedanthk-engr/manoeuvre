#!/bin/bash
# PlatformIO Serial Monitor with forced 115200 baud rate
# Usage: ./monitor.sh [COM_PORT]
# Example: ./monitor.sh /dev/ttyUSB0

if [ -z "$1" ]; then
    echo "Starting PlatformIO monitor at 115200 baud..."
    echo "If you need to specify a port, use: ./monitor.sh /dev/ttyUSB0"
    pio device monitor -b 115200 -e esp32dev
else
    echo "Starting PlatformIO monitor on $1 at 115200 baud..."
    pio device monitor -b 115200 -p "$1" -e esp32dev
fi

