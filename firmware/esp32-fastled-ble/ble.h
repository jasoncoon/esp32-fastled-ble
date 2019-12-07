#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

BLEServer* pServer = NULL;

BLECharacteristic* pBrightnessCharacteristic = NULL;
BLECharacteristic* pColorCharacteristic = NULL;
BLECharacteristic* pPatternCharacteristic = NULL;
BLECharacteristic* pPatternCountCharacteristic = NULL;

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


class PatternCountCharacteristicCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *characteristic) {
      std::string value = characteristic->getValue();

      if (value.length() > 0) {
        Serial.print("New pattern count: ");

        for (int i = 0; i < value.length(); i++) {
          Serial.print((uint8_t)value[i]);
          if (i < value.length() - 1) Serial.print(",");
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
  BLEService *pService = pServer->createService("8bc01404-b072-413b-881e-6ca27b3f630d");

  // Create BLE Characteristics

  // Color
  pColorCharacteristic = pService->createCharacteristic(
                           "5d35f786-8104-4646-b6cd-ef92ea953069",
                           BLECharacteristic::PROPERTY_READ   |
                           BLECharacteristic::PROPERTY_WRITE  |
                           BLECharacteristic::PROPERTY_NOTIFY |
                           BLECharacteristic::PROPERTY_INDICATE
                         );
  pColorCharacteristic->setCallbacks(new ColorCharacteristicCallbacks());
  pColorCharacteristic->setValue(solidColor.raw, 3);
  pColorCharacteristic->addDescriptor(new BLE2902());

  // Brightness
  pBrightnessCharacteristic = pService->createCharacteristic(
                                "eb817a17-301d-49cd-8048-b4840ec6d40c",
                                BLECharacteristic::PROPERTY_READ   |
                                BLECharacteristic::PROPERTY_WRITE  |
                                BLECharacteristic::PROPERTY_NOTIFY |
                                BLECharacteristic::PROPERTY_INDICATE
                              );
  pBrightnessCharacteristic->setCallbacks(new BrightnessCharacteristicCallbacks());
  pBrightnessCharacteristic->setValue(&brightness, 1);
  pBrightnessCharacteristic->addDescriptor(new BLE2902());

  // Pattern
  pPatternCharacteristic = pService->createCharacteristic(
                             "027db9ce-6d4a-48bb-876e-d4044aea8539",
                             BLECharacteristic::PROPERTY_READ   |
                             BLECharacteristic::PROPERTY_WRITE  |
                             BLECharacteristic::PROPERTY_NOTIFY |
                             BLECharacteristic::PROPERTY_INDICATE
                           );
  pPatternCharacteristic->setCallbacks(new PatternCharacteristicCallbacks());
  pPatternCharacteristic->setValue(&currentPatternIndex, 1);
  pPatternCharacteristic->addDescriptor(new BLE2902());

  // Pattern Count
  pPatternCountCharacteristic = pService->createCharacteristic(
                             "5c888a91-2033-45cb-8932-92f7a6538869",
                             BLECharacteristic::PROPERTY_READ   |
                             BLECharacteristic::PROPERTY_WRITE  |
                             BLECharacteristic::PROPERTY_NOTIFY |
                             BLECharacteristic::PROPERTY_INDICATE
                           );
  pPatternCountCharacteristic->setCallbacks(new PatternCountCharacteristicCallbacks());
  pPatternCountCharacteristic->setValue(&patternCount, 1);
  pPatternCountCharacteristic->addDescriptor(new BLE2902());

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
