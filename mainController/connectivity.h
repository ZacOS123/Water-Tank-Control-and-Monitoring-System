//BLE and Wifi functions

#include <WiFi.h>

//// BLE //////
/*
int BLE_scan_and_connect(); //Scans and connects to inf tank. returns -1 for error, 0 for success
int get_inf_data(); //updates local lower tank data
*/
//// WiFi and Cloud ////
void update_cloud(); //updates data on cloud (measurify)
void WiFi_error_handler(WiFiEvent_t event, WiFiEventInfo_t info); //checks if wifi is still connected