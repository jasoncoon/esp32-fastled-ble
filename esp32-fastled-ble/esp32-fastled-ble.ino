/*
   ESP32 FastLED BLE: https://github.com/jasoncoon/esp32-fastled-ble
   Copyright (C) 2017 Jason Coon

   Built upon the amazing FastLED work of Daniel Garcia and Mark Kriegsman:
   https://github.com/FastLED/FastLED

   ESP32 support provided by the hard work of Sam Guyer:
   https://github.com/samguyer/FastLED

   Arduino BLE support based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleNotify.cpp
   Ported to Arduino ESP32 by Evandro Copercini
   updated by chegewara

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <FastLED.h>

#include <EEPROM.h>

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001008)
#warning "Requires FastLED 3.1.8 or later; check github for latest code."
#endif

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

#define DATA_PIN    18
//#define CLK_PIN     12
#define LED_TYPE SK6812
#define COLOR_ORDER GRB
#define NUM_STRIPS 1
#define NUM_LEDS_PER_STRIP 64
#define NUM_LEDS NUM_LEDS_PER_STRIP *NUM_STRIPS
CRGB leds[NUM_LEDS];

#define MILLI_AMPS 1600 // IMPORTANT: set the max milli-Amps of your power supply (4A = 4000mA)
#define FRAMES_PER_SECOND 120

uint8_t power = 1;
uint8_t brightness = 16;
CRGB solidColor = CRGB::Green;

uint8_t currentPatternIndex = 0;
uint8_t currentPaletteIndex = 0;
uint8_t gHue = 0; // rotating "base color" used by many of the patterns
uint8_t speed = 30;
uint8_t cooling = 50;
uint8_t sparking = 120;

#include "patterns.h"
#include "ble.h"

void setup()
{
  Serial.begin(115200);

  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, 0, NUM_LEDS_PER_STRIP);

  // 23, 22,  3, 21, 19, 18,  5,  4,  0,  2, 15, 25, 26, 14, 12, 13
  // FastLED.addLeds<LED_TYPE, 23, COLOR_ORDER>(leds, 0 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  // FastLED.addLeds<LED_TYPE, 22, COLOR_ORDER>(leds, 1 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  // FastLED.addLeds<LED_TYPE, 3, COLOR_ORDER>(leds, 2 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  // FastLED.addLeds<LED_TYPE, 21, COLOR_ORDER>(leds, 3 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  // FastLED.addLeds<LED_TYPE, 19, COLOR_ORDER>(leds, 4 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  // FastLED.addLeds<LED_TYPE, 18, COLOR_ORDER>(leds, 5 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  // FastLED.addLeds<LED_TYPE, 5, COLOR_ORDER>(leds, 6 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  // FastLED.addLeds<LED_TYPE, 4, COLOR_ORDER>(leds, 7 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  // FastLED.addLeds<LED_TYPE, 0, COLOR_ORDER>(leds, 8 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  // FastLED.addLeds<LED_TYPE, 2, COLOR_ORDER>(leds, 9 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  // FastLED.addLeds<LED_TYPE, 15, COLOR_ORDER>(leds, 10 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  // FastLED.addLeds<LED_TYPE, 25, COLOR_ORDER>(leds, 11 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  // FastLED.addLeds<LED_TYPE, 26, COLOR_ORDER>(leds, 12 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  // FastLED.addLeds<LED_TYPE, 14, COLOR_ORDER>(leds, 13 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  // FastLED.addLeds<LED_TYPE, 12, COLOR_ORDER>(leds, 14 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  // FastLED.addLeds<LED_TYPE, 13, COLOR_ORDER>(leds, 15 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);

  //  FastLED.setCorrection(Typical8mmPixel);
  FastLED.setCorrection(TypicalLEDStrip);
  //  FastLED.setCorrection(UncorrectedColor);
  // FastLED.setDither(0);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, MILLI_AMPS);

  // set master brightness control
  FastLED.setBrightness(brightness);

  setupBLE();
}

void loop()
{
  //  // notify changed value
  //  if (deviceConnected && colorChanged) {
  //    pCharacteristic->setValue((uint8_t*)&value, 4);
  //    pCharacteristic->notify();
  //    value++;
  //    delay(3); // bluetooth stack will go into congestion, if too many packets are sent, in 6 hours test i was able to go as low as 3ms
  //  }

  // disconnecting
  if (!deviceConnected && oldDeviceConnected)
  {
    delay(500);                  // give the bluetooth stack the chance to get things ready
    pServer->startAdvertising(); // restart advertising
    Serial.println("start advertising");
    oldDeviceConnected = deviceConnected;
  }

  // connecting
  if (deviceConnected && !oldDeviceConnected)
  {
    // do stuff here on connecting
    oldDeviceConnected = deviceConnected;
    Serial.println("connecting");
  }

  if (power == 0)
  {
    fill_solid(leds, NUM_LEDS, CRGB::Black);
  }
  else
  {
    // Call the current pattern function once, updating the 'leds' array
    patterns[currentPatternIndex].pattern();

    EVERY_N_MILLISECONDS(40)
    {
      // slowly blend the current palette to the next
      nblendPaletteTowardPalette(currentPalette, targetPalette, 8);
      gHue++; // slowly cycle the "base color" through the rainbow
    }
  }

  // send the 'leds' array out to the actual LED strip
  FastLED.show();

  // insert a delay to keep the framerate modest
  FastLED.delay(1000 / FRAMES_PER_SECOND);
  // delay(1000 / FRAMES_PER_SECOND);
}
