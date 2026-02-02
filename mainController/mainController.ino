// v.2.0 //

#include "control.h"
#include "config/config.h"
#include "globalVariables.h"
#include "logic.h"
#include "connectivity.h"
#include <NimBLEDevice.h>
#include <WiFi.h>
#include "esp_wifi.h"

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

  ////////Bluetooth connection setup////////
  Serial.println("Starting BLE and searching for InfTank");

  NimBLEDevice::init("supTank");

  NimBLEScan* pScan = NimBLEDevice::getScan();
  pScan->setActiveScan(true);     // important if name/service is in scan response
  pScan->clearResults();          // clear old/stale results
  NimBLEScanResults results = pScan->getResults(5000);
  pScan->stop();  
  delay(50);
  if(search_and_connect(results) == 1){
    BLE_ERROR = false;
  }
  else{
    BLE_ERROR = true;
  }
  delay(5000);  //delay for BLE connection

  /////////WiFi connection setup////////
  Serial.println("Connecting to WiFi...");
  WiFi.mode(WIFI_STA);
  WiFi.onEvent(WiFi_error_handler);
  WiFi.begin(ssid, password);
  esp_wifi_set_ps(WIFI_PS_NONE);
}


void loop(){
  sup_current_level = get_level_sup();

  handle_error();
  handle_upper_tank();
  update_cloud();
  
  log();
}