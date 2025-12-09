// firmware/src/imu.cpp
#include "imu.h"
#include "config.h"
#include <Wire.h>

bool IMUHandler::begin() {
  Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL);
  
  if (!mpu.begin()) {
    return false;
  }
  
  // Configure ranges for better sensitivity
  mpu.setAccelerometerRange(MPU6050_RANGE_4_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  
  return true;
}

CursorDelta IMUHandler::computeCursorDelta() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // Use gyro to drive cursor (rad/s -> cursor delta)
  float gx = g.gyro.x;
  float gy = g.gyro.y;

  // Apply smoothing filter to reduce noise
  smoothGyroX = FILTER_ALPHA * smoothGyroX + (1.0f - FILTER_ALPHA) * gx;
  smoothGyroY = FILTER_ALPHA * smoothGyroY + (1.0f - FILTER_ALPHA) * gy;

  CursorDelta delta {0, 0};

  // Apply deadzone and gain
  if (fabs(smoothGyroX) > CURSOR_DEADZONE) {
    delta.dx = (int)(smoothGyroX * CURSOR_GAIN_X);
  }
  if (fabs(smoothGyroY) > CURSOR_DEADZONE) {
    delta.dy = (int)(-smoothGyroY * CURSOR_GAIN_Y); // invert Y for natural movement
  }

  return delta;
}

float IMUHandler::getGyroX() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  return g.gyro.x;
}

