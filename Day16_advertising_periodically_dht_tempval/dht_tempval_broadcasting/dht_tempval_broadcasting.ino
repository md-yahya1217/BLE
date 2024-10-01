#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <DHT.h>

#define ADVERTISING_INTERVAL 10000 // Advertising interval in milliseconds
#define DHTPIN 19                 // GPIO pin for DHT11 sensor
#define DHTTYPE DHT11             // DHT11 sensor type

BLEAdvertising *pAdvertising;
DHT dht(DHTPIN, DHTTYPE);        // Create DHT object
float humidity = 0.0;            // Variable to store humidity value

void updateAdvertisingData() {
  // Create new advertising data
  BLEAdvertisementData advertisementData;

  // Read humidity from DHT11 sensor
  humidity = dht.readHumidity();

  // Convert humidity to a string with decimal precision
  String humidityString = String(humidity, 2); // 1 decimal place

  // Convert the string to byte array
  uint8_t humidityData[humidityString.length() + 1];
  humidityString.getBytes(humidityData, humidityString.length() + 1);

  // Set the service UUID and the custom data
  advertisementData.setFlags(ESP_BLE_ADV_FLAG_GEN_DISC | ESP_BLE_ADV_FLAG_BREDR_NOT_SPT);
  advertisementData.setName("ESP32-C3");

  // Add manufacturer data (example manufacturer ID 0xFFFF) 
  String manufacturerData;
  manufacturerData += (char)0xFF;  // Manufacturer ID byte 1
  manufacturerData += (char)0xFF;  // Manufacturer ID byte 2
  manufacturerData += String((char*)humidityData, humidityString.length());  // Humidity data
  String manufacturerDatastr = manufacturerData;

  advertisementData.setManufacturerData(manufacturerDatastr.c_str());

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
  pAdvertising->setMinInterval(0x60);
  pAdvertising->setMaxInterval(0x60);
  pAdvertising->setAdvertisementType(ADV_TYPE_NONCONN_IND);

  // Initialize DHT sensor
  dht.begin();

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
    Serial.print("Advertising humidity value: ");
    Serial.println(humidity, 1); // Print humidity with 1 decimal place
  }

  delay(100); // Add a small delay to ensure the loop runs smoothly
}
