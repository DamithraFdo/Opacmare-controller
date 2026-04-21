/*
 * Damithrafdo from StrydoLabs All Right Reserved | strydolabs@gmail.com | damithrafdo@gmail.com | +94716507322
 * Alternative solution for Opacmare-transformer platform controller
 * Hardware: Arduino UNO, MPU6050 gyro sensor, Relay module, Push button
 * Simulation: https://wokwi.com/projects/461944067471851521
 * GitHub: https://github.com/DamithraFdo/Opacmare-controller
 */

/* “marine-safe”
    Handles wave motion & vibration
    No relay flicker (thanks to hysteresis)
    Doesn’t depend on perfect “0°”
    Smooth angle instead of noisy gyro spikes
    Safe reset when buttons released
*/
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;

sensors_event_t accel, gyro, temp;

// Pins
const int button1 = 3;
const int button2 = 4;
const int relay1 = 7; // Platform
const int relay2 = 8; // Base

// Angle variables
float pitch = 0;
unsigned long lastTime = 0;

// Thresholds (HYSTERESIS)
float triggerAngle = 7.0;
float resetAngle   = 2.0;

// State
bool relay2Locked = false;

// Debounce
unsigned long lastDebounceTime = 0;
const int debounceDelay = 50;

void setup() {
  Serial.begin(115200);

  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);

  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);

  digitalWrite(relay1, LOW);
  digitalWrite(relay2, LOW);

  while (!mpu.begin()) {
    Serial.println("MPU6050 not connected!");
    delay(1000);
  }

  Serial.println("MPU6050 ready!");
  lastTime = millis();
}

void loop() {
  mpu.getEvent(&accel, &gyro, &temp);

  // Time delta
  unsigned long now = millis();
  float dt = (now - lastTime) / 1000.0;
  lastTime = now;

  // --- Calculate pitch angle ---
  float accPitch = atan2(accel.acceleration.y, accel.acceleration.z) * 180 / PI;
  float gyroRate = gyro.gyro.x * 180 / PI;

  // Complementary filter
  pitch = 0.98 * (pitch + gyroRate * dt) + 0.02 * accPitch;

  // --- Read buttons (debounced) ---
  bool b1 = (digitalRead(button1) == LOW);
  bool b2 = (digitalRead(button2) == LOW);

  if ((millis() - lastDebounceTime) > debounceDelay) {

    if (b1 && b2) {
      digitalWrite(relay1, HIGH);

      // Trigger condition
      if (pitch > triggerAngle) {
        relay2Locked = true;
      }

      // Reset condition (hysteresis)
      if (relay2Locked && pitch < resetAngle) {
        relay2Locked = false;
      }

      // Apply relay state
      digitalWrite(relay2, relay2Locked ? LOW : HIGH);

    } else {
      digitalWrite(relay1, LOW);
      digitalWrite(relay2, LOW);
      relay2Locked = false;
    }

    lastDebounceTime = millis();
  }

  // Debug
  Serial.print("Pitch: ");
  Serial.println(pitch);

  delay(10);
}

/*
 * ============================================================
 * Marine-Safe Opacmare Controller (MPU6050)
 * ============================================================
 *
 * Purpose:
 * This code controls a 2-relay Opacmare system using:
 *  - Dual safety push buttons (D3, D4)
 *  - MPU6050 tilt angle (pitch)
 *
 * Designed for marine/boat environments where vibration and
 * noise can cause unstable readings.
 *
 * ------------------------------------------------------------
 * How it works:
 *
 * 1. Sensor Fusion:
 *    MPU6050 accel + gyro are combined using a complementary
 *    filter to produce stable pitch angle.
 *
 * 2. Safety Logic:
 *    Both buttons must be pressed to activate system.
 *    Otherwise all outputs are OFF.
 *
 * 3. Relay Control:
 *    Relay1 (D7): ON when system is enabled.
 *    Relay2 (D8): Controlled by tilt angle.
 *
 * 4. Marine-safe Latching:
 *    - If pitch > 7° → Relay2 OFF (locked state)
 *    - If pitch < 2° → Relay2 ON again
 *    (prevents relay flickering due to wave motion)
 *
 * 5. Stability Features:
 *    - Hysteresis for noise rejection
 *    - Button debounce
 *    - Non-blocking timing
 *
 * ------------------------------------------------------------
 * Notes:
 * - Mount sensor firmly for accurate readings
 * - Tune thresholds based on mechanical setup
 *
 * ============================================================
 */