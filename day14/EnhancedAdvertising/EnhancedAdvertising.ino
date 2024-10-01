#include <Insights.h>

#include <ArduinoBLE.h>

BLEService myService("fff0");
BLEIntCharacteristic myCharacteristic("fff1", BLERead | 
                                              BLEBroadcast |
                                              BLEWrite);

const uint8_t manufactData[4] = {0x01, 0x02, 0x03, 0x04};
uint8_t y=0;
uint8_t serviceData[3];

void setup() {
  Serial.begin(115200);
  while (!Serial);

  if (!BLE.begin()) {
    Serial.println("failed to initialize BLE!");
    while (1);
  }
 manufactData[0] = y;
  y++;
  manufactData[1] = y;
  y++;
  manufactData[2] = y;
  y++;
  manufactData[3] = y;
  y++;

  for(int i=0; i<4; i++){
    Serial.println(manufactData[i]);
    delay(1000);
  }
  BLE.setLocalName("TestDevice"); // Set the device name for the advertisement
  myService.addCharacteristic(myCharacteristic);
  BLE.addService(myService);

  BLE.setManufacturerData(manufactData, sizeof(manufactData));
  BLE.setAdvertisedService(myService);
  BLE.setAdvertisedServiceData(myService, serviceData, sizeof(serviceData));

  BLE.advertise();
  Serial.println("advertising ...");
}

void loop() {
  BLE.poll();
 delay(1000);
  
}
