@echo off
REM PlatformIO Serial Monitor with forced 115200 baud rate
REM This script ALWAYS forces 115200 baud, bypassing VS Code extension issues
REM Usage: monitor.bat [COM_PORT]
REM Example: monitor.bat COM4

echo ========================================
echo Manoeuvre Serial Monitor (115200 baud)
echo ========================================
echo.

if "%1"=="" (
    echo Starting PlatformIO monitor at 115200 baud...
    echo If you need to specify a port, use: monitor.bat COM4
    echo.
    pio device monitor -b 115200 -e esp32dev
) else (
    echo Starting PlatformIO monitor on %1 at 115200 baud...
    echo.
    pio device monitor -b 115200 -p %1 -e esp32dev
)

pause

