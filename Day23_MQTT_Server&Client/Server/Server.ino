#include <WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>
#include <DHT.h>

#define DPIN 19
#define DTYPE DHT11
DHT dht(DPIN, DTYPE);

const char* ssid = "Revive Lab";
const char* password = "Revive@123";
const char* brokeruser = "muhammadyahya1217@gmail.com";
const char* brokerpass = "scienta480";
const char* broker = "broker.emqx.io";

int count = 0;
char* outtopic1 = "/muhammadyahya1217@gmail.com/out";
char* outtopic2 = "/temperature/out";
char* outtopic3 = "/humidity/out";
long currenttime;
long lasttime;
char messages1[50], messages2[50], messages3[50];

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
    } else {
      Serial.print("\nConnection Error --Try again");
      delay(200);
    }
  }
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  setupwifi();
  client.setServer(broker, 1883);
}

void loop() {
  int tc = dht.readTemperature(false);
  int hu = dht.readHumidity();

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  
  currenttime = millis();
  if (currenttime - lasttime > 2000) {
    count++;
    snprintf(messages1, 75, "count: %d", count);
    Serial.print("Sending message 1--");
    Serial.println(messages1);
    client.publish(outtopic1, messages1);
    lasttime = millis();

    snprintf(messages2, 75, "Temperature: %d", tc);
    Serial.print("Sending message 2 --");
    Serial.println(messages2);
    client.publish(outtopic2, messages2);

    snprintf(messages3, 75, "Humidity: %d", hu);
    Serial.print("Sending message 3 --");
    Serial.println(messages3);
    client.publish(outtopic3, messages3);
  }
}
