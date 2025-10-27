// v.2.0 //

#include "control.h"
#include "config/config.h"
#include "globalVariables.h"
#include "logic.h"
#include <ArduinoBLE.h>



void setup()
{
  Serial.begin(9600);
  Serial.println(" ");
  Serial.println("Hello!");

  //pin setup
  pinMode(INF_TRIG_PIN, OUTPUT);
  pinMode(INF_ECHO_PIN, INPUT);
  pinMode(SOURCE_PIN, OUTPUT);

  //Check Inferior tank sensors
  Serial.println("\nChecking Sensors...");

  if(get_level_inf() == -1){
    status = SENSOR_ERROR;
    Serial.println("ERROR: lower tank sensor malfunctioning.");
  }
  else{
    Serial.println("lower tank sensor working properly.");
    status = WAITING;
  }

  //Bluetooth setup
  BLE.begin();
  BLE.setLocalName("infTank");
  BLE.setAdvertisedService(systemService);
  systemService.addCharacteristic(waterLevel);
  systemService.addCharacteristic(infStatus);
  BLE.addService(systemService);
  BLE.advertise();
  Serial.println("BLE setup completed.\n");

}

void loop(){
  inf_current_level = get_level_inf(); //inverted due to ultrasonic sensor
  
  handle_error();
  handle_lower_tank();
  handle_communication();
  log();
}