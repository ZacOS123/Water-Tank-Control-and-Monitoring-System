
#ifndef CONFIG_H
#define CONFIG_H //Prevents double definitions

/////PIN SETUP///////
#define INF_SENSOR_PIN 14 //or A0, water level sensor of the inferior tank is connected to A0
#define SUP_SENSOR_PIN 15 //or A1 water level sensor of the superior tank is connected to A1
#define SOURCE_PIN 11 //pin connected to the water spring valve
#define PUMP_PIN 10 //pin connected to the pump

/////MAX/MIN TANK LEVELS/////
#define INF_TANK_LO 0    //Sensor value when inferior tank is Empty (Low)
#define INF_TANK_HI 1024  //Sensor value when inferior tank is Full (High)
#define SUP_TANK_LO 0         //Sensor value when superior tank is Empty (Low)
#define SUP_TANK_HI 1024      //Sensor value when superior tank is Full (High)

/////MINIMUM VALUES/////
#define MIN_INF_TO_PUMP  INF_TANK_HI/6  //Minimum inferior tank level to pump
#define SUP_ACC_LEVEL    SUP_TANK_HI/6   //Superior tank threshold for pumping (lower than threshold --> pump)
#define TIME_TO_CHECK   10000            //Time in ms to check if pumping or source are working
#define SENSOR_RANGE    10    //divider of the highest value, used to give flexibility because sensor values are precise (divides tank highest level by its value)


#endif

