
#include "control.h"
#include "config.h"
#include "globalVariables.h"

void setup()
{
  Serial.begin(9600);
  Serial.println(" ");
  Serial.println("Hello!");

  //pin setup
  pinMode(PUMP_PIN, OUTPUT);
  pinMode(SOURCE_PIN, OUTPUT);

  //Check sensors
  Serial.println("\nChecking system...");

  //Check upper tank sensor
  if(get_level(SUP_SENSOR_PIN) == -1){
    status = SENSOR_ERROR;
    Serial.println("ERROR: upper tank sensor malfunctioning.");
  }
  else{
    Serial.println("Upper tank sensor working properly.");
    status = WAITING;
  }

  //Check upper tank sensor
  if(get_level(INF_SENSOR_PIN) == -1){
    status = SENSOR_ERROR;
    Serial.println("ERROR: lower tank sensor malfunctioning.");
  }
  else{
    Serial.println("Lower tank sensor working properly.\n");
    status = WAITING;
  }
}

void loop()
{
  if (previous_status != status){  //used for serial monitor update
    previous_status = status;
    switch (status){
      case WAITING: Serial.println("Status: Waiting..."); break;
      case PUMPING: Serial.println("Status: Pumping..."); break;
      case FILLING: Serial.println("Status: Filling lower tank..."); break;
      case PUMPING_FILLING: Serial.println("Status: Pumping and filling lower tank..."); break;
      case PROGRAM_ERROR: Serial.println("ERROR: Program error."); break;
      case SENSOR_ERROR: Serial.println("ERROR: Sensor values are not acceptable. Try re-calibrating sensors."); break;
      case PUMP_ERROR: Serial.println("ERROR: Pump malfunctioning."); break;
      case SOURCE_ERROR: Serial.println("ERROR: Lower tank not filling."); break;
    }
  }

  inf_current_level = get_level(INF_SENSOR_PIN);
  sup_current_level = get_level(SUP_SENSOR_PIN);

  switch (status){
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    case WAITING:
      if(sup_current_level < SUP_ACC_LEVEL)  //if current level less that acceptable level
      {
        if(inf_current_level >= MIN_INF_TO_PUMP){
          start_pump();
          break;
        }
        else{
          start_source();
          break;
        }
      }
      else{
        if(INF_TANK_HI - inf_current_level <= INF_TANK_HI / SENSOR_RANGE){  
          start_source();
          break;
        }
        else{
          break;
        }
      }
    ///////////////////////////////////////////////////////////////////////////////////////////////

    case PUMPING:
      if(check_pumping() != -1){  //checks if pump is pumping

        if(SUP_TANK_HI - sup_current_level < SUP_TANK_HI / SENSOR_RANGE){   //if sup tank not HI
          stop_pump();
          if(INF_TANK_HI - inf_current_level < INF_TANK_HI / SENSOR_RANGE){ //if inf tank full
            break;
          }
          else{
            start_source();
            break;
          }
        }
        else{
          if(inf_current_level - INF_TANK_LO < INF_TANK_HI / SENSOR_RANGE){ //If inf tank empty
            stop_pump();
            start_source();
            break;
          }
          else{  //if inf not empty
            if(inf_current_level < INF_TANK_HI){
              start_source();
            }
            else{
              break;
            }
          }
        }
      }
      else{   //else, if check_pumping gives error
        break;
      }     
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    case FILLING:
      if(check_source() != -1){  //checks if source is filling lower tank
        if (sup_current_level < SUP_ACC_LEVEL){
          if(inf_current_level > MIN_INF_TO_PUMP){  //inf level acceptable to pump
            start_pump();
            break;
          }
          else if (INF_TANK_HI - inf_current_level < INF_TANK_HI / SENSOR_RANGE){ //if inf tank full
            stop_source();
            start_pump();
            break;
          }
          else{
            break;
          }
        }
        else{
          if(INF_TANK_HI - inf_current_level < INF_TANK_HI / SENSOR_RANGE){
            stop_source();
          }
          else{
            break;
          }
        }
      }
      else{   //else, if check_source gives error
        break;
      }    
    //////////////////////////////////////////////////////////////////////////////////////////////

    case PUMPING_FILLING:
      if(check_pumping == 1 || check_pumping == 0){  //checks if pump is pumping
        if(SUP_TANK_HI - sup_current_level < SUP_TANK_HI / SENSOR_RANGE){
          stop_pump();
          if(INF_TANK_HI - inf_current_level < INF_TANK_HI/SENSOR_RANGE){
            stop_source();
            break;
          }
          else{
            break;
          }
        }
        else{
          if (inf_current_level < MIN_INF_TO_PUMP){
            stop_pump();
            break;
          }
          else{
            break;
          }
        }
      }
      else{   //else if pump_check gives error
        break;
      }
    /////////////////////////////////////////////////////////////////////////////////////////////////

    case PROGRAM_ERROR:
      while(1){
        Serial.println("Program error. System interrupted.");
        delay(5000);
      }
    /////////////////////////////////////////////////////////////////////////////////////////////////

    case SENSOR_ERROR:
      while(status = SENSOR_ERROR){  //Continously checks sensors, changes status when sensors values are right
        //Check upper tank sensor
        if(get_level(SUP_SENSOR_PIN) == -1){
          status = SENSOR_ERROR;
          Serial.println("ERROR: upper tank sensor malfunctioning.");
        }
        else{
          Serial.println("Upper tank sensor working properly.");
          status = WAITING;
        }

        //Check upper tank sensor
        if(get_level(INF_SENSOR_PIN) == -1){
          status = SENSOR_ERROR;
          Serial.println("ERROR: lower tank sensor malfunctioning.");
        }
        else{
          Serial.println("Lower tank sensor working properly.\n");
          status = WAITING;
        }
        delay(5000);
      }
      break;
    /////////////////////////////////////////////////////////////////////////////////////////////////

    case PUMP_ERROR:
      if(millis() - pump_time > 2*TIME_TO_CHECK + 1000){  //if 2*TIME_TO_CHECK + 1sec have passed (+1sec to give enough time for the check_pump)
        stop_pump();
        stop_source();
        while(1){
          Serial.println("Pump error. System interrupted.");
          delay(5000);
        }
      }
      else{         //if not yet passed
        if(check_pumping() == 1){
          status = WAITING;  //if pump woking correctly, starts procedure form start
          break;
        }
      }
    /////////////////////////////////////////////////////////////////////////////////////////////////

    case SOURCE_ERROR:
      while(check_source() != 1){
        if(sup_current_level < SUP_ACC_LEVEL && inf_current_level > MIN_INF_TO_PUMP){  //pumps remaining water if needed. Pumps in case lower tank comes alternatively filled
          Serial.println("Pumping remaining water. System will be interrupted afterwards if source not available.");
          do{
            start_pump();
            if(check_pumping() == -1)
            {
              break;
            }
          } while(SUP_TANK_HI - sup_current_level >= SUP_TANK_HI - SENSOR_RANGE && status == SOURCE_ERROR);
          stop_pump();
          Serial.println("Finished pumping remaining water.");
          status = SOURCE_ERROR;
        }
        Serial.println("Source error. Lower tank is not being filled. System interrupted until water source is available.");
        delay(5000);
      }
    //////////////////////////////////////////////////////////////////////////////////////////////////

    default:
      status = PROGRAM_ERROR;
      break;
    //////////////////////////////////////////////////////////////////////////////////////////////////
  }
}