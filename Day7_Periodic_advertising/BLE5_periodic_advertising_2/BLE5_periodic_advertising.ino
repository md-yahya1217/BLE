#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// Define deep sleep time (in microseconds)
#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  10        /* Time ESP32 will go to sleep (in seconds) */

// Define the service and characteristic UUIDs
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

// Pin where the sensor is connected
const int sensorPin = 1;

BLECharacteristic *pCharacteristic;
float sensorValue = 0.0;
float previousSensorValue = 0.0;
unsigned long previousMillis = 0;
const long interval = 10000;  // 60 seconds

void setup() {
  Serial.begin(115200);

  // Initialize BLE
  BLEDevice::init("ESP32_Sensor");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ |
                      BLECharacteristic::PROPERTY_NOTIFY
                    );
  pCharacteristic->addDescriptor(new BLE2902());

  pService->start();
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.println("Waiting for a client connection to notify...");

  // Configure the ESP32 to wake up every TIME_TO_SLEEP seconds
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);

  // Set the initial time
  previousMillis = millis();
}

void loop() {
  // Read the sensor value
  sensorValue = analogRead(sensorPin);
  sensorValue = map(sensorValue,0,4095,0,255);
  Serial.print("Sensor Value: ");
  Serial.println(sensorValue);

  // Check if the sensor value has changed
  if (sensorValue != previousSensorValue) {
    // Convert sensor value to a string
    char sensorString[8];
    dtostrf(sensorValue, 1, 2, sensorString);

    // Update the characteristic value
    pCharacteristic->setValue(sensorString);
    pCharacteristic->notify();
    Serial.println("Characteristic updated and notified.");

    // Reset the timer
    previousMillis = millis();
    previousSensorValue = sensorValue;
  }

  // Check if 60 seconds have passed since the last change
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    // Go to deep sleep
    Serial.println("No change in sensor value for 60 seconds. Going to sleep now...");
    delay(1000);
    Serial.flush();
    esp_deep_sleep_start();
  }

  // Wait for a short while before checking the sensor again
  delay(1000);
}
