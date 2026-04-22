/* main.ino
 * Damithrafdo from StrydoLabs All Right Reserved | strydolabs@gmail.com | damithrafdo@gmail.com | +94716507322
 * Alternative solution for Opacmare-transformer platform controller
 * Hardware: Arduino UNO, MPU6500 gyro sensor, Relay module, Push button
 * Simulation: https://wokwi.com/projects/461944067471851521
 * GitHub: https://github.com/DamithraFdo/Opacmare-controller
 */

/* main code with latching behavior:
   If it exceeds 7° → turn OFF relay2
   Keep it OFF until gyroY returns to ~0°
   Then turn it ON again
   If it exceeds -7° → turn OFF relay2
   Keep it OFF until gyroY returns to ~0°
   NB:  −2≤gyroY≤2
  This is much more stable than exact zero.
  Perfect for real sensors.

 * Opacmare Controller - MPU6500 Direct I2C Version

 */

#include <Wire.h>

// MPU6500 I2C address
#define MPU6500_ADDR 0x68

// Registers
#define PWR_MGMT_1   0x6B
#define GYRO_YOUT_H  0x45

// Pins
const int button1 = 3;
const int button2 = 4;
const int relay1 = 7;
const int relay2 = 8;
const int relay3 = 9;
const int relay4 = 10;

// Thresholds
const float thresholdMax = 10.0;
const float thresholdMin = -10.0;
// Reset zone
const float resetMin = -5.0;
const float resetMax = 5.0;

// Relay latch states
bool relay2Locked = false;
bool relay4Locked = false;

// ---------------- MPU FUNCTIONS ----------------

// Read 16-bit signed register value
int16_t read16Bit(byte reg) {
  Wire.beginTransmission(MPU6500_ADDR);
  Wire.write(reg);
  Wire.endTransmission(false);

  Wire.requestFrom(MPU6500_ADDR, 2);

  int16_t value = (Wire.read() << 8) | Wire.read();
  return value;
}

// Read gyro Y in deg/s
float readGyroY() {
  int16_t rawGyroY = read16Bit(GYRO_YOUT_H);

  // Default sensitivity ±250 dps = 131 LSB/deg/s
  float gyroY = rawGyroY / 131.0;

  return gyroY;
}

// ---------------- SETUP ----------------

void setup() {
  Serial.begin(115200);
  Wire.begin();

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

  // Wake up MPU6500
  Wire.beginTransmission(MPU6500_ADDR);
  Wire.write(PWR_MGMT_1);
  Wire.write(0x00);
  Wire.endTransmission();

  delay(100);

  Serial.println("MPU6500 ready!");
}

// ---------------- LOOP ----------------

void loop() {
  bool b1 = (digitalRead(button1) == LOW);
  bool b2 = (digitalRead(button2) == LOW);

  float gyroY = readGyroY();

  Serial.print("Gyro Y: ");
  Serial.println(gyroY);

  // ---------- BUTTON 1 ----------
  if (b1) {
    digitalWrite(relay1, HIGH);

    if (gyroY > thresholdMax || gyroY < thresholdMin) {
      relay2Locked = true;
    }

    if (relay2Locked && gyroY >= resetMin && gyroY <= resetMax) {
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

    if (gyroY > thresholdMax || gyroY < thresholdMin) {
      relay4Locked = true;
    }

    if (relay4Locked && gyroY >= resetMin && gyroY <= resetMax) {
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