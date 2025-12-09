#include <Arduino.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

// --- PIN DEFINITIONS ---
#define LED_PIN 2           // Built-in LED on most ESP32 Dev Boards
#define TOUCH_PIN_1 4       // GPIO 4 (Touch 0)
#define TOUCH_PIN_2 15      // GPIO 15 (Touch 3)

// --- OBJECTS ---
Adafruit_MPU6050 mpu;

// --- CONFIG ---
const int touchThreshold = 40; // Adjust based on your specific module/wire length

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10); // Wait for serial

  Serial.println("\n--- Manoeuvre Hardware Test ---");

  // 1. Setup LED
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH); // On at start
  delay(500);
  digitalWrite(LED_PIN, LOW);
  Serial.println("LED Test: Blinked once.");

  // 2. Setup MPU6050
  Serial.print("Initializing MPU6050...");
  if (!mpu.begin()) {
    Serial.println("Failed! Check wiring (SDA=21, SCL=22).");
    while (1) {
      digitalWrite(LED_PIN, !digitalRead(LED_PIN)); // Fast blink error
      delay(100);
    }
  }
  Serial.println("Success!");

  // MPU Config
  mpu.setAccelerometerRange(MPU6050_RANGE_4_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  Serial.println("Setup Complete. Loops starting...");
}

void loop() {
  // --- READ IMU ---
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // --- READ TOUCH ---
  // ESP32 touchRead returns lower value when touched
  int t1 = touchRead(TOUCH_PIN_1);
  int t2 = touchRead(TOUCH_PIN_2);

  bool isTouched1 = (t1 < touchThreshold);
  bool isTouched2 = (t2 < touchThreshold);

  // --- LED FEEDBACK ---
  // Light up LED if either sensor is touched
  digitalWrite(LED_PIN, (isTouched1 || isTouched2) ? HIGH : LOW);

  // --- SERIAL OUTPUT ---
  // Format: "IMU: ax,ay,az | T1: raw (bool) | T2: raw (bool)"
  Serial.print("IMU: ");
  Serial.print(a.acceleration.x, 1); Serial.print(",");
  Serial.print(a.acceleration.y, 1); Serial.print(",");
  Serial.print(a.acceleration.z, 1);
  
  Serial.print(" | T1: "); Serial.print(t1); Serial.print("("); Serial.print(isTouched1); Serial.print(")");
  Serial.print(" | T2: "); Serial.print(t2); Serial.print("("); Serial.print(isTouched2); Serial.print(")");
  
  Serial.println();

  delay(100); // 10Hz log rate
}
