//This code defines the functions of the sensors

int get_level (int sensor_pin); //returns selected tank water level

void start_pump();  //Turns on the pump to fill upper tank
void stop_pump(); //turns of the pump
int check_pumping();  //Checks if upper tank is being filled. Returns 0 for success, 1 for error

void start_source();  //Turns on water to fill lower tank
void stop_source(); //Stops filling the inf tank
int check_source();  //Cecks if lower tank is being filled. Returns 0 for success, 1 for error


