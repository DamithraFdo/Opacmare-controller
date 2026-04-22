/* latching behavior.ino
 * Damithrafdo from StrydoLabs All Right Reserved | strydolabs@gmail.com | damithrafdo@gmail.com | +94716507322
 * Alternative solution for Opacmare-transformer platform controller
 * Hardware: Arduino UNO, MPU6050 gyro sensor, Relay module, Push button
 * Simulation: https://wokwi.com/projects/461944067471851521
 * GitHub: https://github.com/DamithraFdo/Opacmare-controller
 */

/* latching behavior:
   If it exceeds 7° → turn OFF relay2
   Keep it OFF until gyroY returns to ~0°
   Then turn it ON again
*/

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;
sensors_event_t accel, gyro, temp;

// Pins
const int button1 = 3;
const int button2 = 4;
const int relay1 = 7;
const int relay2 = 8;
const int relay3 = 9;
const int relay4 = 10;

const float threshold = 10.0;
const float resetThreshold = 0.5;

bool relay2Locked = false;
bool relay4Locked = false;

void setup() {
  Serial.begin(115200);

  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);

  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);

  digitalWrite(relay1, LOW);
  digitalWrite(relay2, LOW);
  digitalWrite(relay3, LOW);
  digitalWrite(relay4, LOW);

  while (!mpu.begin()) {
    Serial.println("MPU6050 not connected!");
    delay(1000);
  }

  Serial.println("MPU6050 ready!");
}

void loop() {
  mpu.getEvent(&accel, &gyro, &temp);

  bool b1 = (digitalRead(button1) == LOW);
  bool b2 = (digitalRead(button2) == LOW);

  float gyroY = gyro.gyro.y * 180 / PI;

  Serial.print("Gyro Y: ");
  Serial.println(gyroY);

  // ---------- BUTTON 1 ----------
  if (b1) {
    digitalWrite(relay1, HIGH);

    if (gyroY > threshold) {
      relay2Locked = true;
    }

    if (relay2Locked && abs(gyroY) < resetThreshold) {
      relay2Locked = false;
    }

    digitalWrite(relay2, relay2Locked ? LOW : HIGH);
  } else {
    digitalWrite(relay1, LOW);
    digitalWrite(relay2, LOW);
    relay2Locked = false;
  }

  // ---------- BUTTON 2 ----------
  if (b2) {
    digitalWrite(relay3, HIGH);

    if (gyroY > threshold) {
      relay4Locked = true;
    }

    if (relay4Locked && abs(gyroY) < resetThreshold) {
      relay4Locked = false;
    }

    digitalWrite(relay4, relay4Locked ? LOW : HIGH);
  } else {
    digitalWrite(relay3, LOW);
    digitalWrite(relay4, LOW);
    relay4Locked = false;
  }

  delay(100);
}