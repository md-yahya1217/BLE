/* *First initiallizing the wifi credentials and building connection with the wifi.
   *Then including the MQtt protocol library*

*/
#include <WiFi.h>
#include <WiFiClient.h>
#include "Arduino.h"
#include <PubSubClient.h>
#include <DHT.h>
#define DPIN 19       //Pin to connect DHT sensor (GPIO number)
#define DTYPE DHT11 
DHT dht(DPIN , DTYPE);
#define analogpin 1
#define ledpin 18
//adding wifi creditenials
const char* ssid = "Revive Lab";
const char* password = "Revive@123";
const char* brokeruser = "muhammadyahya1217@gmail.com";
const char* brokerpass = "scienta480";
const char* broker = "broker.emqx.io";
int count=0;
 char* outtopic1="/muhammadyahya1217@gmail.com/out";
 char* outtopic2="/temperature/out";
 char* outtopic3="/humidity/out";
 char* outtopic4="/PWM/out";
long currenttime;
long lasttime;
char messages1[50],messages2[50],messages3[50],messages4[50];

WiFiClient espClient ;
PubSubClient client(espClient);


void setupwifi(){
  delay(100);
  Serial.print("\nconnecting to...");
  Serial.println("ssid");
  WiFi.begin(ssid,password);
  while(WiFi.status() != WL_CONNECTED){
      delay(100);
      Serial.print("---");
  }

  Serial.print("\nconnected to--");
  Serial.print(ssid);

}
void reconnect(){
  while(!client.connected()){
    Serial.print("\nconnecting to--");
    Serial.println("broker");
    if(client.connect("koikoikoi",brokeruser,brokerpass)){
      Serial.print("\nconnected to--");
      Serial.println(broker);
    }else{
      Serial.print("\nConnection Error --Try again");
      delay(200);
    }
  }
}


void setcallback(){
    




}
void setup() {
  Serial.begin(115200);
  pinMode(ledpin, OUTPUT);
  dht.begin();
  setupwifi();
  client.setServer(broker, 1883);

}


void loop() {
  int SensorValue= analogRead(analogpin);
  int pwm = map(SensorValue,0, 4095, 0, 100);
  analogWrite(ledpin, pwm);
  int tc = dht.readTemperature(false);  //Read temperature in C
  int hu = dht.readHumidity();          //
  // put your main code here, to run repeatedly:
    if(!client.connected()){
      reconnect();
    }
    client.loop();
  currenttime = millis();
  if(currenttime - lasttime > 2000){
    count++;
    snprintf(messages1, 75, " count: %ld ",count);
    Serial.print("Sending messages 1--");
    Serial.println(messages1);
    client.publish(outtopic1, messages1);
    lasttime = millis();
    snprintf(messages2, 75, " Temperature: %ld ",tc);
    Serial.print("Sending messages 2 --");
    Serial.println(messages2);
    client.publish(outtopic2, messages2);
    lasttime= millis();

    snprintf(messages3, 75, " Humidity: %ld ",hu);
    Serial.print("Sending messages 3 --");
    Serial.println(messages3);
    client.publish(outtopic3, messages3);
    lasttime= millis();

    snprintf(messages4, 100, " PWM Value: %ld ",pwm);
    Serial.print("Sending messages 4 --");
    Serial.println(messages4);
    client.publish(outtopic4, messages4);
    lasttime= millis();
  }
}
