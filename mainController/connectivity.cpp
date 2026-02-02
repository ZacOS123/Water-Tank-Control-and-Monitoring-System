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
      continue;
    }
    
    if (device->isAdvertisingService(serviceUUID)){
      Serial.println("InfTank found.\nConnecting...");
      pClient = NimBLEDevice::createClient();
      pClient->setConnectTimeout(5000);     // seconds
      pClient->setConnectionParams(12, 24, 0, 60); 
      if (!pClient) {
        Serial.println("Failed to create client."); // Make sure the client was created
        return 0;
      }

      if (pClient->connect(device, true, false)){
         Serial.println("Connected to InfTank!");

        NimBLERemoteService *pService = pClient->getService(serviceUUID);
        if(!pService){
          Serial.println("BLE Error: could not find service.");
          pClient->disconnect();
          NimBLEDevice::deleteClient(pClient);
          pClient = nullptr;         
          return 0;
        }

        pInfLevel = pService->getCharacteristic(infLevelUUID);
        pErrorFlags = pService->getCharacteristic(errorFlagsUUID);

        if (pInfLevel && pInfLevel->canIndicate()) {
          pInfLevel->subscribe(false, infLevelNotifyCB, true);
          Serial.println("Subscribed to infLevel notifications");
        }
        else{
          Serial.println("Can't indicate infLevel.");
          return 0;
        }

        if (pErrorFlags && pErrorFlags->canIndicate()) {
          pErrorFlags->subscribe(false, errorFlagsNotifyCB, true);
          Serial.println("Subscribed to errorFlags notifications");
        }
        else{
          Serial.println("Can't indicate errorFlags.");
          return 0;
        }
        return 1;
      }
      else{
        Serial.println("Connection failed.");
        NimBLEDevice::deleteClient(pClient);
        pClient = nullptr;
        return 0; 
      }    
    }
  }
  Serial.println ("InfTank BLE not found!");
  return 0;
}

void ClientCallbacks::onConnect(NimBLEClient* pClient){
  Serial.println("Connected to server");
}

void ClientCallbacks::onDisconnect(NimBLEClient* pClient, int reason){
  Serial.print("BLE Disconnected! Reason: ");
  Serial.println(reason);
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
        sup_errors |= ( ((100 - sup_current_level) / (sup_current_level - sup_level_at_start)) * TIME_TO_CHECK ) << 8;
      }
    }
    
    //convert water level to percentage
    int percentage;

    if(sup_current_level == -1){
      percentage = 0;
    }
    else{
      percentage = sup_current_level;
    }
    

    //get time
    now = time(nullptr);

    http.addHeader("Content-Type", "application/json");
    http.addHeader("Authorization", "DVC eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJkZXZpY2UiOnsiX2lkIjoiRGlzcG9zaXRpdm8xIiwiZmVhdHVyZXMiOlsiTGl2ZWxsb19BY3F1YSJdLCJ0aGluZ3MiOlsiQ2lzdGVybmFBIiwiQ2lzdGVybmFCIl0sInZpc2liaWxpdHkiOiJwdWJsaWMiLCJvd25lciI6IjY5MWQ5OGZlNGEzYmY4MDAxZWJhMTVhMCJ9LCJ0ZW5hbnQiOnsicGFzc3dvcmRoYXNoIjp0cnVlLCJfaWQiOiJ3YXRlcl9jb250cm9sIiwib3JnYW5pemF0aW9uIjoiTWVhc3VyaWZ5IG9yZyIsImFkZHJlc3MiOiJNZWFzdXJpZnkgU3RyZWV0LCBHZW5vdmEiLCJlbWFpbCI6ImluZm9AbWVhc3VyaWZ5Lm9yZyIsInBob25lIjoiKzM5MTAzMjE4NzkzODE3IiwiZGF0YWJhc2UiOiJ3YXRlcl9jb250cm9sIn0sImlhdCI6MTc2MzczNDQwNCwiZXhwIjozMzMyMTMzNDQwNH0.pW2g-fBg-tYhvdunMrBu4NPDGJYnn3nKWv8Uvq3WxOc");

    // json string build
    String json = "{";
    json += "\"timestamp\": " + String(now*1000) + ",";
    json += "\"values\": [" + String(percentage) + ", " + String(sup_errors) + "]";
    json += "}";

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
    json += "\"timestamp\": " + String(now*1000) + ","; 
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
    case ARDUINO_EVENT_WIFI_STA_GOT_IP: {
      WIFI_ERROR = false;
      Serial.println("WiFi connected!");
      Serial.print("IP: ");
      Serial.println(WiFi.localIP());

      // Set timezone (Italy: CET/CEST)
      setenv("TZ", "CET-1CEST,M3.5.0,M10.5.0/3", 1);
      tzset();

      Serial.println("Getting date and time...");
      configTime(0, 0, "pool.ntp.org", "time.nist.gov");

      // Wait for time sync (non-blocking / yields)
      struct tm timeinfo;
      if (!getLocalTime(&timeinfo, 10000)) {   // 10 sec timeout
        Serial.println("NTP sync FAILED (still epoch)");
      } else {
        Serial.println("Time synced!");
        Serial.println(&timeinfo, "%d/%m/%Y %H:%M:%S");
      }
      break;
    }

    case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
      WIFI_ERROR = true;
      break;
  }
}




