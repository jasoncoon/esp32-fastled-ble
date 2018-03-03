/*
   ESP32 FastLED BLE: https://github.com/jasoncoon/esp32-fastled-ble
   Copyright (C) 2018 Jason Coon

   Built upon the amazing FastLED work of Daniel Garcia and Mark Kriegsman:
   https://github.com/FastLED/FastLED

   ESP32 support provided by the hard work of Sam Guyer:
   https://github.com/samguyer/FastLED

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

String getPower() {
  return String(power);
}

String setPower(String value) {
  power = value.toInt();
  power = power == 0 ? 0 : 1;
  return String(power);
}

String getBrightness() {
  return String(brightness);
}

String setBrightness(String value) {
  brightness = value.toInt();
  FastLED.setBrightness(brightness);
  return String(brightness);
}

String getPattern() {
  return String(currentPatternIndex);
}

void setPattern(uint8_t value)
{
  if (value >= patternCount)
    value = patternCount - 1;

  currentPatternIndex = value;
}

String setPattern(String value) {
  setPattern(value.toInt());
  return String(currentPatternIndex);
}

String getPatterns() {
  String json = "";

  for (uint8_t i = 0; i < patternCount; i++) {
    json += "\"" + patterns[i].name + "\"";
    if (i < patternCount - 1)
      json += ",";
  }

  return json;
}

String getPalette() {
  return String(currentPaletteIndex);
}

String setPalette(String value) {
  currentPaletteIndex = value.toInt();
  if (currentPaletteIndex < 0) currentPaletteIndex = 0;
  else if (currentPaletteIndex >= paletteCount) currentPaletteIndex = paletteCount - 1;
  targetPalette = palettes[currentPaletteIndex];
  return String(currentPaletteIndex);
}

String getPalettes() {
  String json = "";

  for (uint8_t i = 0; i < paletteCount; i++) {
    json += "\"" + paletteNames[i] + "\"";
    if (i < paletteCount - 1)
      json += ",";
  }

  return json;
}

String getSpeed() {
  return String(speed);
}

String setSpeed(String value) {
  speed = value.toInt();
  return String(speed);
}

String getAutoplay() {
  return String(autoplay);
}

String setAutoplay(String value) {
  autoplay = value.toInt();
  autoplay = autoplay == 0 ? 0 : 1;
  autoPlayTimeout = millis() + (autoplayDuration * 1000);
  return String(autoplay);
}

String getAutoplayDuration() {
  return String(autoplayDuration);
}

String setAutoplayDuration(String value) {
  autoplayDuration = value.toInt();
  if (autoplayDuration < 1) autoplayDuration = 1;
  else if (autoplayDuration > 255) autoplayDuration = 255;
  autoPlayTimeout = millis() + (autoplayDuration * 1000);
  return String(autoplayDuration);
}

String getCyclePalettes() {
  return String(cyclePalettes);
}

String setCyclePalettes(String value) {
  cyclePalettes = value.toInt();
  cyclePalettes = cyclePalettes == 0 ? 0 : 1;
  paletteTimeout = millis() + (paletteDuration * 1000);
  return String(cyclePalettes);
}

String getPaletteDuration() {
  return String(paletteDuration);
}

String setPaletteDuration(String value) {
  paletteDuration = value.toInt();
  if (paletteDuration < 1) paletteDuration = 1;
  else if (paletteDuration > 255) paletteDuration = 255;
  paletteTimeout = millis() + (paletteDuration * 1000);
  return String(paletteDuration);
}

String getSolidColor() {
  return String(solidColor.r) + "," + String(solidColor.g) + "," + String(solidColor.b);
}

String setSolidColor(uint8_t r, uint8_t g, uint8_t b)
{
  solidColor = CRGB(r, g, b);

  return "\"" + String(solidColor.r) + "," + String(solidColor.g) + "," + String(solidColor.b) + "\"";
}

String setSolidColor(CRGB color) {
  return setSolidColor(color.r, color.g, color.b);
}

String setSolidColor(String value) {
  CRGB color = parseColor(value);

  return setSolidColor(color);
}

String getCooling() {
  return String(cooling);
}

String setCooling(String value) {
  cooling = value.toInt();
  return String(cooling);
}

String getSparking() {
  return String(sparking);
}

String setSparking(String value) {
  sparking = value.toInt();
  return String(sparking);
}

String getTwinkleSpeed() {
  return String(twinkleSpeed);
}

String setTwinkleSpeed(String value) {
  twinkleSpeed = value.toInt();
  if (twinkleSpeed < 0) twinkleSpeed = 0;
  else if (twinkleSpeed > 8) twinkleSpeed = 8;
  return String(twinkleSpeed);
}

String getTwinkleDensity() {
  return String(twinkleDensity);
}

String setTwinkleDensity(String value) {
  twinkleDensity = value.toInt();
  if (twinkleDensity < 0) twinkleDensity = 0;
  else if (twinkleDensity > 8) twinkleDensity = 8;
  return String(twinkleDensity);
}

FieldList fields = {
  { "a8c27e11-bf97-40a5-8438-3631bb75a7e1", "power", "Power", BooleanFieldType, 0, 1, getPower, NULL, setPower },
  { "672e2d71-af3c-407a-a7dc-c9b7beb229f2", "brightness", "Brightness", NumberFieldType, 1, 255, getBrightness, NULL, setBrightness },
  { "7ac75fb4-33ef-45e1-b63e-e399b1c2accb", "speed", "Speed", NumberFieldType, 1, 255, getSpeed, NULL, setSpeed },

//  { "3dbf8d76-f9c9-4415-ab56-df246ea56e56", "patternSection", "Pattern", SectionFieldType },
  { "75c34563-a7ba-4ff3-8d9f-832fa1cd5a0e", "pattern", "Pattern", SelectFieldType, 0, patternCount, getPattern, getPatterns, setPattern },
  { "55aae8b1-2958-4ff4-bfef-38949fe3a38d", "autoplay", "Cycle Patterns", BooleanFieldType, 0, 1, getAutoplay, NULL, setAutoplay },
  { "3d78f086-ed5d-4d72-871b-874ba8b84e35", "autoplayDuration", "Pattern Duration", NumberFieldType, 1, 255, getAutoplayDuration, NULL, setAutoplayDuration },

//  { "b202dff9-fbd4-48ca-b0a3-b7f0c1272aff", "paletteSection", "Palette", SectionFieldType },
  { "4ea8ae43-f99c-46b9-aec4-749673fdb835", "palette", "Palette", SelectFieldType, 0, paletteCount, getPalette, getPalettes, setPalette },
  { "b5d64d7f-a5d1-4f61-bf1c-89152fcc17d3", "cyclePalettes", "Cycle Palettes", BooleanFieldType, 0, 1, getCyclePalettes, NULL, setCyclePalettes },
  { "8b291198-8141-49db-baa1-620cfdf7ca49", "paletteDuration", "Palette Duration", NumberFieldType, 1, 255, getPaletteDuration, NULL, setPaletteDuration },

//  { "42b27fcf-ce35-4db0-94b2-65c0de4f07ae", "solidColorSection", "Solid Color", SectionFieldType },
  { "fd8fd729-d189-413a-8cf3-53395931cae4", "solidColor", "Color", ColorFieldType, 0, 255, getSolidColor, NULL, setSolidColor },

//  { "e9794f5e-cc87-4227-846c-bf936da2cfa0", "fire", "Fire & Water", SectionFieldType },
  { "77f52711-c619-4b4d-a180-8b18e812a15c", "cooling", "Cooling", NumberFieldType, 0, 255, getCooling, NULL, setCooling },
  { "bb2dbf79-081c-4895-9422-2e6638aeb912", "sparking", "Sparking", NumberFieldType, 0, 255, getSparking, NULL, setSparking },

//  { "79583572-8406-426f-9efe-d34837976320", "twinklesSection", "Twinkles", SectionFieldType },
  { "f1923e5f-a78f-4418-95ce-ee3f4058069b", "twinkleSpeed", "Twinkle Speed", NumberFieldType, 0, 8, getTwinkleSpeed, NULL, setTwinkleSpeed },
  { "018abf88-a753-4d57-b920-24c39042149d", "twinkleDensity", "Twinkle Density", NumberFieldType, 0, 8, getTwinkleDensity, NULL, setTwinkleDensity },
};

uint8_t fieldCount = ARRAY_SIZE(fields);
