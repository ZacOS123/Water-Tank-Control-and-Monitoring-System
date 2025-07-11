//This program is used to calibrate system parameters present in config.h
//upload to micrcontroller, check sensor values, then change the values of the parameters in config.h accordingly.
//When you're done, re-upload mainLogic.ino to the microcontroller and it should work.

#include "config.h"
unsigned long start_time;

void setup(){
  Serial.begin(9600);
}

void loop(){
  Serial.print("\n\nLower tank sensor is connected to pin ");
  Serial.println(INF_SENSOR_PIN);
  delay(2000);
  Serial.println("Sensor values will be shown for 10 seconds:");
  Serial.print("Starting in:  ");
  delay(1000);
  Serial.print("1");
  delay(1000);
  Serial.print("  2");
  delay(1000);
  Serial.print("   3\n\n");
  delay(1000);
  start_time = millis();
  while(millis() - start_time < 10000){
    Serial.print(analogRead(INF_SENSOR_PIN));
    Serial.print("   ");
    delay(300);
  }

  Serial.print("\n\nUpper tank sensor is connected to pin ");
  Serial.println(SUP_SENSOR_PIN);
  delay(2000);
  Serial.println("Sensor values will be shown for 10 seconds:");
  Serial.print("Starting in:  ");
  delay(1000);
  Serial.print("1");
  delay(1000);
  Serial.print("  2");
  delay(1000);
  Serial.print("   3\n\n");
  delay(1000);
  start_time = millis();
  while(millis() - start_time < 10000){
    Serial.print(analogRead(SUP_SENSOR_PIN));
    Serial.print("   ");
    delay(300);
  }
}