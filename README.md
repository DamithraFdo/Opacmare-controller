# Opacmare Controller

Arduino-based control system using MPU6050, push buttons, and relays.

 * Simulation: https://wokwi.com/projects/461944067471851521
 * GitHub: https://github.com/DamithraFdo/Opacmare-controller

## Features

* Dual button safety activation
* MPU6050 gyro-based control logic
* Relay switching based on motion threshold

## Hardware

* Arduino (Uno/Nano)
* MPU6050
* 2x Push buttons (D3, D4)
* 2x Relays (D7, D8)

## Logic

* Both buttons must be pressed to activate system
* If Gyro Y > threshold → Relay D8 OFF
* Else → Both relays ON

## Notes

* Uses raw gyro data (not filtered angle)
* Can be upgraded with complementary/Kalman filter

## Alternative Implementations

This repository includes multiple versions of the control logic under the `Alternative/` folder for different use cases and testing conditions.

### 1. latching behavior.ino

* Uses **raw gyroscope (Y-axis)** values
* Implements **latching logic**:

  * If gyro exceeds threshold → Relay OFF
  * Remains OFF until motion returns near zero
* Simple and lightweight

**Use when:**

* You need a quick solution
* System is relatively stable (low vibration)
* Precision is not critical

---

### 2. marine-safe.ino

* Uses **filtered angle (pitch) from MPU6050**
* Implements:

  * Complementary filter (gyro + accelerometer)
  * Hysteresis (separate trigger/reset angles)
  * Debounced button input
* Designed for **real-world marine conditions**

**Use when:**

* Installed on boat / high vibration environment
* Need stable and reliable behavior
* Avoid relay flickering and false triggers

---
