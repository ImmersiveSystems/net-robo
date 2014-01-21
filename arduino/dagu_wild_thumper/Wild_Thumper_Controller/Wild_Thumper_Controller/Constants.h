// Baud rate for serial communications
#define Brate 115200     

#define VoltageScale 68
#define SpeedScale 2.25
#define MinLimit 50.0
#define MaxStraightLimit 250.0
#define MaxInitStraightMove 127.0
#define InitStablePWM 60.0
#define InitStableTurn 90.0
#define InitStableTurnPWM 127.0
#define TurnRate 1.0
#define ShiftRate 5.0
#define InitMoveAdjustValue 1.0
#define AccelRate 10.0
#define DeccelRate 0.001

 // This is the nominal battery voltage reading. Peak charge can only occur above this voltage.
#define batvolt 884 // 544 
 // This is the voltage at which the speed controller goes into recharge mode.
#define lowvolt 680 // 455
#define peakvolt 918 // 571
// If the battery voltage does not change in this number of milliseconds then stop charging.
#define chargetimeout 300000 

// set overload current for left motor 
#define Leftmaxamps 800  
// set overload current for right motor    
#define Rightmaxamps 800    
// time in mS before motor is re-enabled after overload occurs 
#define overloadtime 100     

// default position for servo6 on "power up" - 1500uS is center position on most servos
#define DServo0 1500
#define DServo1 1500 
#define DServo2 1500
#define DServo3 1500
#define DServo4 1500
#define DServo5 1500
#define DServo6 1500