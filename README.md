# ESP32 FastLED BLE (Bluetooth Low Energy)

Web BLE control app: https://jasoncoon.github.io/esp32-fastled-ble/

\*\* Currently only supported in Chrome on MacOS, Windows, and Linux

**Work in progress BLE port of https://github.com/jasoncoon/esp32-fastled-webserver**

Control addressable LEDs with an ESP32 via a web browser over BLE.

## Features

### Current Status:

- [x] DemoReel100 patterns
- [x] FastLED ESP32 multi-core parallel output support
- [ ] Setting storage in EEPROM
- [x] Ability to adjust these settings via BLE:
  - [x] power on/off
  - [x] brightness
  - [x] pattern
  - [ ] autoplay on/off
  - [ ] autoplay duration
  - [ ] speed
  - [x] palette
  - [ ] auto palette cycling
  - [ ] palette duration
  - [x] solid color
  - [ ] twinkle speed/density
  - [ ] fire cooling/sparking

## Requirements

### Hardware

#### ESP32 Development Board

[![Adafruit HUZZAH32 – ESP32 Feather Board](https://cdn-learn.adafruit.com/assets/assets/000/041/619/thumb100/feather_3405_iso_ORIG.jpg?1494445509)](https://www.adafruit.com/product/3405)

- [Adafruit HUZZAH32 – ESP32 Feather Board](https://www.adafruit.com/product/3405)
- [WEMOS LOLIN D32](https://wiki.wemos.cc/products:d32:d32)
- [NodeMCU-32S Board](https://amzn.to/2MWrCrk)
- [ESP32-DevKitC Board](https://amzn.to/2PpZ1J8)
- [An ESP32 development board of your choice](https://www.google.com/search?q=esp32+development+board)

#### Addressable LED strip

[![Adafruit NeoPixel Ring](https://www.adafruit.com/images/145x109/1586-00.jpg)](https://www.adafruit.com/product/1586)

[Adafruit NeoPixel Ring](https://www.adafruit.com/product/1586)

#### Other hardware:

- [3.3V to 5V Logic Level Shifter](http://www.digikey.com/product-detail/en/texas-instruments/SN74HCT245N/296-1612-5-ND/277258) (required if LEDs "glitch")

Recommended by [Adafruit NeoPixel "Best Practices"](https://learn.adafruit.com/adafruit-neopixel-uberguide/best-practices) to help protect LEDs from current onrush:

- [1000µF Capacitor](http://www.digikey.com/product-detail/en/panasonic-electronic-components/ECA-1EM102/P5156-ND/245015)
- [300 to 500 Ohm resistor](https://www.digikey.com/product-detail/en/stackpole-electronics-inc/CF14JT470R/CF14JT470RCT-ND/1830342)

Level shifter shield PCB:

- [Octo Level Shifter FeatherWing](https://www.evilgeniuslabs.org/level-shifter-featherwing) (tidy level shifter PCB)
- [NodeMCU-32S Level Shifter Shield](https://www.tindie.com/products/jasoncoon/nodemcu-32s-esp32-wi-fi-and-ble-led-controller/)
- [16 Output NodeMCU-32S Level Shifter Shield](https://www.tindie.com/products/jasoncoon/16-output-nodemcu-esp32-wifi-ble-led-controller/)
- [8 Output Wemos D32 Level Shifter Shield](https://www.tindie.com/products/jasoncoon/wemos-d32-esp32-led-level-shifter-shield/)
- [16 Output Wemos D32 Level Shifter Shield](https://www.tindie.com/products/jasoncoon/16-output-wemos-d32-wifi-ble-led-controller/)
- [8 Output ESP32 DevKitC Level Shifter Shield](https://www.tindie.com/products/jasoncoon/esp32-devkitc-wi-fi-and-ble-led-controller/)

### Software

- [Arduino](https://www.arduino.cc/en/main/software)
- [ESP32 Arduino Libraries & Tools](https://github.com/espressif/arduino-esp32)
- [Arduino ESP32 filesystem uploader](https://github.com/me-no-dev/arduino-esp32fs-plugin)

#### Libraries

- [FastLED](https://github.com/FastLED/FastLED)
- [ESP32 Arduino Libraries & Tools](https://github.com/espressif/arduino-esp32)
