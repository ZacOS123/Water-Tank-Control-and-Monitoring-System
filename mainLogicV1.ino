// v.1.1 //

#include "control.h"
#include "config/config.h"
#include "globalVariables.h"
#include "logic.h"

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

void loop(){
  inf_current_level = get_level(INF_SENSOR_PIN);
  sup_current_level = get_level(SUP_SENSOR_PIN);
  
  handle_upper_tank();
  handle_lower_tank();
  handle_error();
  log();
}