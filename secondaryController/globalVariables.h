#include "config/config.h"
#include <stdint.h>
#include <ArduinoBLE.h>

#ifndef CONSTANTS_H
#define CONSTANTS_H //Prevents double definitions

typedef enum {
	WAITING, //All tanks are full and the system is waiting OR system is starting up
  FILLING, //Filling inferior tank with source
  BLOCKING_ERROR, //A blocking error occured. Check error flags
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

extern bool SENSOR_ERROR;
extern bool SOURCE_ERROR;
extern bool BLE_ERROR;


#endif

