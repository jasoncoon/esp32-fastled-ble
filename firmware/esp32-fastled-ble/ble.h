#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

BLEServer* pServer = NULL;

BLECharacteristic* pPowerCharacteristic = NULL;
BLECharacteristic* pBrightnessCharacteristic = NULL;
BLECharacteristic* pColorCharacteristic = NULL;
BLECharacteristic* pPatternCharacteristic = NULL;
BLECharacteristic* pPatternCountCharacteristic = NULL;
BLECharacteristic* pPaletteCharacteristic = NULL;
BLECharacteristic* pPaletteCountCharacteristic = NULL;

bool deviceConnected = false;
bool oldDeviceConnected = false;
//uint32_t value = 0;

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

class ServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

class PowerCharacteristicCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *characteristic) {
      std::string value = characteristic->getValue();

      if (value.length() > 0) {
        Serial.print("New power: ");

        if (value.length() == 1) {
          power = value[0];
          if (power > 1) power = 1;
          Serial.println(power);
        }
      }
    }
};

class BrightnessCharacteristicCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *characteristic) {
      std::string value = characteristic->getValue();

      if (value.length() > 0) {
        Serial.print("New brightness: ");

        if (value.length() == 1) {
          brightness = value[0];
          Serial.println(brightness);
          FastLED.setBrightness(brightness);
        }
      }
    }
};

class ColorCharacteristicCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *characteristic) {
      std::string value = characteristic->getValue();

      if (value.length() > 0) {
        Serial.println("*********");
        Serial.print("New color: ");

        for (int i = 0; i < value.length(); i++) {
          Serial.print((uint8_t)value[i]);
          if (i < value.length() - 1) Serial.print(",");
        }

        if (value.length() == 3) {
          solidColor.r = value[0];
          solidColor.g = value[1];
          solidColor.b = value[2];
        }

        Serial.println();
        Serial.println("*********");
      }
    }
};

class PatternCharacteristicCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *characteristic) {
      std::string value = characteristic->getValue();

      if (value.length() > 0) {
        Serial.print("New pattern: ");

        if (value.length() == 1) {
          currentPatternIndex = value[0];
          if (currentPatternIndex >= patternCount) currentPatternIndex = patternCount - 1;
          Serial.println(currentPatternIndex);
        }
      }
    }
};

class PaletteCharacteristicCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *characteristic) {
      std::string value = characteristic->getValue();

      if (value.length() > 0) {
        Serial.print("New palette: ");

        if (value.length() == 1) {
          currentPaletteIndex = value[0];
          if (currentPaletteIndex >= paletteCount) currentPaletteIndex = paletteCount - 1;
          targetPalette = palettes[currentPaletteIndex];
          Serial.println(currentPaletteIndex);
        }
      }
    }
};

void setupBLE() {
  // Create the BLE Device
  BLEDevice::init("ESP32");

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new ServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(BLEUUID("8bc01404-b072-413b-881e-6ca27b3f630d"), 30);

  // Create BLE Characteristics

  // Power
  pPowerCharacteristic = pService->createCharacteristic(
                                "c7df1272-99f7-4059-8a35-ca03831f2897",
                                BLECharacteristic::PROPERTY_READ   |
                                BLECharacteristic::PROPERTY_WRITE  |
                                BLECharacteristic::PROPERTY_NOTIFY
                              );
  pPowerCharacteristic->setCallbacks(new PowerCharacteristicCallbacks());
  pPowerCharacteristic->setValue(&power, 1);
  pPowerCharacteristic->addDescriptor(new BLE2902());

  // Brightness
  pBrightnessCharacteristic = pService->createCharacteristic(
                                "eb817a17-301d-49cd-8048-b4840ec6d40c",
                                BLECharacteristic::PROPERTY_READ   |
                                BLECharacteristic::PROPERTY_WRITE  |
                                BLECharacteristic::PROPERTY_NOTIFY
                              );
  pBrightnessCharacteristic->setCallbacks(new BrightnessCharacteristicCallbacks());
  pBrightnessCharacteristic->setValue(&brightness, 1);
  pBrightnessCharacteristic->addDescriptor(new BLE2902());

  // Color
  pColorCharacteristic = pService->createCharacteristic(
                           "5d35f786-8104-4646-b6cd-ef92ea953069",
                           BLECharacteristic::PROPERTY_READ   |
                           BLECharacteristic::PROPERTY_WRITE  |
                           BLECharacteristic::PROPERTY_NOTIFY
                         );
  pColorCharacteristic->setCallbacks(new ColorCharacteristicCallbacks());
  pColorCharacteristic->setValue(solidColor.raw, 3);
  pColorCharacteristic->addDescriptor(new BLE2902());

  // Pattern
  pPatternCharacteristic = pService->createCharacteristic(
                             "027db9ce-6d4a-48bb-876e-d4044aea8539",
                             BLECharacteristic::PROPERTY_READ   |
                             BLECharacteristic::PROPERTY_WRITE  |
                             BLECharacteristic::PROPERTY_NOTIFY
                           );
  pPatternCharacteristic->setCallbacks(new PatternCharacteristicCallbacks());
  pPatternCharacteristic->setValue(&currentPatternIndex, 1);
  pPatternCharacteristic->addDescriptor(new BLE2902());

  // Pattern Count
  pPatternCountCharacteristic = pService->createCharacteristic(
                             "5c888a91-2033-45cb-8932-92f7a6538869",
                             BLECharacteristic::PROPERTY_READ
                           );
  pPatternCountCharacteristic->setValue(&patternCount, 1);
  pPatternCountCharacteristic->addDescriptor(new BLE2902());

  // Palette
  pPaletteCharacteristic = pService->createCharacteristic(
                             "a2f47ab8-a385-456e-bd29-60f29a6fffed",
                             BLECharacteristic::PROPERTY_READ   |
                             BLECharacteristic::PROPERTY_WRITE  |
                             BLECharacteristic::PROPERTY_NOTIFY
                           );
  pPaletteCharacteristic->setCallbacks(new PaletteCharacteristicCallbacks());
  pPaletteCharacteristic->setValue(&currentPaletteIndex, 1);
  pPaletteCharacteristic->addDescriptor(new BLE2902());

  // Palette Count
  pPaletteCountCharacteristic = pService->createCharacteristic(
                             "692cc883-c9d1-458d-bf8d-c3c760dd7808",
                             BLECharacteristic::PROPERTY_READ
                           );
  pPaletteCountCharacteristic->setValue(&paletteCount, 1);
  pPaletteCountCharacteristic->addDescriptor(new BLE2902());

  // Start the service
  pService->start();

  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID("8bc01404-b072-413b-881e-6ca27b3f630d");
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
  BLEDevice::startAdvertising();
  Serial.println("Waiting for a client connection to notify...");
}
