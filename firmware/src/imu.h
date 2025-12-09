// firmware/src/imu.h
#pragma once
#include <Arduino.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

struct CursorDelta {
  int dx;
  int dy;
};

class IMUHandler {
public:
  bool begin();
  CursorDelta computeCursorDelta();
  float getGyroX(); // Get raw gyro X for slide navigation

private:
  Adafruit_MPU6050 mpu;
  float lastGyroX = 0;
  float lastGyroY = 0;
  
  // Simple smoothing filter
  float smoothGyroX = 0;
  float smoothGyroY = 0;
  static const float FILTER_ALPHA = 0.7f; // smoothing factor
};

