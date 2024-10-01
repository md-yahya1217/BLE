#include <WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>

#define ledpin 18

const char* ssid = "Revive Lab";
const char* password = "Revive@123";
const char* brokeruser = "muhammadyahya1217@gmail.com";
const char* brokerpass = "scienta480";
const char* broker = "broker.emqx.io";

char* intopic = "/ledbrightness/in";

WiFiClient espClient;
PubSubClient client(espClient);

void setupwifi() {
  delay(100);
  Serial.print("\nconnecting to...");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print("---");
  }
  Serial.print("\nconnected to--");
  Serial.print(ssid);
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("\nconnecting to--");
    Serial.println(broker);
    if (client.connect("koikoikoi", brokeruser, brokerpass)) {
      Serial.print("\nconnected to--");
      Serial.println(broker);
      client.subscribe(intopic);
    } else {
      Serial.print("\nConnection Error --Try again");
      delay(200);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  String message = "";
  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  int value = message.toInt();
  Serial.print("Received message: ");
  Serial.print(topic);
  Serial.print(" with value: ");
  Serial.println(value);
  analogWrite(ledpin, value);
}

void setup() {
  Serial.begin(115200);
  pinMode(ledpin, OUTPUT);
  setupwifi();
  client.setServer(broker, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
