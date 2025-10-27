// v.2.0 //

#include "control.h"
#include "config/config.h"
#include "globalVariables.h"
#include "logic.h"

void setup()
{
  Serial.begin(9600);
  Serial.println(" ");
  Serial.println("Hello!");

  ///////pin setup//////////
  pinMode(SUP_TRIG_PIN, OUTPUT);
  pinMode(SUP_ECHO_PIN, INPUT);
  pinMode(PUMP_PIN, OUTPUT);

  ////////Check superior tank sensors/////////
  Serial.println("\nChecking Sensors...");

  if(get_level_sup() == -1){
    status = SENSOR_ERROR;
    Serial.println("ERROR: upper tank sensor malfunctioning.");
  }
  else{
    Serial.println("Upper tank sensor working properly.");
    status = WAITING;
  }

  /////////Cloud connection setup////////

  ////////Bluetooth connection setup////////

}


void loop(){
  sup_current_level = get_level_sup(); // same here
  lower_status = get_lower_status(); //saves lower system status
  
  handle_upper_tank();
  handle_error();
  log();
}