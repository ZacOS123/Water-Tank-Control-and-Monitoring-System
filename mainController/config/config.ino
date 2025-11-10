//This program is used to calibrate system parameters present in config.h
//upload to micrcontroller, check sensor values, then change the values of the parameters in config.h accordingly.
//When you're done, re-upload mainLogic.ino to the microcontroller and it should work.

#include "config.h"
unsigned long start_time;

void setup(){
  Serial.begin(9600);

  pinMode(SUP_TRIG_PIN, OUTPUT);
  pinMode(SUP_ECHO_PIN, INPUT);
}

void loop(){
  long raw_distance [MEASURE_NUM];
  float distance;

  Serial.print("\n\nLower tank Ultrasonic Trigger is connected to pin ");
  Serial.println(INF_TRIG_PIN);
  Serial.print("\nLower tank Ultrasonic ECHO is connected to pin ");
  Serial.println(INF_ECHO_PIN);
  delay(3000);
  Serial.println("INVERTED Sensor values will be shown for 10 seconds. Empty = SUP_TANK_HI:");
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
    //set pin to LOW
    digitalWrite(INF_TRIG_PIN, LOW); 
    delayMicroseconds(2);
    distance = 0;
    //Send trigger [MEASURE_NUM] times
      for(int i=0; i<MEASURE_NUM; i++){
        digitalWrite(INF_TRIG_PIN, HIGH);
        delayMicroseconds(10);
        digitalWrite(INF_TRIG_PIN, LOW);

        raw_distance[i] = (pulseIn(INF_ECHO_PIN, HIGH) * 0.0343)/2;
      }
      for(int j=0; j<MEASURE_NUM; j++){
        distance = distance + raw_distance[j];
                
      }

    Serial.print(distance/MEASURE_NUM);
    Serial.print("\n");
    delay(300);
  }
/*
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
  }*/
}
