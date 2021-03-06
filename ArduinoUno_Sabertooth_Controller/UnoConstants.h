#ifndef UnoConstants_h
#define UnoConstants_h

#define PWMRate 1.27
#define FORWARD 2
#define STOP 1
#define BACKWARD 0

#define RightMotor 1
#define LeftMotor 2
#define StopSignal 0

#define PI 3.1415
#define WHEEL_DIAMETER 11 // cm
#define ROBOT_TRACK_WIDTH 35 // width of the robot based on the distance between the centers of left and right wheels

#define WHEEL_CERCUMFERENCE 34.5565 // Wheel Diameter = 11 in centimeter and PI = 3.1415
#define VELOCITY_CALC_INTERVAL 1000 //ms
#define TICK_COUNT_PER_REVOLUTION 12000
#define CERCUMFERENCE_REVOLUTION_RATIO 0.0028 //  WHEEL_CERCUMFERENCE / TICK_COUNT_PER_REVOLUTION;
#define HEADING_ANGLE_RATIO 0.0001 //((PI * (WHEEL_DIAMETER / ROBOT_TRACK_WIDTH)) / TICK_COUNT_PER_REVOLUTION)) = 0.00008227738

const float TICK_COUNT_FULL_ROTATION = (ROBOT_TRACK_WIDTH / WHEEL_DIAMETER) * TICK_COUNT_PER_REVOLUTION;

#define ELBOW 5
#define CLAW 6
#define WRIST 4
#define ELBOW_INIT 95
#define CLAW_INIT 180
#define WRIST_INIT 95

#define IntteruptPin_LeftEncoder 0
#define InterruptPin_RightEncoder 1

#define PAN 11
#define TILT 12
#define PAN_INIT 90
#define TILT_INIT 120

#define LASER 10

#define ServoDelayValue 5
#define ServoAdjustRate 10
#define ClawMin 0

#define MaxTiltAngle 150
#define MaxElbowAngle 140
#define MaxOtherServoAngle 180
#define MinServoAngle 30
#define MinPanAngle 0

#endif
