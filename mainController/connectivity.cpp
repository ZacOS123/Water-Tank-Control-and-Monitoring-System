#include "connectivity.h"
#include "globalVariables.h"
#include "config/config.h"
#include <NimBLEDevice.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>

/*
int BLE_scan_and_connect(){ //Scans and connects to inf tank. returns -1 for error, 0 for success

  NimBLEScan *pScan = NimBLEDevice::getScan();
  Serial.println("Scanning BLE devices...");
  NimBLEScanResults results = pScan->getResults(20 * 1000);  //scan for 15 secs
  int result_count = results.getCount();
  for (int i = 0; i < result_count; i++) {
    const NimBLEAdvertisedDevice *device = results.getDevice(i);

    //Server found//
    if (device != nullptr){
      if (device->isAdvertisingService(serviceUuid)) {
        pClient = NimBLEDevice::createClient();
        Serial.println("Lower tank system found!");
        if (!pClient) { // client not created
          Serial.println("Unable to initiate connection: unable to create client (locally).");
          return -1;
        }
        else{
          if (pClient->connect(&device)) {  //if was able to connect 
            pService = pClient->getService(serviceUuid);
            Serial.println("Connected to lower tank system!");

            return 0;
          }
          else { //if wasnt able to connect
            Serial.println("Failed to connect to server.");
            return -1;
          }
        }
      }
    } 

    //Server not found//
    Serial.println("Could not find BLE server.");
    return -1;
    
  }
} 

//////////////////////////////////////////////////////////////////////////

int get_inf_data (){  //returns inferior tank water level
  if(pClient != nullptr){
    if(pClient->isConnected()){ //if BLE connected
      if (pService != nullptr) {
        pInf_level = pService->getCharacteristic("74ac19c2-5aa1-4419-9426-dab1961d0b9f1");
        pError_flags = pService->getCharacteristic("74ac19c2-5aa1-4419-9426-dab1961d0b9f2");

        if(pError_flags != nullptr){
          std::string value = pError_flags->readValue();
          inf_errors = value[0];  //LSB = SENSOR_ERROR.    2nd LSB = SOURCE_ERROR
        }
        else{
          Serial.println("Could not store lower tank error flags: can't create BLE characteristic (locally).");
        }

        if (pInf_level != nullptr && !(inf_errors & 0x01)) {  //if char exists and no sensor error
          std::string value = pInf_level->readValue();
          inf_current_level = value[0];
        }
            else{
          Serial.println("Could not store lower tank water level: can't create BLE characteristic (locally).");
        }
      }
      else{
        Serial.println("Could not access lower tank data: can't create BLE service (locally).");
      }
    }
  }
} 
*/
//////////////////////////////////////////////////////////////////////////

//// WiFi and Cloud ////

////////////////////////////////////////////////////////////////////////////

void update_cloud(){ //updates data on cloud (measurify)
  if(!WIFI_ERROR && millis() - update_time >= TIME_TO_UPDATE){
    update_time = millis();
    Serial.println("Updating cloud data...");
    HTTPClient http;
    WiFiClientSecure client;
    client.setInsecure();   // used for https instead of http
    if(!http.begin("https://tracker.elioslab.net/v1/measurements/Livello_acqua_A/timeserie")){
      Serial.println("HTTP error: Couldn't prepare connection");
    }
    
    //sup error flags build
    uint8_t sup_errors = 0x00; //error flags
    if(SENSOR_ERROR){
      sup_errors |= 0x01;
    }
    if(PUMP_ERROR){
      sup_errors |= 0x02;
    }
    if(BLE_ERROR){
      sup_errors |= 0x04;
    }
    if(status == PUMPING){
      sup_errors |= 0x08;
    }

    //convert water level to percentage
    int percentage = (((SUP_SENSOR_HI - sup_current_level)*100)/(SUP_SENSOR_HI - SUP_SENSOR_LO));  //rounded to nearest
    

    http.addHeader("Content-Type", "application/json");
    http.addHeader("Authorization", "DVC eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJkZXZpY2UiOnsiX2lkIjoiRGlzcG9zaXRpdm8xIiwiZmVhdHVyZXMiOlsiTGl2ZWxsb19BY3F1YSJdLCJ0aGluZ3MiOlsiQ2lzdGVybmFBIiwiQ2lzdGVybmFCIl0sInZpc2liaWxpdHkiOiJwdWJsaWMiLCJvd25lciI6IjY5MWQ5OGZlNGEzYmY4MDAxZWJhMTVhMCJ9LCJ0ZW5hbnQiOnsicGFzc3dvcmRoYXNoIjp0cnVlLCJfaWQiOiJ3YXRlcl9jb250cm9sIiwib3JnYW5pemF0aW9uIjoiTWVhc3VyaWZ5IG9yZyIsImFkZHJlc3MiOiJNZWFzdXJpZnkgU3RyZWV0LCBHZW5vdmEiLCJlbWFpbCI6ImluZm9AbWVhc3VyaWZ5Lm9yZyIsInBob25lIjoiKzM5MTAzMjE4NzkzODE3IiwiZGF0YWJhc2UiOiJ3YXRlcl9jb250cm9sIn0sImlhdCI6MTc2MzczNDQwNCwiZXhwIjozMzMyMTMzNDQwNH0.pW2g-fBg-tYhvdunMrBu4NPDGJYnn3nKWv8Uvq3WxOc");

    // json string build
    String json = "{";
    json += "\"timestamp\": \"" + String((now*1000) + (millis() - sync_time)) + "\","; 
    json += "\"values\": [" + String(percentage) + ", " + String(sup_errors) + "]";
    json += "}";

//String(now + ((millis() - sync_time) / 1000))

    int httpCode = http.POST(json);
    if (httpCode > 0) {
      Serial.println("Response code: " + String(httpCode));
    }
    else {
      Serial.println("POST failed");
      Serial.print("Response code: ");
      Serial.println(httpCode);
    }
    http.end();
  }
}

////////////////////////////////////////////////////////////////////

void WiFi_error_handler(WiFiEvent_t event, WiFiEventInfo_t info){ //checks if wifi is still connected
  switch (event) {
    case IP_EVENT_STA_GOT_IP:
      WIFI_ERROR = false;
      Serial.println("WiFi connected");
      break;

    case WIFI_EVENT_STA_DISCONNECTED:
      WIFI_ERROR = true;
      Serial.println("WiFi disconnected");
      break;
  }
}




