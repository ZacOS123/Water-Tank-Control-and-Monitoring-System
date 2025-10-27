
#ifndef CONFIG_H
#define CONFIG_H //Prevents double definitions

/////PIN SETUP///////
#define INF_TRIG_PIN 6 //Ultraasonic trigger pin for lower tank
#define INF_ECHO_PIN 7 //Ultrasonic echo pin for lower tank

#define SUP_TRIG_PIN 8 //Ultrasonic trigger pin for upper tank
#define SUP_ECHO_PIN 9 //Ultrasonic echo pin for upper tank

#define MEASURE_NUM 5 //Number of ultrasonic reading before calculating the mean (for accurate reading)
                      //Big numbers take more time!

#define SOURCE_PIN 11 //pin connected to the spring water valve
#define PUMP_PIN 10 //pin connected to the pump

/////MAX/MIN TANK LEVELS/////
/// ! VALUES ARE INVERTED DUE TO ULTRASONIC TOP TO BOTTOM MEASUREMENT ! ////
#define SUP_SENSOR_LO 0     //Ultrasonic low values (TANK FULL)
#define SUP_SENSOR_HI 1024  //Ultrasonic high values (TANK EMPTY)

/////MINIMUM VALUES/////
#define MIN_INF_TO_PUMP  20  //Minimum percentage of inferior tank level to pump
#define SUP_ACC_LEVEL    (SUP_SENSOR_HI - SUP_SENSOR_LO)/4   //Superior tank threshold for pumping (lower than threshold --> pump)
#define TIME_TO_CHECK   10000            //Time in ms to check if pumping or source are working
#define SENSOR_RANGE    10    //divider of the highest value, used to give flexibility because sensor values are precise (divides tank highest level by its value)


#endif

