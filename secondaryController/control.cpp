#include "control.h"
#include "globalVariables.h"
#include "config/config.h"



/////note: level means the air not water level, due to ultrasonic inverted measurment. ////

int get_level_inf(){    //returns air level in inferior tank. Return -1 for sensor error

  long raw_distance [MEASURE_NUM];
  int distance;

 //set pin to LOW
  digitalWrite(INF_TRIG_PIN, LOW); 
  delayMicroseconds(2);
  distance = 0;

  //Send trigger [MEASURE_NUM] times

  for(int i=0; i<MEASURE_NUM; i++){
    
    digitalWrite(INF_TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(INF_TRIG_PIN, LOW);

    raw_distance[i] = (pulseIn(INF_ECHO_PIN, HIGH, 50000) * 0.0343)/2;
    delay(100);
  }

  for(int j=0; j<MEASURE_NUM; j++){
    distance = distance + raw_distance[j];            
  }

  distance = distance/MEASURE_NUM;

  //error check
  if(distance > INF_SENSOR_HI || distance < INF_SENSOR_LO){
    return -1;
  }
  else{
    return distance; //distance from sensor to water surface
  }
}

///////////////////////////////////////////////////////////////////

void start_source(){      //Turns on the pump to fill upper tank
  digitalWrite(SOURCE_PIN, HIGH);
}

///////////////////////////////////////////////////////////////////

void stop_source(){   //turns of the pump
  digitalWrite(SOURCE_PIN, LOW);
} 

///////////////////////////////////////////////////////////////////

int check_source(){      //Checks if lower tank is being filled. Returns 0 for "not yet", -1 for error, 1 for "working properly"
  if(status != FILLING){ //if not filling
    inf_level_at_start = 0;   //reset level
    source_time = 0;          //reset timer
    return 0;
  }
  else{  //if started filling
    if (previous_status != FILLING){
      source_time = millis();
      inf_level_at_start = inf_current_level;
    }
  }

  if(millis() - source_time >= TIME_TO_CHECK){ //if TIME_TO_CHECK passed
    if (inf_current_level == -1){  //In case get_level had errors
      return 0;
    }
    if(inf_current_level >= inf_level_at_start){  //if level not increased
      source_time = millis();
      inf_level_at_start = inf_current_level;
      return -1;
    }
    else{   //if level increased
      source_time = millis();
      inf_level_at_start = inf_current_level;
      return 1;
    }
  }
  else{      //if TIME_TO_CHECK not passed
   return 0; //does nothing if it has not elapsed TIME_TO_CHECK
  }
}

///////////////////////////////////////////////////



