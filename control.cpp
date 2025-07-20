#include "control.h"
#include "globalVariables.h"
#include "config/config.h"
#include <Arduino.h>


int get_level (int sensor_pin){    //returns water level in selected tank. Return -1 for sensor after setting status
  int value = analogRead(sensor_pin);  //possible feature: takes different values and calculates the average, for accurate readings...
  if (sensor_pin == INF_SENSOR_PIN && (value < INF_TANK_LO || value > INF_TANK_HI)){ //checks if sensor values are acceptable corresponding to the inf tank
    return -1;
  }
  else if(sensor_pin == SUP_SENSOR_PIN && (value < SUP_TANK_LO || value > SUP_TANK_HI)){ //checks if sensor values are acceptable corresponding to the sup tank
    return -1;
  }
  else{
    return value;
  }
}

////////////////  PUMP  //////////////////////////////////////////////////////////////////////////

void start_pump(){      //Turns on the pump to fill upper tank
  digitalWrite(PUMP_PIN, HIGH);
}

void stop_pump(){   //turns of the pump
  digitalWrite(PUMP_PIN, LOW);
} 

int check_pumping(){      //Checks if upper tank is being filled. Returns 0 for "not yet", -1 for error, 1 for "working properly"
  if(status != PUMPING && status != PUMPING_FILLING){ //if not pumping
    sup_level_at_start = 0;   //reset level
    pump_time = 0;          //reset timer
    return 0;
  }
  else{  //if pumping
    if (previous_status != PUMPING && previous_status != PUMPING_FILLING){
      pump_time = millis();
      sup_level_at_start = sup_current_level;
    }
  }

  if(millis() - pump_time >= TIME_TO_CHECK){ //if TIME_TO_CHECK passed
    if (sup_current_level == -1){  //In case get_level had errors
      return 0;
    }
    if(sup_current_level - sup_level_at_start <= 0){  //if level not increased
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

//////////////// SOURCE  //////////////////////////////////////////////////////////////////////////////////////

void start_source(){      //Turns on water to fill lower tank
  digitalWrite(SOURCE_PIN, HIGH);
}

void stop_source(){     //Stops filling the inf tank
  digitalWrite(SOURCE_PIN, LOW);
}    

int check_source(){      //Checks if lower tank is being filled. Returns 0 for "not yet", -1 for error, 1 for "working properly"
  if(status != FILLING && status != PUMPING_FILLING){ //if not pumping
      inf_level_at_start = 0;   //reset level
      source_time = 0;          //reset timer
      return 0;
    }
    else{  //if filling (used for the first loop)
      if (previous_status != FILLING && previous_status != PUMPING_FILLING){
        source_time = millis();
        inf_level_at_start = inf_current_level;
      }
    }

  if(millis() - source_time >= TIME_TO_CHECK){ //if TIME_TO_CHECK passed
    if (inf_current_level == -1){  //In case get_level had errors
      return 0;
    }
    if(inf_current_level - inf_level_at_start <= 0){  //if level not increased
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
