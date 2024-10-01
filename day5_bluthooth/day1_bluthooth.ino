#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

#include <BluetoothSerial.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Bluetooth not available or not enabled. It is only available for the ESP32 chip.
#endif

BluetoothSerial SerialBT;
String device_name = "ESP32-BT-Client";
#define BT_DISCOVER_TIME	10000
static bool btScanSync = true;


void btAdvertisedDeviceFound(BTAdvertisedDevice* pDevice) {
	Serial.printf("Found a device asynchronously: %s\n", pDevice->toString().c_str());
}

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32test"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
  
  if (btScanSync) {
    Serial.println("Starting discover...");
    BTScanResults *pResults = SerialBT.discover(BT_DISCOVER_TIME);
    if (pResults)
      pResults->dump(&Serial);
    else
      Serial.println("Error on BT Scan, no result!");
  }
   // Print to serial monitor.
  Serial.printf("The device \"%s\" is started and can be paired with Bluetooth.\n", device_name.c_str());
}

void loop() {
  // read the input on analog pin 0:
  const int analogue= 34;
  int sensorValue = analogRead(analogue);
  
  // print out the value you read:
 // Serial.println(sensorValue);
  delay(100);  // delay in between reads for stability
 String a;
  a = sensorValue;
  if(isnan(sensorValue)){
    Serial.println("failed to read sensor value");
  }
  Serial.print("sensor value is:");
  Serial.println(a);
  SerialBT.print("sensor value is: ");
  SerialBT.println(a);
  delay(200);
}
