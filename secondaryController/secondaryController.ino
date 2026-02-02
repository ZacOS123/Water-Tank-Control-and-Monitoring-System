// v.2.0 //

#include "control.h"
#include "config/config.h"
#include "globalVariables.h"
#include "logic.h"
#include <NimBLEDevice.h>



void setup(){
  delay(1000);
  Serial.begin(115200);
  while(!Serial) {
    delay(10);
  }
  Serial.println("\nHello!");

  //Bluetooth setup
  NimBLEDevice::init("InfTank");// Initialize NimBLE
  NimBLEServer* pServer = NimBLEDevice::createServer();// Create BLE server
  NimBLEService* pService = pServer->createService("74ac19c2-5aa1-4419-9426-dab1961d0b9f");// Create a service
  if(!pService){
    Serial.print("Couldn't create Service");
  }
  else{
    ServerCallbacks* pCallbacks = new ServerCallbacks();
    pServer->setCallbacks(pCallbacks);
    pInfLevel = pService->createCharacteristic( // Create a characteristic (read + write)
                            "74ac19c2-5aa1-4419-9426-dab1961d0b91",
                            NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::INDICATE
                        );
    pErrorFlags = pService->createCharacteristic( // Create a characteristic (read + write)
                          "74ac19c2-5aa1-4419-9426-dab1961d0b92",
                          NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::INDICATE
                        );
    if(pInfLevel == nullptr){Serial.print("could not initialize characteristic");}

    pInfLevel->setValue((uint8_t) 0); // Set initial value
    pErrorFlags->setValue((uint16_t) 0); // Set initial value
    pService->start(); // Start the service
    NimBLEAdvertising* pAdvertising = NimBLEDevice::getAdvertising(); // Start advertising
    pAdvertising->addServiceUUID("74ac19c2-5aa1-4419-9426-dab1961d0b9f");
    NimBLEAdvertisementData scanRespData; // Proper way to enable scan response
    pAdvertising->setScanResponseData(scanRespData);
    pAdvertising->setName("InfTank");

    pAdvertising->start(0);
    
    Serial.println("\nBLE Advertising started");
  }
  
  
  //pin setup
  pinMode(INF_TRIG_PIN, OUTPUT);
  pinMode(INF_ECHO_PIN, INPUT);
  pinMode(SOURCE_PIN, OUTPUT);

  //Check Inferior tank sensors
  Serial.println("\nChecking Sensors...");

  if(get_level_inf() == -1){
    SENSOR_ERROR = true;
    Serial.println("ERROR: tank sensor malfunctioning.");
  }
  else{
    Serial.println("Tank sensor working properly.");
    SENSOR_ERROR = false;
  }    
  
  //first data update
  //build error flags//
  uint16_t error_flags = 0;
  if(SENSOR_ERROR){
    error_flags |= 0x01;
  }
  pInfLevel->setValue((uint8_t)get_level_inf()); 
  pErrorFlags->setValue(error_flags); 
  pInfLevel->indicate();
  pErrorFlags->indicate();
  Serial.println("Updated data via BLE.");
  
}

void loop(){
  inf_current_level = get_level_inf(); //inverted due to ultrasonic sensor

  handle_error();
  handle_lower_tank();
  handle_communication();

  

  log();
}