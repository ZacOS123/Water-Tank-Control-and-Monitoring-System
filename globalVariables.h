#include "config.h"

#ifndef CONSTANTS_H
#define CONSTANTS_H //Prevents double definitions

typedef enum {
	WAITING, //All tanks are full and the system is waiting OR system is starting up
  PUMPING, //Pump ON and pumping
  FILLING, //Filling inferior tank with source
	PUMPING_FILLING, //Pump ON and filling inferior tank
	PROGRAM_ERROR,  //values error
	SENSOR_ERROR,  //unexpected values from sensors
	PUMP_ERROR,  //Tank not filling while pump is ON
  SOURCE_ERROR, //No water input to the lower tank
} SystemStatus;

extern SystemStatus status;
extern SystemStatus previous_status; //used for serial monitor update
extern unsigned long source_time;  //in milliseconds, used to check if the system is working "as expected" for the inferior tank
extern unsigned long pump_time;  //in milliseconds, used to check if the system is working "as expected" for the superior tank
extern int sup_level_at_start; //water level in sup tank when the pump started
extern int inf_level_at_start; //water level in inf tank when the source started
extern int inf_current_level;  //saves the lower tank's water level for the current loop
extern int sup_current_level;  //saves the upper tank's water level for the current loop


#endif

