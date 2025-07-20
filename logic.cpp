#include "control.h"
#include "globalVariables.h"
#include "config/config.h"
#include <Arduino.h>


void handle_lower_tank (){
  if(status != SOURCE_ERROR){
    if(inf_current_level < MIN_INF_TO_PUMP){ //if lower tank not full
      start_source();
      switch(status){  //change status
        case WAITING: status = FILLING; break;
        case PUMPING: status = PUMPING_FILLING; break;
      }
      if(check_source() == -1){
        status = SOURCE_ERROR;
      }
    }
    if(INF_TANK_HI - inf_current_level < INF_TANK_HI / SENSOR_RANGE){ //if lower tank full
      stop_source();
      switch(status){
        case FILLING: status = WAITING; break;
        case PUMPING_FILLING: status = PUMPING; break;
      }
      check_source(); //resets timer and level_at_start in check_source()
    }
  }
}


void handle_upper_tank (){
  if(status != PUMP_ERROR){
    if(sup_current_level < SUP_ACC_LEVEL && inf_current_level >= MIN_INF_TO_PUMP){  //if upper tank under acceptable level AND lower not empty
      start_pump();
      switch(status){  //change status
        case WAITING: status = PUMPING; break;
        case FILLING: status = PUMPING_FILLING; break;
      }
      if(check_pumping() == -1){
        status = PUMP_ERROR;
      }
    }
    if(SUP_TANK_HI - sup_current_level < SUP_TANK_HI / SENSOR_RANGE || inf_current_level < INF_TANK_HI / SENSOR_RANGE){  //if upper tank full OR lower tank empty
      stop_pump();
      switch(status){
        case PUMPING: status = WAITING; break;;
        case PUMPING_FILLING: status = FILLING; break;
      }
      check_pumping(); //resets timer and level_at_start in check_pump()
    }
  }
}


void handle_error(){
  switch (status){
  ///////////////////////////////////
    case SENSOR_ERROR:
      delay(5000);
      //Continously checks sensors, changes status when sensors values are right
      //Check upper tank sensor
      if(get_level(SUP_SENSOR_PIN) == -1){
        status = SENSOR_ERROR;
        Serial.println("ERROR: upper tank sensor malfunctioning.");
      }
      else{
        Serial.println("Upper tank sensor working properly.");
        status = WAITING;
      }

      //Check lower tank sensor
      if(get_level(INF_SENSOR_PIN) == -1){
        status = SENSOR_ERROR;
        Serial.println("ERROR: lower tank sensor malfunctioning.");
      }
      else{
        Serial.println("Lower tank sensor working properly.\n");
        status = WAITING;
      }
      break;
    /////////////////////////////////////////////////////////////////////////////////////////////////

    case PUMP_ERROR:
      if(sup_current_level < SUP_ACC_LEVEL && inf_current_level >= MIN_INF_TO_PUMP){ //if sup is low and inf is acc
        Serial.println("Pump error: upper tank not being filled.");
        start_pump();
        sup_level_at_start = sup_current_level;
        pump_time = millis();
        while (pump_time + TIME_TO_CHECK > millis()){
          if(sup_level_at_start < get_level(SUP_SENSOR_PIN)){
            Serial.println("\nFalse error: pump working correctly.");
            Serial.println("System resumed...\n");
            status = PUMPING;
            return;
          }
        }
        stop_source(); //if error persist turns off source
        while(1){
          Serial.println("Pump malfunctioning. System intrrupted.\n");
          delay(5000);  
        }
      }
    /////////////////////////////////////////////////////////////////////////////////////////////////

    case SOURCE_ERROR:
      Serial.println("\nSource error: lower tank is not being filled.");
      Serial.println("Pumping will still work if water is available.\n");
      if(source_time + TIME_TO_CHECK < millis() || source_time == 0){
        inf_level_at_start = inf_current_level;
        source_time = millis();
      }
      if(inf_current_level > inf_level_at_start){
        Serial.println("Lower tank is being filled, system will resume...\n");
        status = FILLING;
        return;
      }
      delay(5000);
      break;
    //////////////////////////////////////////////////////////////////////////////////////////////////
  }
}


void log(){
  if (previous_status != status){  //used for serial monitor update
    previous_status = status;
    switch (status){
      case WAITING: Serial.println("Status: Waiting..."); break;
      case PUMPING: Serial.println("Status: Pumping..."); break;
      case FILLING: Serial.println("Status: Filling lower tank..."); break;
      case PUMPING_FILLING: Serial.println("Status: Pumping and filling lower tank..."); break;
      case SENSOR_ERROR: Serial.println("ERROR: Sensor values are not acceptable. Try re-calibrating sensors."); break;
      case PUMP_ERROR: Serial.println("ERROR: Pump malfunctioning."); break;
      case SOURCE_ERROR:  Serial.println("SOURCE_ERROR"); break;
    }
  }
}