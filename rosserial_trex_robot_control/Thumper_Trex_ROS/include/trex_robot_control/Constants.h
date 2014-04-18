#define LeftMotor  1
#define RightMotor 2
#define SpeedScale 2 //scaling factor to adjust the speed of the motors

#define LOWBAT 550    // default low battery voltage is 5.5V
#define BATTERYINFOPUBLISH 9999
#define ACCELVALUEPUBLISH 999
#define IRANGEDATAPUBLISH 50

#define sensitivity 50  //minimum magnitude required to register as a n impact

#define MAXRANGE 254 //used for IR sensor
#define SAMPLEUPPERBOUND 10

#define BATTERYINFOPUBLISH 9999

#define ULTRASONIC_cm 0 // convert the sonar readings to cm
#define ULTRASONIC_inch 1 // convert the sonar readings to inch
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
#define pingSpeed  50 // frequency to send out a ping (in milliseconds). 50ms would be 20 times a second.