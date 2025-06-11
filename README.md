# 🌈 RGB Double Helix Lamp with Arduino Nano

This project is a customizable RGB lamp inspired by the original [RGB Double Helix Lamp - EasyPrint](https://www.printables.com/model/62043-rgb-double-helix-lamp-easyprint). 
It uses an **Arduino Nano** and the **FastLED** library to create dynamic LED lighting effects.

## 🛠️ Project Overview

- Arduino code to control individually addressable RGB LEDs (WS2812B)
- Multiple lighting effects (rainbow, fade, color transitions, etc.)
- 3D printed enclosure (external source)

This project has been continued and improved in a new repository:  
[HelixLamp ESP32 C3](https://github.com/BogyS/HelixLamp_ESP32_C3)

## 📦 What You Need

- Arduino Nano (or compatible)
- WS2812B strip 2x 14 LED's (60 leds per meter) 10 mm width
- 5V USB power supply
- 330Ω resistor (on DIN line), 470µF capacitor, push button, USB connector
- 3D printed parts: [Printables model link](https://www.printables.com/model/62043-rgb-double-helix-lamp-easyprint)

### 📍 Wiring

| Component         | Arduino Nano Pin | Notes                                |
|-------------------|------------------|--------------------------------------|
| WS2812B Data In   | D4               | Use a 330Ω resistor in-line          |
| Button            | D2               | INPUT_PULLUP                         |
| LED Strip VCC     | 5V               | From USB or external power           |
| LED Strip GND     | GND              | Common ground with Arduino           |
| Capacitor         | Across 5V and GND| 470µF, to stabilize power supply     |

## 💡 Libraries Used

- [FastLED](https://github.com/FastLED/FastLED)
- [OneButton](https://github.com/mathertel/OneButton)

## 🕹️ How to Use

See full usage instructions here 👉 [USAGE.md](./doc/USAGE.md)



