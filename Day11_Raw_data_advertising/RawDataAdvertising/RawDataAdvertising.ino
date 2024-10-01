#include <ArduinoBLE.h>
#define analogpin 1

BLEService myService("fff0");
BLEIntCharacteristic myCharacteristic("fff1", BLERead | BLEBroadcast);
uint8_t Value = analogRead(analogpin);
  
    uint8_t sensorValue[2]={
    sensorValue[0] = (Value & 0xFF),     // Low byte
    sensorValue[1] = ((Value >> 8) & 0xFF) // High byte
    };
// Advertising parameters should have a global scope. Do NOT define them in 'setup' or in 'loop'
uint8_t completeRawAdvertisingData[] = {0x02,0x01,0x06,0x09,0xff,0x01,
                                              sensorValue[0],
                                              sensorValue[1]       
                                                 // High byte of 16-bit sensor value
    };

void setup() {
  Serial.begin(115200);
  while (!Serial);

  if (!BLE.begin()) {
    Serial.println("failed to initialize BLE!");
    while (1);
  }

  myService.addCharacteristic(myCharacteristic);
  BLE.addService(myService);

  // Build advertising data packet
  BLEAdvertisingData advData;
  // If a packet has a raw data parameter, then all the other parameters of the packet will be ignored
  advData.setRawData(completeRawAdvertisingData, sizeof(completeRawAdvertisingData));  
  // Copy set parameters in the actual advertising packet
  BLE.setAdvertisingData(advData);

  // Build scan response data packet
  BLEAdvertisingData scanData;
  scanData.setLocalName("Test advertising raw data");
  // Copy set parameters in the actual scan response packet
  BLE.setScanResponseData(scanData);
  
  BLE.advertise();

  Serial.println("advertising ...");
}

void loop() {
  
      BLE.poll();
}
