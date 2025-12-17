#include "globalVariables.h"
#include <stdint.h>
#include <NimBLEDevice.h>

extern SystemStatus status = WAITING;
extern SystemStatus previous_status = BLOCKING_ERROR; // set to Blocking error for serial monitor update

extern unsigned long source_time = 0;  //in milliseconds, used to check if the system is working "as expected" for the superior tank
extern int inf_level_at_start = 0; //water level in sup tank when the pump started

extern int inf_current_level = 0;  //saves the lower tank's water level for the current loop
extern unsigned long BLE_time = 0; //in milliseconds, used to check elapsed time for BLE between updates

bool SENSOR_ERROR = false;
bool SOURCE_ERROR = false;

NimBLECharacteristic* pInfLevel = nullptr;
NimBLECharacteristic* pErrorFlags = nullptr;

