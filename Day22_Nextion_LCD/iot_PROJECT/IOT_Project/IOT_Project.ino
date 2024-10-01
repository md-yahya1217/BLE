#include <Nextion.h>
#include "DHT.h"

// Initialize the State variables and DHT sensor pins
#define DPIN 22
#define DTYPE DHT11
DHT dht(DPIN, DTYPE);
float tc;
float hu;

// Declare button and text field objects for page 0.
NexButton page0_Next_b1 = NexButton(0, 3, "b1"); // Page 0, ID 3, name "b1"
// Declare button and text field objects for page 1.
NexText t0 = NexText(1, 4, "t0");
NexText t1 = NexText(1, 5, "t1");
NexDSButton bt0 = NexDSButton(1, 6, "bt0"); // Declare bt0 correctly as NexDSButton

// Array of all NexTouch objects
NexTouch *nex_listen_list[] = {
    &page0_Next_b1,
    &t0,
    &t1,
    &bt0,
    NULL // Terminate the array with NULL
};

bool displayData = false; // Variable to track the display state

void bt0_pushcallback(void *ptr) {
    uint32_t state;
    bt0.getValue(&state); // Get the state of the dual-state button

    if (state == 1) {
        displayData = true; // Start displaying data
    } else {
        displayData = false; // Stop displaying data
        sendToNextion("t0.txt=\"\"", "t1.txt=\"\""); // Clear the display
    }
}

void setup(void) {
    Serial2.begin(9600, SERIAL_8N1, 16, 17);
    dht.begin();
    bt0.attachPush(bt0_pushcallback); // Attach the correct callback
    sendToNextion("t0.txt=\"0\"", "t1.txt=\"0\"");
}

void sendToNextion(String command1, String command2) {
    Serial2.print(command1);
    Serial2.write(0xFF);
    Serial2.write(0xFF);
    Serial2.write(0xFF);
    Serial2.print(command2);
    Serial2.write(0xFF);
    Serial2.write(0xFF);
    Serial2.write(0xFF);
}

void loop(void) {
    nexLoop(nex_listen_list);

    if (displayData) {
        tc = dht.readTemperature(false); // Read temperature in C
        hu = dht.readHumidity();         // Read Humidity
        char temp[1000];
        char humid[1000];
        sprintf(temp, "t0.txt=\"%.1f\"", tc);
        sprintf(humid, "t1.txt=\"%.1f\"", hu);
        sendToNextion(temp, humid);
    }

    delay(2000); // Delay between readings
}
