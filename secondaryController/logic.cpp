#include "control.h"
#include "globalVariables.h"
#include "config/config.h"
#include <NimBLEDevice.h>


void handle_lower_tank (){
  if(status != BLOCKING_ERROR){
    if(inf_current_level > INF_SENSOR_LO + ((INF_SENSOR_HI - INF_SENSOR_LO) / SENSOR_RANGE)){  //if lower tank not full
      start_source();
      status = FILLING;
      if(check_source() == -1){
        SOURCE_ERROR = true;
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
  //Check sensor//
  if(inf_current_level == -1){
    SENSOR_ERROR = true;
    status = BLOCKING_ERROR;
  }
  else{
    SENSOR_ERROR = false;
  }
}

//////////////////////////////////////////////////////////////////////

void handle_communication(){
  if(millis() - BLE_time >= TIME_TO_UPDATE){
    BLE_time = millis();

    //build error flags//
    uint16_t error_flags = 0;
    if(SENSOR_ERROR){
      error_flags |= 0x01;
    }
    if(SOURCE_ERROR){
      error_flags |= 0x02;
    }
    if(status == FILLING){
      if(millis() - source_time >= TIME_TO_CHECK){
        error_flags |= ( ((inf_current_level - INF_SENSOR_LO) / (inf_level_at_start - inf_current_level)) * TIME_TO_CHECK ) << 8;
      }
    }
    pInfLevel->setValue((uint8_t)inf_current_level); 
    pErrorFlags->setValue(error_flags); 
    pInfLevel->indicate();
    pErrorFlags->indicate();
    Serial.println("Updated data via BLE.");
  }
}
  

//////////////////////////////////////////////////////////////////////

void log(){
  if(previous_status != status){  //used for serial monitor update
    previous_status = status;
    switch (status){
      case WAITING: Serial.println("Status: Waiting..."); break;
      case FILLING: Serial.println("Status: Filling lower tank..."); break;
      case BLOCKING_ERROR: Serial.print("The following error occurred:"); 
        if(SENSOR_ERROR){
          Serial.println("Sensor error.\nSystem interrupted.\nCheck sensor or consider re-calibrating.");
        }
        else if(SOURCE_ERROR){
          Serial.println("Source error.\nSystem operation will continue if water is present.");
        }
        break;
    }
  }
}