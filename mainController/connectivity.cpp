#include "connectivity.h"
#include "globalVariables.h"
#include <NimBLEDevice.h>

void BLE_scan(){  //start BLE and scan for devices

  NimBLEScan *pScan = NimBLEDevice::getScan();
  NimBLEScanResults results = pScan->getResults(15 * 1000);  //scan for 15 secs

  NimBLEUUID serviceUuid("74ac19c2-5aa1-4419-9426-dab1961d0b9f");
}

//////////////////////////////////////////////////////////////////////////

int BLE_connect(){ //connects to inf tank controller. Return -1 for error, 0 for success
  for (int i = 0; i < results.getCount(); i++) {
    const NimBLEAdvertisedDevice *device = results.getDevice(i);

    //Server found//
    if (device->isAdvertisingService(serviceUuid)) {
      NimBLEClient *pClient = NimBLEDevice::createClient();

      if (!pClient) { // client not created
        Serial.println("Unable to initiate connection: unable to create client (locally).");
        return -1;
      }

      if (pClient->connect(&device)) {  //if was able to connect 
        NimBLERemoteService *pService = pClient->getService(serviceUuid);
        return 0;
      }
      else { //if wasnt able to connect
        Serial.println("Failed to connect to server.");
        return -1;
      }
    }

    //Server not found//
    if(i == results.getCount()-1 && !(device->isAdvertisingService(serviceUuid))){
      Serial.println("Could not find BLE server.");
      return -1;
    }
  }
} 

//////////////////////////////////////////////////////////////////////////

int get_inf_data (){  //returns inferior tank water level
  if (pService != nullptr) {
    NimBLERemoteCharacteristic *pInf_level = pService->getCharacteristic("74ac19c2-5aa1-4419-9426-dab1961d0b9f1");
    NimBLERemoteCharacteristic *pError_flags = pService->getCharacteristic("74ac19c2-5aa1-4419-9426-dab1961d0b9f1");

    if(pError_flags != nullptr){
      std::string value = pCharacteristic->readValue();
      inf_errors = value;  //LSB = SENSOR_ERROR.    2nd LSB = SOURCE_ERROR
    }
    else{
      Serial.println("Could not store lower tank error flags: can't create BLE characteristic.");
    }

    if (pInf_level != nullptr && !(inf_errors & 0x01)) {  //if char exists and no sensor error
      std::string value = pCharacteristic->readValue();
      inf_current_level = value;
    }
        else{
      Serial.println("Could not store lower tank water level: can't create BLE characteristic.");
    }
  }
  else{
    Serial.println("Could not access lower tank data: can't create BLE service.");
  }
} 

//////////////////////////////////////////////////////////////////////////
