#include "control.h"
#include "globalVariables.h"
#include "config/config.h"
#include <Arduino.h>


void handle_upper_tank (){
  if(status != BLOCKING_ERROR && lower_status != 3){ //get_lower_status code corrisponding to enum (3 = blocking error)
    if(sup_current_level > SUP_ACC_LEVEL && inf_current_level >= MIN_INF_TO_PUMP){  //if upper tank under acceptable level AND lower not empty
      start_pump();
      status = PUMPING;
      if(check_pumping() == -1){
        stop_pump();
        PUMP_ERROR = true;
      }
    }
    if(sup_current_level > (SUP_SENSOR_HI - SUP_SENSOR_LO) / SENSOR_RANGE || inf_current_level <= 2){  //if upper tank full OR lower tank empty (2%)
      stop_pump();
      status = WAITING;
      check_pumping(); //resets timer and level_at_start in check_pump()
    }
  }
}

////////////////////////////////

void handle_error(){
  //checks sensors
  if(sup_current_level == -1){
    stop_pump();
    SENSOR_ERROR = true;
  }
  else{
    SENSOR_ERROR = false;
  }

  //Set status
  if(SENSOR_ERROR || PUMP_ERROR || BLE_ERROR){
    stop_pump();
    status = BLOCKING_ERROR;
  }
}

/////////////////////////////////

void log(){
  if (previous_status != status){  //used for serial monitor update
    previous_status = status;
    switch (status){
      case WAITING: Serial.println("Status: Waiting..."); break;
      case PUMPING: Serial.println("Status: Pumping..."); break;
      case BLOCKING_ERROR: Serial.print("The following error(s) occurred:"); 
        if(PUMP_ERROR){
          Serial.println("Pump malfunctioning. System interrupted.");
        }
        if(SENSOR_ERROR){
          Serial.println("Sensor values are not acceptable.\nTry re-calibrating.\nSystem will resume when values are compatible...");
        }
        if(BLE_ERROR){
          Serial.println("Connection to lower tank lost.\nSystem interrupted.\nTrying to reconnect...");
        }
        break;
    }
    if(WIFI_ERROR){
      Serial.println("Connection to server lost. The system will continue offline.\nTrying to reconnect...");
    }
  }
}