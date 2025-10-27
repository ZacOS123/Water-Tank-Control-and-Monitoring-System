#include "globalVariables.h"
#include <Arduino.h>

extern SystemStatus status = WAITING;
extern SystemStatus previous_status = PUMPING; //used for serial monitor update. Starts with pumping to update log

unsigned long pump_time;  //in milliseconds, used to check if the system is working "as expected" for the superior tank
int sup_level_at_start; //water level in sup tank when the pump started

int inf_current_level;  //saves the lower tank's water level for the current loop
int sup_current_level;  //saves the upper tank's water level for the current loop
int lower_status;  //saves lower controller and tank status

