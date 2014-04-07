#ifndef Constants_h
#define Constants_h

#define LeftMotor  1
#define RightMotor 2
#define SpeedScale 2 //scaling factor to adjust the speed of the motors

#define PI 3.1415
#define WHEEL_CERCUMFERENCE 34.5565 // Wheel Diameter = 11 in centimeter and PI = 3.1415
#define TICK_COUNT_PER_REVOLUTION 12000
#define VELOCITY_CALC_INTERVAL 1000 //ms
#define CERCUMFERENCE_REVOLUTION_RATIO 0.0028 //  WHEEL_CERCUMFERENCE / TICK_COUNT_PER_REVOLUTION;
#define HEADING_ANGLE_RATIO 0.0001 //((PI * (WHEEL_DIAMETER / ROBOT_TRACK_WIDTH)) / TICK_COUNT_PER_REVOLUTION)) = 0.00008227738

#define LOWBAT 550    // default low battery voltage is 5.5V
#define BATTERYINFOPUBLISH 9999
#define ACCELVALUEPUBLISH 999
#define IRANGEDATAPUBLISH 50

#define sensitivity 50  //minimum magnitude required to register as a n impact

#define MAXRANGE 254 //used for IR sensor
#define SAMPLEUPPERBOUND 10

#define ELBOW_INIT 95
#define CLAW_INIT 180
#define WRIST_INIT 95

#define PAN_INIT 90
#define TILT_INIT 120

#define ServoAdjustRate 10
#define ClawMin 0

#define MaxTiltAngle 150
#define MaxElbowAngle 140
#define MaxOtherServoAngle 180
#define MinServoAngle 30
#define MinPanAngle 0

#endif