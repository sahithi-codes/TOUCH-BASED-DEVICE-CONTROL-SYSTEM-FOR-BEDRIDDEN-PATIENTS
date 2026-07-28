# TOUCH-BASED-DEVICE-CONTROL-SYSTEM-FOR-BEDRIDDEN-PATIENTS

## Overview

This project implements a Password-Protected Touch-Based Device Control System for Bedridden Patients using the LPC2148 ARM7 Microcontroller. The system enables physically challenged or bedridden individuals to control household devices through a resistive touch screen interface after successful password authentication. Passwords are securely stored in EEPROM and can be modified when required. The system also includes an emergency alert feature using a buzzer, providing additional safety and convenience.

---

## Block Diagram

<img width="927" height="566" alt="block_diagram" src="https://github.com/user-attachments/assets/e0292d45-9eba-4019-8965-ec41a00ad5e0" />

# 🏥 Touch-Based Device Control System for Bedridden Patients

[![Microcontroller](https://img.shields.io/badge/MCU-LPC2148%20ARM7-blue.svg)](https://www.nxp.com)
[![Language](https://img.shields.io/badge/Language-Embedded%20C-orange.svg)]()
[![IDE](https://img.shields.io/badge/IDE-Keil%20µVision-green.svg)](https://www.keil.com)
[![License](https://img.shields.io/badge/License-MIT-lightgrey.svg)](LICENSE)

An accessible, secure, and touch-operated device control system built on the **LPC2148 ARM7** microcontroller. Designed specifically for bedridden patients, elderly individuals, and rehabilitation care, this system allows users to control ambient appliances (lights, fans) and trigger emergency alerts independently.

---

## 📌 System Features

* **🔑 Password-Protected Access:** Matrix keypad input with non-volatile password storage in SPI EEPROM.
* **📱 Touch-Screen UI:** UART-driven resistive touch interface for seamless device toggling.
* **⚡ Low-Latency Interrupts:** Fast response times using interrupt-driven UART communication.
* **📺 Real-time Telemetry:** 16x2 LCD display provides visual status updates and security feedback.
* **🚨 Emergency Alert System:** Dedicated touch trigger for immediate buzzer activation.
* **🔒 EEPROM Password Reprogramming:** Securely update and permanently write new credentials on-the-fly.

---

## 🛠️ Hardware & Software Requirements

### Hardware Components
* **Microcontroller:** LPC2148 ARM7TDMI-S
* **Touch Interface:** Resistive Touch Screen Module + Controller
* **Storage:** AT25LC512 SPI EEPROM
* **Input Device:** 4x4 Matrix Keypad
* **Display:** 16x2 Character LCD
* **Actuators/Outputs:** LED1 (Light), LED2 (Fan), Piezo Buzzer
* **Power Supply:** 5V / 3.3V Regulated DC Supply

### Software Toolchain
* **Language:** Embedded C
* **IDE:** Keil µVision (ARM Compiler)
* **Flashing Tool:** Flash Magic
* **Simulation (Optional):** Proteus VSM

---

## 📐 System Architecture

| Peripheral | Protocol / Bus | Description |
| :--- | :--- | :--- |
| **Resistive Touch** | UART (Interrupt Driven) | Sends coordinate data to MCU for action mapping |
| **AT25LC512 EEPROM** | Hardware SPI | Secure, non-volatile password storage |
| **4x4 Matrix Keypad** | GPIO Matrix | Secure PIN entry and system setup |
| **16x2 LCD** | GPIO Parallel | Visual output for device states and feedback |
| **LEDs & Buzzer** | Digital GPIO Out | Appliance simulation and acoustic emergency alert |

---

## 🚀 Operating Instructions (How to Use)

### 1️⃣ System Initialization
1. Connect the power supply to the LPC2148 development board.
2. The LCD displays `SYSTEM INITIALIZING...` followed by `ENTER PASSWORD`.

### 2️⃣ Authentication
1. Enter the multi-digit password using the **4x4 Matrix Keypad**.
2. Press `#` (or designated key) to submit.
3. Upon validation, the LCD will read `ACCESS GRANTED`, enabling the touch screen controls.

### 3️⃣ Operating Devices via Touch Screen
* **Turn ON Device 1 (Light):** Tap the **Device 1** area on the touch screen. The system processes the UART interrupt, toggles **LED1 ON**, and updates the LCD to `DEV1: ON`.
* **Turn ON Device 2 (Fan):** Tap the **Device 2** area. **LED2 turns ON**, and LCD updates to `DEV2: ON`.
* **Trigger Emergency Buzzer:** Tap the **EMERGENCY** region. The piezo buzzer will sound continuously until reset.

### 4️⃣ Modifying the Password
1. Press `*` on the keypad to initiate the reset sequence.
2. Authenticate with the current password.
3. Input the new password. The LPC2148 will update the stored bytes in the **AT25LC512 EEPROM** over SPI.

---

## 🔮 Future Roadmap

- [ ] **IoT & Mobile Dashboard:** Integrate ESP8266/ESP32 for cloud logging and remote caregiver access.
- [ ] **Patient Vitals Monitoring:** Add I2C sensors for Heart Rate (MAX30102), Body Temp, and SpO₂ monitoring.
- [ ] **Wireless Relays:** Upgrade control outputs using NRF24L01 / Bluetooth to switch AC high-voltage mains.

---

## 🤝 Project Outcomes

* Developed a fully modular, low-power assistive control system for patient independence.
* Implemented interrupt-driven peripheral handling (UART, SPI, GPIO) on ARM7 hardware.
* Created a secure authentication pipeline using SPI-based EEPROM persistence.
