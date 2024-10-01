#include <Nextion.h>

// Initialize counter, state variables, and timing
int counter = 0;
bool isCounting = false;
unsigned long lastUpdateTime = 0;
const unsigned long updateInterval = 1000; // Update every second

// Declare button and text field objects (ensure IDs and names match your Nextion project)
NexButton b0 = NexButton(0, 1, "b0"); // Page 0, ID 1, name "b0"
NexButton b1 = NexButton(0, 2, "b1"); // Page 0, ID 2, name "b1"
NexText t0 = NexText(0, 3, "t0"); // Page 0, ID 3, name "t0"

// Array of all NexTouch objects
NexTouch *nex_listen_list[] = {
    &b0,
    &b1,
    NULL // Terminate the array with NULL
};

// Function to handle button 0 press
void b0PushCallback(void *ptr)
{
    isCounting = true;
}

// Function to handle button 1 press
void b1PushCallback(void *ptr)
{
    isCounting = false;
}

// Function to update the counter value on the Nextion display
void updateCounterDisplay()
{
    char buffer[1000];
    sprintf(buffer, "t0.txt=\"%d\"", counter);
    sendToNextion(buffer);
}

void setup(void)
{
    Serial2.begin(9600, SERIAL_8N1, 16, 17);
    delay(100);
    // Register button push callbacks
    b0.attachPush(b0PushCallback);
    b1.attachPush(b1PushCallback);

    // Send a simple text message to the Nextion display
    sendToNextion("t0.txt=\"Counter: 0\""); // Initialize with a starting message
}

void sendToNextion(String command) {
  Serial2.print(command);
  Serial2.write(0xFF);  // End of command
  Serial2.write(0xFF);
  Serial2.write(0xFF);
}

void loop(void)
{
    // Listen for button events
    nexLoop(nex_listen_list);

    // Update the counter if counting is active
    if (isCounting)
    {
        unsigned long currentTime = millis();
        if (currentTime - lastUpdateTime >= updateInterval)
        {
            lastUpdateTime = currentTime;
            counter++;
            updateCounterDisplay();
        }
    }
}
