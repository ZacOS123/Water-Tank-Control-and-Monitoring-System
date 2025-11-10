// v.2.0 //

#include "control.h"
#include "config/config.h"
#include "globalVariables.h"
#include "logic.h"
#include "connectivity.h"
#include <NimBLEDevice.h>
#include <WiFi.h>

void setup(){
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

  /////////WiFi connection setup////////
  Serial.println("Connecting to WiFi...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  for(int k=0; k<10; k++) {
    if(WiFi.status() != WL_CONNECTED){
      WIFI_ERROR = false;
      Serial.println("WiFi connected!");
      break;
    }
    delay(1000);
    Serial.print(".");
    if(k == 9){
      WIFI_ERROR = true;
      Serial.println("Unable to connect! (System will keep retrying...)");
    }
  }
  
  ////////Bluetooth connection setup////////
  NimBLEDevice::init("supTank");
  if (BLE_scan_and_connect() == 0){
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