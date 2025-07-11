#include "globalVariables.h"
#include <Arduino.h>

extern SystemStatus status = WAITING;
extern SystemStatus previous_status = SOURCE_ERROR; //used for serial monitor update

extern unsigned long source_time = 0;  //in milliseconds, used to check if the system is working "as expected" for the inferior tank
extern unsigned long pump_time = 0;  //in milliseconds, used to check if the system is working "as expected" for the superior tank
extern int sup_level_at_start = 0; //water level in sup tank when the pump started
extern int inf_level_at_start = 0; //water level in inf tank when the source started

extern int inf_current_level = 0;  //saves the lower tank's water level for the current loop
extern int sup_current_level = 0;  //saves the upper tank's water level for the current loop


