#include "config/config.h"
#include <stdint.h>
#include <ArduinoBLE.h>

#ifndef CONSTANTS_H
#define CONSTANTS_H //Prevents double definitions

typedef enum {
	WAITING, //All tanks are full and the system is waiting OR system is starting up
  FILLING, //Filling inferior tank with source
  SENSOR_ERROR, //unexpected values from lower tank sensor
  SOURCE_ERROR, //No water input to the lower tank
  SENSOR_SOURCE_ERROR, //Both source and sensor not working
  BLE_ERROR, //BLE not connected
} SystemStatus;

extern SystemStatus status;
extern SystemStatus previous_status; //used for serial monitor update
extern int inf_current_level;  //saves the lower tank's air level for the current loop
extern int inf_level_at_start; //water level in sup tank when the pump started
extern unsigned long source_time;  //in milliseconds, used to check if the system is working "as expected" for the superior tank
extern unsigned short BLE_time; //in milliseconds, used to check elapsed time for BLE between updates

extern BLEService systemService;
extern BLEUnsignedShortCharacteristic waterLevel;
extern BLEUnsignedShortCharacteristic infStatus;

/*
uint8_t errorFlags; //integer to save error flags (bitwise)
#define SENSOR_ERROR      0x01  // bit 0
#define SOURCE_ERROR  0x02  // bit 1
#define BLE_ERROR    0x03  // bit 2
//use bitwise operators to change errorFlags: if 101 --> sensor and BLE error...
*/

#endif

