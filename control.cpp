#include "control.h"
#include "globalVariables.h"
#include "config/config.h"
#include <Arduino.h>


int get_level (int sensor_pin){    //returns water level in selected tank. Return -1 for sensor/program error after setting status
 // if (sensor_pin < 0 || sensor_pin > 19){  //checks if sensor pin is acceptable (set for Arduino UNO)
 //   status = PROGRAM_ERROR;
 //   return -1;
 // }
 // else{
    int value = analogRead(sensor_pin);  //possible feature: takes different values and calculates the average, for accurate readings...
    if (sensor_pin == INF_SENSOR_PIN && (value < INF_TANK_LO || value > INF_TANK_HI)){ //checks if sensor values are acceptable corresponding to the inf tank
      status = SENSOR_ERROR;
      return -1;
    }
    if (sensor_pin == SUP_SENSOR_PIN && (value < SUP_TANK_LO || value > SUP_TANK_HI)){ //checks if sensor values are acceptable corresponding to the sup tank
      status = SENSOR_ERROR;
      return -1;
    }
    else{
      return value;
    }

 // }
}

////////////////  PUMP  //////////////////////////////////////////////////////////////////////////

void start_pump(){      //Turns on the pump to fill upper tank
  digitalWrite(PUMP_PIN, HIGH);
  sup_level_at_start = get_level(SUP_SENSOR_PIN);
  pump_time = millis(); //starts timer (used for check functions)
  switch (status){  //changes status relative to the past status
  case WAITING:
    status = PUMPING;
    break;

  case FILLING:
    status = PUMPING_FILLING;
    break;

  case PROGRAM_ERROR:  
  case SENSOR_ERROR:  
  case PUMP_ERROR: 
    break; 

  case SOURCE_ERROR:   
    status = SOURCE_ERROR;  //used for SOURCE_ERROR logic
    break;

  case PUMPING:
  case PUMPING_FILLING:

  default:
    status = PROGRAM_ERROR;
    break;
  }
}

void stop_pump(){   //turns of the pump
  digitalWrite(PUMP_PIN, LOW);
  switch (status){  //changes status relative to the past status
    case PUMPING:
      status = WAITING;
      break;
    case PUMPING_FILLING:
      status = FILLING; 
      break;

    case PROGRAM_ERROR:  
    case SENSOR_ERROR:  
    case PUMP_ERROR:     
    case SOURCE_ERROR:
      break; 
  
    case WAITING:
    case FILLING:

    default:
      status = PROGRAM_ERROR;
      break;
  }
} 

int check_pumping(){      //Checks if upper tank is being filled. Returns 0 for "not yet", -1 for error, 1 for "working properly"
  if(millis() - pump_time <= TIME_TO_CHECK){
    return 0; //does nothing if it has not elapsed TIME_TO_CHECK
  }
  else
  {
    int level = get_level(SUP_SENSOR_PIN);
    if (level == -1){return -1;}  //In case get_level had errors
    if(level - sup_level_at_start <= 0){  //checks if the current level is lower or higher than the level at start
      status = PUMP_ERROR;
      return -1;
    }
    else{
      sup_level_at_start = get_level(SUP_SENSOR_PIN);
      pump_time = millis();
      return 1;
    }
  }
}

//////////////// SOURCE  //////////////////////////////////////////////////////////////////////////////////////

void start_source(){      //Turns on water to fill lower tank
  digitalWrite(SOURCE_PIN, HIGH);
  inf_level_at_start = get_level(INF_SENSOR_PIN);
  source_time = millis(); //starts timer (used for check functions)
  switch (status){   //changes status relative to the past status
  case WAITING:
    status = FILLING;
    break;
  case PUMPING:
    status = PUMPING_FILLING;
    break;

  case PROGRAM_ERROR:  
  case SENSOR_ERROR:  
  case PUMP_ERROR:
  case SOURCE_ERROR:
    break;

  case FILLING:
  case PUMPING_FILLING:

  default:
    status = PROGRAM_ERROR;
    break;
  }
}

void stop_source(){     //Stops filling the inf tank
  digitalWrite(SOURCE_PIN, LOW);
  switch (status){    //changes status relative to the past status
  case FILLING:
    status = WAITING;

  case PUMPING_FILLING:
    status = PUMPING;  //error if already filling or filling and pumping and entered start_source
    break;

  case PROGRAM_ERROR:  
  case SENSOR_ERROR:  
  case PUMP_ERROR:
  case SOURCE_ERROR:
    break;

  case WAITING:
  case PUMPING:
  default:
    status = PROGRAM_ERROR;
    break;
  }
}    

int check_source(){      //Checks if lower tank is being filled. Returns 0 for "not yet", -1 for error, 1 for "working properly"
    if(millis() - source_time <= TIME_TO_CHECK){
    return 0; //does nothing if it has not elapsed TIME_TO_CHECK
  }
  else
  {
    int level = get_level(INF_SENSOR_PIN);
    if (level == -1){return 0;}  //In case get_level had errors
    if(level - inf_level_at_start <= 0){  //checks if the current level is lower or higher than the level at start
      status = SOURCE_ERROR;
      source_time = millis();
      return -1;
    }
    else{
      inf_level_at_start = get_level(INF_SENSOR_PIN);
      source_time = millis();
      return 1;
    }
  }
}
