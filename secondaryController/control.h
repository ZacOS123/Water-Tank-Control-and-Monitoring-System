
//This code defines low level functions
#include "NimBLEDevice.h"

int get_level_inf (); //returns inferior tank water level

void start_source();  //Turns on the pump to fill upper tank
void stop_source(); //turns of the pump
int check_source();  //Checks if upper tank is being filled. Returns 0 for success, 1 for error

class ServerCallbacks: public      //used to advertise even after disconnect
NimBLEServerCallbacks{
  void onDisconnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo, int reason)
  override;
};


