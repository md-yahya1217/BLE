#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define ADVERTISING_INTERVAL 10000 // Advertising interval in milliseconds

BLEAdvertising *pAdvertising;
int counter = 0;

void updateAdvertisingData() {
  // Create new advertising data
  BLEAdvertisementData advertisementData;

  // Increment counter
  counter++;

  // Convert counter to byte array
  uint8_t counterData[4];
  counterData[0] = (counter >> 24) & 0xFF;
  counterData[1] = (counter >> 16) & 0xFF;
  counterData[2] = (counter >> 8) & 0xFF;
  counterData[3] = counter & 0xFF;

  // Set the service UUID and the custom data
  advertisementData.setFlags(ESP_BLE_ADV_FLAG_GEN_DISC | ESP_BLE_ADV_FLAG_BREDR_NOT_SPT);
  advertisementData.setName("ESP32-C3");

  // Add manufacturer data (example manufacturer ID 0xFFFF)
  String manufacturerData;
  manufacturerData += (char)0xFF;  // Manufacturer ID byte 1
  manufacturerData += (char)0xFF;  // Manufacturer ID byte 2
  manufacturerData += String((char*)counterData, 4);  // Counter data

  advertisementData.setManufacturerData(manufacturerData);

  // Set the new advertising data
  pAdvertising->setAdvertisementData(advertisementData);
}

void setup() {
  Serial.begin(115200);

  // Initialize BLE
  BLEDevice::init("ESP32-C3");

  // Create BLE server
  BLEServer *pServer = BLEDevice::createServer();

  // Create BLE advertising
  pAdvertising = pServer->getAdvertising();

  // Set advertising parameters for non-connectable and non-scannable advertising
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinInterval(0x40);
  pAdvertising->setMaxInterval(0x40);
  pAdvertising->setAdvertisementType(ADV_TYPE_NONCONN_IND);

  // Update advertising data for the first time
  updateAdvertisingData();

  // Start advertising
  pAdvertising->start();
  Serial.println("Advertising started...");
}

void loop() {
  // Update advertising data at the defined interval
  static unsigned long lastAdvertisingUpdate = 0;
  if (millis() - lastAdvertisingUpdate > ADVERTISING_INTERVAL) {
    lastAdvertisingUpdate = millis();

    // Stop advertising
    pAdvertising->stop();

    // Update the advertising data
    updateAdvertisingData();

    // Start advertising again
    pAdvertising->start();
    Serial.print("Advertising counter value: ");
    Serial.println(counter);
  }

  delay(100); // Add a small delay to ensure the loop runs smoothly
}
