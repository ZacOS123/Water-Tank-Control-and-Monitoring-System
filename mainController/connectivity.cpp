#include "connectivity.h"
#include "globalVariables.h"
#include "config/config.h"
#include <NimBLEDevice.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>


////BLE /////

void infLevelNotifyCB(NimBLERemoteCharacteristic* pChar, uint8_t* data, size_t length, bool isNotify){
  if (length >= sizeof(int)) {
      // Server sent a full int (4 bytes on ESP32)
      memcpy(&inf_current_level, data, sizeof(int));
  } else if (length == 1) {
      // Server sent only a single byte representing percentage
      inf_current_level = data[0];
  }
  Serial.print("inf_current_level updated: ");
  Serial.println(inf_current_level);
}

void errorFlagsNotifyCB(NimBLERemoteCharacteristic* pChar, uint8_t* data, size_t length, bool isNotify){
  if (length >= sizeof(uint16_t)) {
      // Copy 2 bytes from BLE notification to the global variable
      memcpy(&inf_errors, data, sizeof(uint16_t));
  } else if (length == 1) {
      // Sometimes server may send only 1 byte
      inf_errors = data[0];
  }
  Serial.print("inf_errors updated: ");
  Serial.println(inf_errors);
}

int search_and_connect (NimBLEScanResults results){  //return 0 for error, 1 for success
  for (int i = 0; i < results.getCount(); i++) {
    const NimBLEAdvertisedDevice *device = results.getDevice(i);
    if(!device){
      Serial.println ("No device Found.");
      return 0;
    }
    
    if (device->isAdvertisingService(serviceUUID)) {
      Serial.println("Checking BLE devices...");
      pClient = NimBLEDevice::createClient();

      if (!pClient) { // Make sure the client was created
          return 0;
      }

      if (pClient->connect(device)) {
          NimBLERemoteService *pService = pClient->getService(serviceUUID);
        if(!pService){
          Serial.println("BLE Error: could not find service.");
          pClient->disconnect();         
          return 0;
        }
        else {
          pInfLevel = pService->getCharacteristic(infLevelUUID);
          pErrorFlags = pService->getCharacteristic(errorFlagsUUID);

          if (pInfLevel && pInfLevel->canIndicate()) {
            pInfLevel->subscribe(true, infLevelNotifyCB, true);
            Serial.println("Subscribed to infLevel notifications");
          }
          if (pErrorFlags && pErrorFlags->canIndicate()) {
            pErrorFlags->subscribe(true, errorFlagsNotifyCB, true);
            Serial.println("Subscribed to errorFlags notifications");
          }
          return 1;
        }
      }
      else {
        pClient->disconnect(); // failed to connect
      }
    }
  }
  Serial.println ("InfTank BLE not found!");
  return 0;
}

void onConnect(NimBLEClient* pClient){
  Serial.println("Connected to server");
}

void onDisconnect(NimBLEClient* pClient, int reason){
  Serial.print("BLE Disconnected! Reason: ");
  Serial.println(reason);

  // Clear characteristic pointers
  pInfLevel = nullptr;
  pErrorFlags = nullptr;

  BLE_ERROR = true;
}

////////////////////////////////////////////////////////////////////////////





//// WiFi and Cloud ////

void update_cloud(){ //updates data on cloud (measurify)
  if(!WIFI_ERROR && millis() - update_time >= TIME_TO_UPDATE){
    update_time = millis();

    //upper update//
    Serial.println("Updating upper cloud data...");
    HTTPClient http;
    WiFiClientSecure client;
    client.setInsecure();   // used for https instead of http
    if(!http.begin("https://tracker.elioslab.net/v1/measurements/Livello_acqua_A/timeserie")){
      Serial.println("HTTP error: Couldn't prepare connection to update upper data");
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
      if(millis() - pump_time >= TIME_TO_CHECK){
        sup_errors |= ( ((sup_current_level - SUP_SENSOR_LO) / (sup_level_at_start - sup_current_level)) * TIME_TO_CHECK ) << 8;
      }
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
      Serial.println("POST failed for upper tank");
      Serial.print("Response code: ");
      Serial.println(httpCode);
    }
    http.end();
  }

  ///////////////////////////////

  //Lower update//
  if(!BLE_ERROR){
    Serial.println("Updating lower cloud data...");
    HTTPClient http;
    WiFiClientSecure client;
    client.setInsecure();   // used for https instead of http
    if(!http.begin("https://tracker.elioslab.net/v1/measurements/Livello_acqua_B/timeserie")){
      Serial.println("HTTP error: Couldn't prepare connection to update lower data.");
    }

    http.addHeader("Content-Type", "application/json");
    http.addHeader("Authorization", "DVC eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJkZXZpY2UiOnsiX2lkIjoiRGlzcG9zaXRpdm8yIiwiZmVhdHVyZXMiOlsiTGl2ZWxsb19BY3F1YSJdLCJ0aGluZ3MiOlsiQ2lzdGVybmFBIiwiQ2lzdGVybmFCIl0sInZpc2liaWxpdHkiOiJwdWJsaWMiLCJvd25lciI6IjY5MWQ5OGZlNGEzYmY4MDAxZWJhMTVhMCJ9LCJ0ZW5hbnQiOnsicGFzc3dvcmRoYXNoIjp0cnVlLCJfaWQiOiJ3YXRlcl9jb250cm9sIiwib3JnYW5pemF0aW9uIjoiTWVhc3VyaWZ5IG9yZyIsImFkZHJlc3MiOiJNZWFzdXJpZnkgU3RyZWV0LCBHZW5vdmEiLCJlbWFpbCI6ImluZm9AbWVhc3VyaWZ5Lm9yZyIsInBob25lIjoiKzM5MTAzMjE4NzkzODE3IiwiZGF0YWJhc2UiOiJ3YXRlcl9jb250cm9sIn0sImlhdCI6MTc2MzczNDY3NSwiZXhwIjozMzMyMTMzNDY3NX0.1I2qC0J_3iqa8g6ZOA_XaOZgMP0olMLKidkeDvKZX_k");

    // json string build
    String json = "{";
    json += "\"timestamp\": \"" + String((now*1000) + (millis() - sync_time)) + "\","; 
    json += "\"values\": [" + String(inf_current_level) + ", " + String(inf_errors) + "]";
    json += "}";

    int httpCode = http.POST(json);
    if (httpCode > 0) {
      Serial.println("Response code: " + String(httpCode));
    }
    else {
      Serial.println("POST failed for lower tank");
      Serial.print("Response code: ");
      Serial.println(httpCode);
    }
    http.end();
  }
}

////////////////////////////////////////////////////////////////////

void WiFi_error_handler(WiFiEvent_t event, WiFiEventInfo_t info){ //checks if wifi is still connected
  switch (event) {
    case ARDUINO_EVENT_WIFI_STA_GOT_IP:
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

    case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
      WIFI_ERROR = true;
      break;
  }
}




