// Need to add this include
#include <driver/adc.h>

#define analogInPin  1 //1 and 13 are working fine
int sensorValue; // Analog output of the sensor
float referenceVoltage = 3.3;
 int adcMaxValue = 4095;
float calibration = 0.36; // Adjust this value based on multimeter readings
int bat_percentage;

void setup() {
  Serial.begin(115200);
}
void loop() {
  sensorValue =  analogRead(analogInPin);
  float voltage = ((sensorValue * referenceVoltage) / adcMaxValue); // Adjust calculation as per your voltage divider and calibration
 
  bat_percentage = mapfloat(voltage, 2.8, 4.2, 0, 100); // 2.8V as Battery Cut off Voltage & 4.2V as Maximum Voltage

  if (bat_percentage >= 100) {
    bat_percentage = 100;
  }
  if (bat_percentage <= 0) {
    bat_percentage = 1;
  }
  Serial.print("Analog Value = ");
  Serial.print(sensorValue);

  Serial.print("\t Output Voltage = ");
  Serial.print(voltage);
  Serial.print("\t Battery Percentage = ");
  Serial.println(bat_percentage);
  delay(1000);
}

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
