#include "config/config.h"
#include <Arduino.h>
#include <NimBLEDevice.h>

#ifndef CONSTANTS_H
#define CONSTANTS_H //Prevents double definitions

typedef enum {
	WAITING, //All tanks are full and the system is waiting OR system is starting up
  PUMPING, //Pump ON and pumping
  BLOCKING_ERROR, // blocking error occured, system interrupted.
} SystemStatus;

extern time_t now;
extern unsigned long update_time;
extern SystemStatus status;
extern SystemStatus previous_status; //used for serial monitor update
extern unsigned long pump_time;  //in milliseconds, used to check if the system is working "as expected" for the superior tank
extern int sup_level_at_start; //air level in sup tank when the pump started
extern int inf_current_level;  //saves the lower tank's air level for the current loop
extern int sup_current_level;  //saves the upper tank's air level for the current loop
extern uint16_t inf_errors; //saves error flags: bit 0 = SENSOR_ERROR, bit 1 = SOURCE_ERROR,

extern const char* ssid;
extern const char* password;

extern NimBLEUUID serviceUUID;
extern NimBLEUUID infLevelUUID;
extern NimBLEUUID errorFlagsUUID;
// Characteristic and server pointers
extern NimBLEClient *pClient;
extern NimBLERemoteCharacteristic* pInfLevel;
extern NimBLERemoteCharacteristic* pErrorFlags;

extern bool SENSOR_ERROR;
extern bool PUMP_ERROR;
extern bool BLE_ERROR;
extern bool WIFI_ERROR;

#endif

