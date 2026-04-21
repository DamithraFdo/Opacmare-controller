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

