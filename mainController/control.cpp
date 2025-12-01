#include "control.h"
#include "globalVariables.h"
#include "config/config.h"
#include <Arduino.h>

///// NOTE: level means the air not water level, due to ultrasonic inverted measurment. ////

int get_level_sup(){    //returns water level in superior tank. Return -1 for sensor after setting status

  long raw_distance [MEASURE_NUM];
  int distance;

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

  distance = distance/MEASURE_NUM;

  //error check
  if(distance > SUP_SENSOR_HI || distance < SUP_SENSOR_LO){
    return -1;
  }
  else{
    return distance;
  }
}

///////////////////////////////////////////////////////////////////

void start_pump(){      //Turns on the pump to fill upper tank
  digitalWrite(PUMP_PIN, HIGH);
}

///////////////////////////////////////////////////////////////////

void stop_pump(){   //turns of the pump
  digitalWrite(PUMP_PIN, LOW);
} 

///////////////////////////////////////////////////////////////////

int check_pumping(){      //Checks if upper tank is being filled. Returns 0 for "not yet", -1 for error, 1 for "working properly"
  if(status != PUMPING){ //if not pumping
    sup_level_at_start = 0;   //reset level
    pump_time = 0;          //reset timer
    return 0;
  }
  else{  //if pumping
    if (previous_status != PUMPING){
      pump_time = millis();
      sup_level_at_start = sup_current_level;
    }
  }

  if(millis() - pump_time >= TIME_TO_CHECK){ //if TIME_TO_CHECK passed
    if (sup_current_level == -1){  //In case get_level had errors
      return 0;
    }
    if(sup_current_level >= sup_level_at_start){  //if level not increased
      pump_time = millis();
      sup_level_at_start = sup_current_level;
      return -1;
    }
    else{   //if level increased
      pump_time = millis();
      sup_level_at_start = sup_current_level;
      return 1;
    }
  }
  else{      //if TIME_TO_CHECK not passed
   return 0; //does nothing if it has not elapsed TIME_TO_CHECK
  }
}
