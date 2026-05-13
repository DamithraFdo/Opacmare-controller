/* Self-control.ino
 * Damithrafdo from StrydoLabs All Right Reserved | strydolabs@gmail.com | damithrafdo@gmail.com | +94716507322
 * Alternative solution for Opacmare-transformer platform controller
 * Hardware: Arduino UNO, MPU6050 gyro sensor, Relay module, Push button
 * Simulation: https://wokwi.com/projects/461944067471851521
 * GitHub: https://github.com/DamithraFdo/Opacmare-controller
 */

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

// MPU6050
Adafruit_MPU6050 mpu;
sensors_event_t accel, gyro, temp;

// PINS
const int button1 = 3;
const int button2 = 4;

const int relay1 = 7;
const int relay2 = 8;
const int relay3 = 9;
const int relay4 = 10;

// ANGLE SETTINGS
const float negativeLimit = -7.0;
const float positiveLimit = 7.0;

const float resetMin = -3.0;
const float resetMax = 3.0;

const float alpha = 0.96;

// VARIABLES
float angleY = 0;
unsigned long lastTime;

// POSITION CONTROL FUNCTION
void maintainPosition(bool systemActive, int activeRelayPin) {

  if (!systemActive) return;

  // TOO LOW (-7)
  // NEED TO GO UP
  if (angleY < negativeLimit) {

    digitalWrite(activeRelayPin, LOW);   // main relay OFF
    digitalWrite(relay2, HIGH);         // correction UP
    digitalWrite(relay4, LOW);
  }

  // TOO HIGH (+7)
  // NEED TO GO DOWN
  else if (angleY > positiveLimit) {

    digitalWrite(activeRelayPin, LOW);   // main relay OFF
    digitalWrite(relay2, LOW);
    digitalWrite(relay4, HIGH);         // correction DOWN
  }

  // STABLE ZONE (-3 to +3)
  else if (angleY >= resetMin && angleY <= resetMax) {

    digitalWrite(relay2, LOW);
    digitalWrite(relay4, LOW);

    digitalWrite(activeRelayPin, HIGH); // main relay ON again
  }
}

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

  // MPU INIT
  while (!mpu.begin()) {
    Serial.println("MPU6050 not connected!");
    delay(1000);
  }

  Serial.println("MPU6050 READY!");

  lastTime = millis();
}

void loop() {

  // Read sensor
  mpu.getEvent(&accel, &gyro, &temp);

  // time delta
  unsigned long currentTime = millis();
  float dt = (currentTime - lastTime) / 1000.0;
  lastTime = currentTime;

  // accelerometer angle
  float accAngleY =
    atan2(accel.acceleration.x,
          accel.acceleration.z) * 180 / PI;

  // gyro rate
  float gyroRateY = gyro.gyro.y * 180 / PI;

  // complementary filter
  angleY =
    alpha * (angleY + gyroRateY * dt)
    + (1 - alpha) * accAngleY;

  // button states
  bool b1 = (digitalRead(button1) == LOW);
  bool b2 = (digitalRead(button2) == LOW);

  // debug
  Serial.print("Angle Y: ");
  Serial.println(angleY);

  // BUTTON 1 SYSTEM
  if (b1) {
    maintainPosition(true, relay1);
  } else {
    digitalWrite(relay1, LOW);
  }

  // BUTTON 2 SYSTEM
  if (b2) {
    maintainPosition(true, relay3);
  } else {
    digitalWrite(relay3, LOW);
  }

  // SAFETY: no buttons pressed
  if (!b1 && !b2) {
    digitalWrite(relay2, LOW);
    digitalWrite(relay4, LOW);
  }

  delay(20);
}