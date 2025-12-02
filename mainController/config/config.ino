//This program is used to calibrate system parameters present in config.h
//upload to micrcontroller, check sensor values, then change the values of the parameters in config.h accordingly.
//When you're done, re-upload mainLogic.ino to the microcontroller and it should work.

#include "config.h"
#include <Arduino.h>

int ledPin = 27; // any GPIO
int ledChannel = 0;
int resolution = 8; // 8-bit resolution: 0-255

void setup(){
  Serial.begin(9600);

  pinMode(SUP_TRIG_PIN, OUTPUT);
  pinMode(SUP_ECHO_PIN, INPUT);

  //ledcAttach(ledPin, 5000, resolution);
}

void loop(){
  long raw_distance [MEASURE_NUM];
  float distance;
  //ledcWrite(ledPin, 255);
  Serial.print("\n\nLower tank Ultrasonic Trigger is connected to pin ");
  Serial.println(SUP_TRIG_PIN);
  Serial.print("\nLower tank Ultrasonic ECHO is connected to pin ");
  Serial.println(SUP_ECHO_PIN);
  delay(3000);
  Serial.println("Sensor values will be shown starting in: ");
  delay(1000);
  Serial.print("1");
  delay(1000);
  Serial.print("  2");
  delay(1000);
  Serial.print("   3\n\n");
  delay(1000);

  while(1){
    //set pin to LOW
    digitalWrite(SUP_TRIG_PIN, LOW); 
    delayMicroseconds(2);
    distance = 0;
    //Send trigger [MEASURE_NUM] times
      for(int i=0; i<MEASURE_NUM; i++){
        digitalWrite(SUP_TRIG_PIN, HIGH);
        delayMicroseconds(10);
        digitalWrite(SUP_TRIG_PIN, LOW);

        raw_distance[i] = (pulseIn(SUP_ECHO_PIN, HIGH) * 0.0343)/2;
      }
      for(int j=0; j<MEASURE_NUM; j++){
        distance = distance + raw_distance[j];
                
      }

    Serial.print(distance/MEASURE_NUM);
    Serial.print("\n");
   // ledcWrite(ledPin, distance/MEASURE_NUM);

    delay(300);
  }

}
