//BLE and Wifi functions

#include <WiFi.h>
#include <NimBLEDevice.h>

//BLE//
int search_and_connect (NimBLEScanResults res);
void infLevelNotifyCB(NimBLERemoteCharacteristic* pChar, uint8_t* data, size_t length, bool isNotify);
void errorFlagsNotifyCB(NimBLERemoteCharacteristic* pChar, uint8_t* data, size_t length, bool isNotify);

class ClientCallbacks : public NimBLEClientCallbacks {
public:
    void onConnect(NimBLEClient* pClient) override;
    void onDisconnect(NimBLEClient* pClient, int reason) override;
};




//// WiFi and Cloud ////
void update_cloud(); //updates data on cloud (measurify)
void WiFi_error_handler(WiFiEvent_t event, WiFiEventInfo_t info); //checks if wifi is still connected