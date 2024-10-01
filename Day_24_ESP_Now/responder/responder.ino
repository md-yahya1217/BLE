
// Include Libraries
#include <esp_now.h>
#include <WiFi.h>
#define ledpin 18
// Define a data structure
typedef struct struct_message {
  char a[32];
  int b;
  float c;
  //bool d;
} struct_message;

// Create a structured object
struct_message myData;


// Callback function executed when data is received
void OnDataRecv(const esp_now_recv_info *info, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Data received: ");
  Serial.println(len);
  Serial.print("Character Value: ");
  Serial.println(myData.a);
  Serial.print("Integer Value: ");
  Serial.println(myData.b);
  Serial.print("Float Value: ");
  Serial.println(myData.c);
 // Serial.print("Boolean Value: ");
  //Serial.println(myData.d);
  Serial.println();
}

void setup() {
  // Set up Serial Monitor
  Serial.begin(115200);
  
  // Set ESP32 as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  pinMode( ledpin, OUTPUT);
  // Initilize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Register callback function
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {
  int pwm = map(myData.b,0, 4095, 0, 100);
  analogWrite(ledpin, pwm);
}