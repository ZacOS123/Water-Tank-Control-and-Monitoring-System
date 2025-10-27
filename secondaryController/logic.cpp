#include "control.h"
#include "globalVariables.h"
#include "config/config.h"
#include <Arduino.h>


void handle_lower_tank (){
  if(status != SENSOR_ERROR && status != SENSOR_SOURCE_ERROR){
    if(inf_current_level > INF_SENSOR_LO + ((INF_SENSOR_HI - INF_SENSOR_LO) / SENSOR_RANGE)){  //if lower tank not full
      start_source();
      status = FILLING;
      if(check_source() == -1){
        status = SOURCE_ERROR;
      }
    }
    else{  //Lower tank full
      stop_source();
      status = WAITING;
      check_source(); //resets timer and level_at_start in check_pump()
    }
  }
}

//////////////////////////////////////////////////////////////////////

void handle_error(){
  if(inf_current_level == -1){ //checks sensors
    switch (status){
      case SOURCE_ERROR:  status = SENSOR_SOURCE_ERROR; break;
      case SENSOR_SOURCE_ERROR: break;
      default: status = SENSOR_ERROR; break;
    }
  }
  else{
    switch (status){
      case SENSOR_SOURCE_ERROR: status = SOURCE_ERROR; break;
      case SENSOR_ERROR: status = WAITING; break;
    }
  }
}

//////////////////////////////////////////////////////////////////////

void handle_communication(){
  if(update_data_BLE() == -1){
    status = BLE_ERROR;
  }
}

//////////////////////////////////////////////////////////////////////

void log(){
  if(previous_status != status){  //used for serial monitor update
    previous_status = status;
    switch (status){
      case WAITING: Serial.println("Status: Waiting..."); break;
      case FILLING: Serial.println("Status: Filling lower tank..."); break;
      case SENSOR_ERROR: Serial.println("ERROR: Upper tank sensor values are not acceptable. Try re-calibrating sensors."); break;
      case SOURCE_ERROR:  Serial.println("SOURCE_ERROR"); break;
      case SENSOR_SOURCE_ERROR:  Serial.println("SOURCE and SENSOR ERROR"); break;
    }
  }
}