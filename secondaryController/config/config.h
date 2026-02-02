
#ifndef CONFIG_H
#define CONFIG_H //Prevents double definitions

/////PIN SETUP///////
#define INF_TRIG_PIN 13 //Ultraasonic trigger pin for lower tank
#define INF_ECHO_PIN 14 //Ultrasonic echo pin for lower tank


#define MEASURE_NUM 1 //Number of ultrasonic reading before calculating the mean (for accurate reading)
                      //Big numbers take more time!

#define SOURCE_PIN 27 //pin connected to the spring water valve

/////MAX/MIN TANK LEVELS/////
/// ! VALUES ARE INVERTED DUE TO ULTRASONIC TOP TO BOTTOM MEASUREMENT ! ////
#define INF_SENSOR_LO 3    //Ultrasonic low values (Tank FULL)
#define INF_SENSOR_HI 27  //Ultrasonic High values (Tank EMPTY)

/////MINIMUM VALUES/////
#define TIME_TO_CHECK   300000            //Time in ms to check if pumping or source are working
#define SENSOR_RANGE    10    //divider of the highest value, used to give flexibility because sensor values are precise (divides tank highest level by its value)
#define TIME_TO_UPDATE  5000  // time to update BLE data

#endif

