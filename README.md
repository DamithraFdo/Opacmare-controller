# ⚓ Opacmare Controller

Arduino-based control system for an **Opacmare transformer boat platform** using **MPU6050**, **push buttons**, and **relay-based latching safety control**.

This project is designed as an **alternative control solution for platform stabilization and safety relay switching** in marine environments.

- **Simulation:** https://wokwi.com/projects/461944067471851521
- **GitHub:** https://github.com/DamithraFdo/Opacmare-controller

---

## 🚀 Current Stable Version

The **fully functional main production code** is now available in:

`main.ino`

This is the recommended file for:

- real hardware testing
- deployment
- further development

---

## ✨ Features

- Dual independent button control channels
- MPU6050 gyro-based motion threshold detection
- Relay latching safety logic
- Positive and negative threshold support
- Automatic relay recovery when motion returns near zero
- Lightweight Arduino UNO compatible implementation
- Multiple alternative implementations for testing and marine-safe deployment

---

## 🧩 Hardware Used

- Arduino UNO / Nano
- MPU6050 (Gyroscope + Accelerometer)
- 2 Push Buttons
  - `D3` → Button 1
  - `D4` → Button 2
- 4 Relays
  - `D7` → Relay 1
  - `D8` → Relay 2
  - `D9` → Relay 3
  - `D10` → Relay 4

---

## 📂 Project Structure

```text
Opacmare-controller/
│── Opacmare-controller.ino
├── main.ino
├── README.md
│
├── Alternative/
│   ├── latching behavior.ino
│   └── marine-safe.ino
```

---

## ⚙️ Main Control Logic (`main.ino`)

The main code implements **dual relay latching logic**.

### 🔘 Button 1 Logic

When **Button 1 (D3)** is pressed:

- `Relay 1` → ON
- `Relay 2` → ON

If:

`Gyro Y > +10`

OR

`Gyro Y < -10`

Then:

- `Relay 2` → OFF
- remains OFF (latched state)

It automatically turns ON again when:

`Gyro Y ≈ 0`

### 🔘 Button 2 Logic

When **Button 2 (D4)** is pressed:

- `Relay 3` → ON
- `Relay 4` → ON

If:

`Gyro Y > +10`

OR

`Gyro Y < -10`

Then:

- `Relay 4` → OFF
- remains OFF (latched state)

It automatically turns ON again when:

`Gyro Y ≈ 0`

---

## 🎯 Threshold Configuration

```cpp
const float thresholdMax = 10.0;
const float thresholdMin = -10.0;
const float resetThreshold = 0.5;
```

---

## 📁 File Descriptions

### ✅ `main.ino` *(Recommended)*

This is the **latest stable fully functional version**.

### 🧪 `Opacmare-controller.ino`

Legacy base implementation / original project file.

### 🧪 `Alternative/latching behavior.ino`

Simplified latch logic implementation.

### 🌊 `Alternative/marine-safe.ino`

Advanced real-world deployment version.

---

## ⚠️ Important Technical Note

The current `main.ino` logic uses raw gyroscope Y-axis angular velocity (deg/s), not actual tilt angle.

For higher accuracy in real marine conditions, the `marine-safe.ino` version is recommended.

---

## 👨‍💻 Author

**Damithra Fernando**  
Founder & CEO — **Strydo Labs**

📧 strydolabs@gmail.com  
📧 damithrafdo@gmail.com  

**Connecting Analog to Digital**
