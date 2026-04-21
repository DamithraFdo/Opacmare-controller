/*
 * Damithrafdo from StrydoLabs All Right Reserved | strydolabs@gmail.com | damithrafdo@gmail.com | +94716507322
 * Alternative solution for Opacmare-transformer platform controller
 * Hardware: Arduino UNO, MPU6050 gyro sensor, Relay module, Push button
 * Simulation: https://wokwi.com/projects/461944067471851521
 * GitHub: https://github.com/DamithraFdo/Opacmare-controller
 */


#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;

sensors_event_t accel, gyro, temp;

// Pin definitions
const int button1 = 3;
const int button2 = 4;
const int relay1 = 7;
const int relay2 = 8;

void setup(void) {
  Serial.begin(115200);

  // Button inputs (using internal pull-up)
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);

  // Relay outputs
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);

  // Start with relays OFF
  digitalWrite(relay1, LOW);
  digitalWrite(relay2, LOW);

  while (!mpu.begin()) {
    Serial.println("MPU6050 not connected!");
    delay(1000);
  }
  Serial.println("MPU6050 ready!");
}

void loop() {
  mpu.getEvent(&accel, &gyro, &temp);

  // Read buttons (LOW = pressed)
  bool b1 = (digitalRead(button1) == LOW);
  bool b2 = (digitalRead(button2) == LOW);

  float gyroX = gyro.gyro.x * 180 / PI; // convert to deg/s
  float gyroY = gyro.gyro.y * 180 / PI;
  float gyroZ = gyro.gyro.z * 180 / PI;

  Serial.print("Gyro X: ");
  Serial.print(gyroX);
  Serial.print(", Y: ");
  Serial.print(gyroY);
  Serial.print(", Z: ");
  Serial.println(gyroZ);



  if (b1 && b2) {
    // Both buttons pressed
    digitalWrite(relay1, HIGH);

    if (gyroY > 3) { //(gyroZ > 3) logic set to 3 degrees
      digitalWrite(relay2, LOW);   // turn OFF D8 relay
    } else {
      digitalWrite(relay2, HIGH);  // both ON
    }

  } else {
    // If not both pressed → everything OFF
    digitalWrite(relay1, LOW);
    digitalWrite(relay2, LOW);
  }

  delay(100);
}