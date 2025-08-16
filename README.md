# ESP32-S3 Keyboard  
**Teensyduino-Compatible API + USB (Adafruit TinyUSB) + BLE (NimBLE)**

This project is a **port of the [Teensyduino Keyboard class](https://www.pjrc.com/teensy/td_keyboard.html)** for the **ESP32-S3**,supporting both **USB HID** via [Adafruit TinyUSB](https://github.com/adafruit/Adafruit_TinyUSB_Arduino) and **Bluetooth Low Energy HID** via [NimBLE-Arduino](https://github.com/h2zero/NimBLE-Arduino).

It allows you to use the familiar `Keyboard` API from Teensy on ESP32-S3 boards to send keystrokes either over USB when plugged in, or wirelessly via BLE — with minimal code changes.

## ✨ Features

- **Teensy-compatible API** – works with `Keyboard.print()`, `Keyboard.press()`, `Keyboard.release()`, etc.
- **Dual output modes**:
  - **USB HID** – native USB keyboard using Adafruit TinyUSB.
  - **BLE HID** – wireless keyboard using NimBLE.
- **Low memory footprint** – NimBLE uses far less RAM than Bluedroid.
- **Full keycode support** – modifiers, special keys, printable characters.
- **Easy migration** – drop-in replacement for many Teensy keyboard projects.

## 🚀 Key Advantage Over ESP32-BLE-Keyboard

Unlike the popular [ESP32-BLE-Keyboard](https://github.com/T-vK/ESP32-BLE-Keyboard) library, **this library supports multiple keyboard layouts** (not just US).  
This makes it easier to use the correct key mapping for your language and region without rewriting code or manually remapping keys.

## 📦 Requirements

- **Board**: ESP32-S3 with native USB (e.g., ESP32-S3-DevKitC, ESP32-S3-Box)
- **Framework**: Arduino-ESP32
- **Libraries**:
  - [Adafruit TinyUSB Arduino](https://github.com/adafruit/Adafruit_TinyUSB_Arduino)
  - [NimBLE-Arduino](https://github.com/h2zero/NimBLE-Arduino)

## 📚 References

- [Teensyduino Keyboard Documentation](https://www.pjrc.com/teensy/td_keyboard.html)  
- [Adafruit TinyUSB Arduino Library](https://github.com/adafruit/Adafruit_TinyUSB_Arduino)  
- [NimBLE-Arduino Library](https://github.com/h2zero/NimBLE-Arduino)  
