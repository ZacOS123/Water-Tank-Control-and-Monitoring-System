// v.2.0 //

#include "control.h"
#include "config/config.h"
#include "globalVariables.h"
#include "logic.h"
#include "connectivity.h"
#include <NimBLEDevice.h>

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
    SENSOR_ERROR = true;
    Serial.println("ERROR: upper tank sensor malfunctioning.");
  }
  else{
    Serial.println("Upper tank sensor working properly.");
    SENSOR_ERROR = false;
  }

  /////////Cloud connection setup////////

  ////////Bluetooth connection setup////////
  NimBLEDevice::init("supTank");
  BLE_scan();
  if (BLE_connect() == 0){
    BLE_ERROR = false;
  }
}


void loop(){
  sup_current_level = get_level_sup(); // same here
  get_inf_data(); //updates data from lower tank

  handle_error();
  handle_upper_tank();
  
  log();
}