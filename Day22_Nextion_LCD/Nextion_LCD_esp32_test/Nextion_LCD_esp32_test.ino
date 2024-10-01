#include <Nextion.h>

// Create Nextion text object
NexText t0 = NexText(0, 1, "t0"); // Page 0, ID 1 , name "t0"

//creating list
NexTouch *nex_listen_list[] = {
    &t0,
    NULL
};

void setup() {
 // Serial.begin(115200);  // Initialize serial monitor
  Serial2.begin(115200, SERIAL_8N1, 16, 17);  // Initialize Serial2 for Nextion
  
  delay(500);  // Give some time for the Nextion display to initialize
  
  // Send text to a text box component with the ID "t0"
  sendToNextion("t0.txt=\"Hello world \"");
}

void sendToNextion(String command) {
  Serial2.print(command);
  Serial2.write(0xFF);  // End of command
  Serial2.write(0xFF);
  Serial2.write(0xFF);
  Serial.println("Text sent to Nextion display");
}

void loop() {
  // Nothing needed here for this simple example
}