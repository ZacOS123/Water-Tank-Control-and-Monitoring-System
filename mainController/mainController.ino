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
  WiFi.onEvent(WiFi_error_handler);
  WiFi.begin(ssid, password);
  

  wl_status_t wifi_status; //saves wifi status for current loop (as enum in wifi.h)
  for(int k=0; k<10; k++) {
    wifi_status = WiFi.status();
    if(wifi_status == WL_CONNECTED){
      WIFI_ERROR = false;
      Serial.println("WiFi connected!");

      //get current time
      Serial.println("Getting date and time... ");
      configTime(0, 0, "pool.ntp.org");
      while(now < 20){
        now = time(nullptr);
      }
      Serial.println("Done!");
      sync_time = millis();
      break;
    }
    delay(1000);
    Serial.print(".");
    if(k == 9 && wifi_status != WL_CONNECTED){
      WIFI_ERROR = true;
      Serial.println("Unable to connect! (System will keep retrying...)");
    }
  }

 
  ////////Bluetooth connection setup////////
  Serial.println("Starting BLE");

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