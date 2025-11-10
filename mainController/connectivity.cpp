#include "connectivity.h"
#include "globalVariables.h"
#include <NimBLEDevice.h>

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

//////////////////////////////////////////////////////////////////////////