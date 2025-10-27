#include "config/config.h"

#ifndef CONSTANTS_H
#define CONSTANTS_H //Prevents double definitions

typedef enum {
	WAITING, //All tanks are full and the system is waiting OR system is starting up
  PUMPING, //Pump ON and pumping
	SENSOR_ERROR,  //unexpected values from upper tank sensors
	PUMP_ERROR,  //Tank not filling while pump is ON
  SENSOR_PUMP_ERROR, //both Pump and sensor not working
} SystemStatus;

extern SystemStatus status;
extern SystemStatus previous_status; //used for serial monitor update
extern unsigned long pump_time;  //in milliseconds, used to check if the system is working "as expected" for the superior tank
extern int sup_level_at_start; //air level in sup tank when the pump started
extern int inf_current_level;  //saves the lower tank's air level for the current loop
extern int sup_current_level;  //saves the upper tank's air level for the current loop
extern int lower_status; //saves lower controller and tank status


#endif

