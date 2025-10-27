#include "control.h"
#include "globalVariables.h"
#include "config/config.h"
#include <Arduino.h>


void handle_upper_tank (){
  if(status != PUMP_ERROR && status != SENSOR_PUMP_ERROR && status != SENSOR_ERROR && lower_status != 3 && lower_status != 4){ //get_lower_status code corrisponding to enum
    if(sup_current_level > SUP_ACC_LEVEL && inf_current_level >= MIN_INF_TO_PUMP){  //if upper tank under acceptable level AND lower not empty
      start_pump();
      status = PUMPING;
      if(check_pumping() == -1){
        stop_pump();
        switch (status){
          case SENSOR_ERROR: status = SENSOR_PUMP_ERROR; break;
          case SENSOR_PUMP_ERROR: break;
          default: status = PUMP_ERROR; break;
        }
      }
    }
    if(sup_current_level > (SUP_SENSOR_HI - SUP_SENSOR_LO) / SENSOR_RANGE || inf_current_level <= 2){  //if upper tank full OR lower tank empty (2%)
      stop_pump();
      status = WAITING;
      check_pumping(); //resets timer and level_at_start in check_pump()
    }
  }
}


void handle_error(){
  ///////////////////////////////////
  if(sup_current_level == -1){ //checks sensors
    stop_pump();
    switch (status){
      case PUMP_ERROR:  status = SENSOR_PUMP_ERROR; break;
      case SENSOR_PUMP_ERROR: break;
      default: status = SENSOR_ERROR; break;
    }
  }
  else{
    switch (status){
      case SENSOR_PUMP_ERROR: status = PUMP_ERROR; break;
      case SENSOR_ERROR: status = WAITING; break;
    }
  }

  ///////////////////////////////////
  
  if(status == PUMP_ERROR){    //tries the pump one last time and then blocks if error persists
    if (previous_status != PUMP_ERROR){
      pump_time = millis();
      sup_level_at_start = sup_current_level;
    }
    if(millis() - pump_time >= TIME_TO_CHECK){ //if TIME_TO_CHECK passed
      if (sup_current_level == -1){  //In case get_level had errors
        status = SENSOR_PUMP_ERROR;
      }
      if(sup_current_level >= sup_level_at_start){  //if level not increased
        pump_time = millis();
        sup_level_at_start = sup_current_level;
        stop_pump();
        while(1){
          Serial.print("PUMP ERROR. SYSTEM INTERRUPTED. CHECK PUMP AND RESTART.\n\n");
          delay(5000);
        }
      }
      else{   //if level increased
        pump_time = millis();
        sup_level_at_start = sup_current_level;
        status = PUMPING;
      }
    }
  }
}


void log(){
  if (previous_status != status){  //used for serial monitor update
    previous_status = status;
    switch (status){
      case WAITING: Serial.println("Status: Waiting..."); break;
      case PUMPING: Serial.println("Status: Pumping..."); break;
      case SENSOR_ERROR: Serial.println("ERROR: Upper tank sensor values are not acceptable. Try re-calibrating sensors."); break;
      case PUMP_ERROR: Serial.println("ERROR: Pump malfunctioning."); break;
      case SENSOR_PUMP_ERROR: Serial.println("ERROR: Pump and Sensor malfuntioning"); break;
    }
  }
}