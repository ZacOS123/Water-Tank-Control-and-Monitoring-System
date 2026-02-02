#include "globalVariables.h"
#include <Arduino.h>

time_t now = 0;
unsigned long update_time = 0; //used to update cloud every TIME_TO_UPDATE
SystemStatus status = WAITING;
SystemStatus previous_status = PUMPING; //used for serial monitor update. Starts with pumping to update log

unsigned long pump_time;  //in milliseconds, used to check if the system is working "as expected" for the superior tank
int sup_level_at_start; //water level in sup tank when the pump started

int inf_current_level;  //saves the lower tank's water level for the current loop
int sup_current_level;  //saves the upper tank's water level for the current loop
uint16_t inf_errors = 0x03;  //saves lower controller and tank status


// WiFi credentials
const char* ssid = "Zahi's A32";
const char* password = "luciusmalfoy333";


//BLE UUIDs//
NimBLEUUID serviceUUID("74ac19c2-5aa1-4419-9426-dab1961d0b9f");
NimBLEUUID infLevelUUID("74ac19c2-5aa1-4419-9426-dab1961d0b91");
NimBLEUUID errorFlagsUUID("74ac19c2-5aa1-4419-9426-dab1961d0b92");

// Characteristic and server pointers
NimBLEClient *pClient = nullptr;
NimBLERemoteCharacteristic* pInfLevel = nullptr;
NimBLERemoteCharacteristic* pErrorFlags = nullptr;


//Error Flags//
bool SENSOR_ERROR = true;
bool PUMP_ERROR = false;
bool BLE_ERROR = true;
bool WIFI_ERROR = true;
