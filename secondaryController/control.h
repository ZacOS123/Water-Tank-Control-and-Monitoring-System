//This code defines the functions of the sensors

int get_level_inf (); //returns inferior tank water level
//int update_data_BLE (); //sends a data update to main controller

void start_source();  //Turns on the pump to fill upper tank
void stop_source(); //turns of the pump
int check_source();  //Checks if upper tank is being filled. Returns 0 for success, 1 for error

