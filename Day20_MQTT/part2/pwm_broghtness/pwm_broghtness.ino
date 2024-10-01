#include "Arduino.h"
#define analogpin 1
#define ledpin 18

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(ledpin, OUTPUT);

}

void loop() {
  
  // put your main code here, to run repeatedly:
  int SensorValue= analogRead(analogpin);
  int pwm = map(SensorValue,0, 4095, 0, 100);
  analogWrite(ledpin, pwm);
  Serial.print("\nPWM value:");
  Serial.println(pwm);
  delay(200);
}
