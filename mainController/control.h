//This code defines the functions of the sensors

int get_level_sup (); //returns upper tank water level
void start_pump();  //Turns on the pump to fill upper tank
void stop_pump(); //turns of the pump
int check_pumping();  //Checks if upper tank is being filled. Returns 0 for success, 1 for error

