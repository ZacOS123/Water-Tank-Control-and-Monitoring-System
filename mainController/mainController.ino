// v.2.0 //

#include "control.h"
#include "config/config.h"
#include "globalVariables.h"
#include "logic.h"
#include "connectivity.h"
#include <NimBLEDevice.h>
#include <WiFi.h>

void setup(){
  delay(1000); //startup delay for power stability

  Serial.begin(115200);
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

  /////////WiFi connection setup////////
  Serial.println("Connecting to WiFi...");
  WiFi.mode(WIFI_STA);
  WiFi.onEvent(WiFi_error_handler);
  WiFi.begin(ssid, password);

 
  ////////Bluetooth connection setup////////
  Serial.println("Starting BLE and searching for InfTank");

  NimBLEDevice::init("supTank");

  NimBLEScan* pScan = NimBLEDevice::getScan();
  NimBLEScanResults results = pScan->getResults(10 * 1000); // continuous scan
  
  if(search_and_connect(results) == 1){
    BLE_ERROR = false;
  }
  else{
    BLE_ERROR = true;
  }
}


void loop(){
  sup_current_level = get_level_sup(); // same here
 // get_inf_data(); //updates data from lower tank

  handle_error();
  handle_upper_tank();
  update_cloud();
  
  log();
}