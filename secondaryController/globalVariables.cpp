#include "globalVariables.h"
#include <Arduino.h>
#include <stdint.h>

extern SystemStatus status = WAITING;
extern SystemStatus previous_status = BLOCKING_ERROR; // set to Blocking error for serial monitor update

extern unsigned long source_time = 0;  //in milliseconds, used to check if the system is working "as expected" for the superior tank
extern int inf_level_at_start = 0; //water level in sup tank when the pump started

extern int inf_current_level = 0;  //saves the lower tank's water level for the current loop
extern unsigned short BLE_time = 0; //in milliseconds, used to check elapsed time for BLE between updates

bool SENSOR_ERROR = false;
bool SOURCE_ERROR = false;
bool BLE_ERROR = false;

/*
BLEService systemService("74ac19c2-5aa1-4419-9426-dab1961d0b9f");  // Custom service UUID
BLEShortCharacteristic waterLevel("74ac19c2-5aa1-4419-9426-dab1961d0b9f1", BLERead | BLEIndicate);
BLEUnsignedShortCharacteristic infStatus("74ac19c2-5aa1-4419-9426-dab1961d0b9f2", BLERead | BLEIndicate);
*/

