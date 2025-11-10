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
    SENSOR_ERROR = true;
    Serial.println("ERROR: lower tank sensor malfunctioning.");
  }
  else{
    Serial.println("lower tank sensor working properly.");
    SENSOR_ERROR = false;
  }

  //Bluetooth setup
  if(!BLE.begin()){
    BLE_ERROR = true;
    Serial.println("Failed to start BLE!");
  }
  else{
    BLE.setLocalName("infTank");
    BLE.setDeviceName("infTank");
    BLE.setAdvertisedService(systemService);
    systemService.addCharacteristic(waterLevel);
    systemService.addCharacteristic(infErrors);
    BLE.addService(systemService);
    
    BLE.setEventHandler(BLEDisconnected, disconnect_handler);
    BLE.setEventHandler(BLEConnected, connect_handler);

    BLE.advertise();
    Serial.println("Started advertising...");
    BLE_ERROR = false;
  }

}

void loop(){
  inf_current_level = get_level_inf(); //inverted due to ultrasonic sensor

  handle_error();
  handle_lower_tank();
  handle_communication();

  log();
}