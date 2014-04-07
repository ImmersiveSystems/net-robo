// RobotController.ino

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Servo.h>

#include "dagu_trex_atmega328p/RobotController.h"
#include "dagu_trex_atmega328p/IOpins.h"
#include "dagu_trex_atmega328p/Constants.h"

byte LeftMotorBrake;
byte RightMotorBrake;

unsigned long RobotController::VoltageTimer;
unsigned long RobotController::AccelTimer;
unsigned long RobotController::IRangeTimer;
unsigned long RobotController::VelocityTimer;

Servo servoElbow;  // create servo object to control a servo (max. of eight objects)
Servo servoClaw;
Servo servoWrist;

Servo servoPan;
Servo servoTilt;


int RobotController::ElbowPos_Cur;
int RobotController::ClawPos_Cur;
int RobotController::WristPos_Cur;

int RobotController::PanPos_Cur;
int RobotController::TiltPos_Cur;

float RobotController::LeftWheelVelocity;	
float RobotController::RightWheelVelocity;
float RobotController::RobotVelocity;

long RobotController::Robot_XCoord;
long RobotController::Robot_YCoord;
float RobotController::Robot_HeadingAngle;

static byte LeftEncoderReading_old;
static byte LeftEncoderReading_new;
static byte RightEncoderReading_old;
static byte RightEncoderReading_new;

float RobotController::LeftMotorEncoderValue;
float RobotController::RightMotorEncoderValue;
float RobotController::LeftMotorEncoderValue_prev;
float RobotController::RightMotorEncoderValue_prev;
float RobotController::LeftMotorEncoderPos_TrackPrev;
float RobotController::RightMotorEncoderPos_TrackPrev;

RobotController::RobotController(): ElbowPin(ELBOW), ClawPin(CLAW), WristPin(WRIST), ElbowInitPos(ELBOW_INIT), ClawInitPos(CLAW_INIT), WristInitPos(WRIST_INIT), PanPin(PAN), TiltPin(TILT), PanInitPos(PAN_INIT), TiltInitPos(TILT_INIT)
{
	LeftMode = 0;
	RightMode = 0;		
	LeftMotorSpeed = 0;
	RightMotorSpeed = 0;

	LeftMotorBrake = 0;
	RightMotorBrake = 0;	

	LeftMotorCur = 0;
	RightMotorCur = 0;
	voltage = 0.0;

	LeftWheelVelocity = 0.0;	
  	RightWheelVelocity = 0.0;
  	RobotVelocity = 0.0;

  	LeftWheelVelocity = 0.0;	
	RightWheelVelocity = 0.0;
  	RobotVelocity = 0.0;

  	Robot_XCoord = 0;
  	Robot_YCoord = 0;
  	Robot_HeadingAngle = 0;

	magnitude = 0;
	Xaxis = 0;
	Yaxis = 0;
	Zaxis = 0;
	dX = 0;
	dY = 0;
	dZ = 0;

	VoltageTimer = 0;
	AccelTimer = 0;
	IRangeTimer = 0;
	VelocityTimer = 0;

	LeftEncoderReading_old = 0;
	LeftEncoderReading_new = 0;
	RightEncoderReading_old = 0;
	RightEncoderReading_new = 0;

	LeftMotorEncoderValue = 0;
	RightMotorEncoderValue = 0;
	LeftMotorEncoderValue_prev = 0;
	RightMotorEncoderValue_prev = 0;	
	LeftMotorEncoderPos_TrackPrev = 0;
	RightMotorEncoderPos_TrackPrev = 0;
}

void RobotController::InitServosCurPos()
{
	ElbowPos_Cur = ElbowInitPos;
	ClawPos_Cur = ClawInitPos;
	WristPos_Cur = WristInitPos;

	PanPos_Cur = PanInitPos;
	TiltPos_Cur = TiltInitPos;
}

void RobotController::InitServos()
{
  servoElbow.attach(ElbowPin);
  servoClaw.attach(ClawPin);  
  servoWrist.attach(WristPin);

  servoElbow.writeMicroseconds(ElbowInitPos);
  servoClaw.writeMicroseconds(ClawInitPos);  
  servoWrist.writeMicroseconds(WristInitPos);

  servoPan.attach(PanPin);
  servoTilt.attach(TiltPin);

  servoPan.writeMicroseconds(PanInitPos);
  servoTilt.writeMicroseconds(TiltInitPos);	

  InitServosCurPos();
}

void RobotController::InitMotorsPins()
{
  pinMode(LEFTMOTORPWM_pin,OUTPUT);          // configure left  motor PWM       pin for output
  pinMode(LEFTMOTORDIRECTION_pin,OUTPUT);   // configure left  motor direction pin for output
  pinMode(LEFTMOTORBRAKE_pin,OUTPUT);      // configure left  motor brake     pin for output
  
  pinMode(RIGHTMOTORPWM_pin,OUTPUT);        // configure right motor PWM       pin for output
  pinMode(RIGHTMOTORDIRECTION_pin,OUTPUT); // configure right motor direction pin for output
  pinMode(RIGHTMOTORBRAKE_pin,OUTPUT);     // configure right motor brake     pin for output	
}
/*
void RobotController::InitIRangeData(sensor_msgs::Range &range_msg)
{
	char frameid[] = "/ir_ranger";

	range_msg.radiation_type = sensor_msgs::Range::INFRARED;
  	range_msg.header.frame_id =  frameid;
  	range_msg.field_of_view = 0.01;
  	range_msg.min_range = 0.03;
  	range_msg.max_range = 0.4;	
}
*/
void RobotController::InitEncoders()
{
	pinMode(LEFTMOTORENCODER_pin, INPUT);
	digitalWrite(LEFTMOTORENCODER_pin, HIGH); // enables pull-up resistor
	pinMode(RIGHTMOTORENCODER_pin, INPUT);
	digitalWrite(RIGHTMOTORENCODER_pin, HIGH); // enables pull-up resistor  
        
        LeftEncoderReading_new = digitalRead(LEFTMOTORENCODER_pin);
        RightEncoderReading_new = digitalRead(RIGHTMOTORENCODER_pin);
}

void RobotController::UpdateEncoderValues(dagu_trex_atmega328p::TrexRobotData &robPwr_msg)//(dagu_trex_atmega328p::TrexPowerMsg &encm)
{	
	LeftEncoderReading_old = LeftEncoderReading_new;
	RightEncoderReading_old = RightEncoderReading_new;

	LeftEncoderReading_new = digitalRead(LEFTMOTORENCODER_pin); 
	RightEncoderReading_new = digitalRead(RIGHTMOTORENCODER_pin);

	if (LeftEncoderReading_old != LeftEncoderReading_new)
	{// if old does not equal new then add or subtract 1 depending on motor direction	
        if (LeftMode == 2)	
        {
    		LeftMotorEncoderValue = LeftMotorEncoderValue + 1;//LeftMotorSpeed/abs(LeftMotorSpeed);
        }
        else if(LeftMode == 0)
        {
            LeftMotorEncoderValue = LeftMotorEncoderValue - 1;//LeftMotorSpeed/abs(LeftMotorSpeed);
        }
	} 
	if (RightEncoderReading_old != RightEncoderReading_new)
	{// if old does not equal new then add or subtract 1 depending on motor direction
        if(RightMode == 2)
        {
            RightMotorEncoderValue = RightMotorEncoderValue + 1; //+ RightMotorSpeed/abs(RightMotorSpeed);
        }
        else if (RightMode == 0)
        {
	        RightMotorEncoderValue = RightMotorEncoderValue - 1; //+ RightMotorSpeed/abs(RightMotorSpeed);
        }
	}

	robPwr_msg.left_encoder = LeftMotorEncoderValue;
	robPwr_msg.right_encoder = RightMotorEncoderValue;
}

void RobotController::IRScan(dagu_trex_atmega328p::TrexRobotData &robPwr_msg)//(sensor_msgs::Range &range_msg)
{
	int sample;
        // Get data
	sample = analogRead(IR_pin) / 4;
  	// if the ADC reading is too low, then we are really far away from anything
  	if(sample < SAMPLEUPPERBOUND)
  	{
	    robPwr_msg.irange_dist = MAXRANGE;     // max range
            return;		
  	}   	

  	sample = 1309 / (sample - 3); // Magic numbers to get cm
  	robPwr_msg.irange_dist =  (sample - 1) / 100; //convert to meters
}

int RobotController::ShutdownMotors()
{	
	LeftMotorSpeed = 0;      // set left  motor speed to 0 (off)
	RightMotorSpeed = 0;      // set right motor speed to 0 (off)
	ProcessMotorCommand(LeftMotor);       // update H bridges
	ProcessMotorCommand(RightMotor);       // update H bridges
}

int RobotController::CheckMsgValidity(const dagu_trex_atmega328p::TrexServoMotorCmds &nmc)
{ // H -> 72  P -> 80  T -> 84  E -> 69  C -> 67   W -> 87 Z -> 90
	if((int)nmc.cmd_type == 72 || (int)nmc.cmd_type == 80 || (int)nmc.cmd_type == 84 || (int)nmc.cmd_type == 69 ||(int)nmc.cmd_type == 67 || (int)nmc.cmd_type == 87 || (int)nmc.cmd_type == 90)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int RobotController::SerialCommunicate(const dagu_trex_atmega328p::TrexServoMotorCmds &nmc)
{
    int StatusFlag = 0;
    switch((int)nmc.cmd_type)
    {
      case 72: //i.e., "H"
      	LeftMotorBrake = 0;
	RightMotorBrake = 0;
        SetPWM(LeftMotor, nmc);
        SetPWM(RightMotor, nmc);
        StatusFlag = 1;
        break;

      case 80: //i.e., "P"
        PanPos_Cur = (int)nmc.pan_pos; //data;          
        ProcessServoCommand(PanPin);
        StatusFlag = 1;
        break;
        
      case 84: //i.e., "T"
        TiltPos_Cur = (int)nmc.tilt_pos; //data;          
        ProcessServoCommand(TiltPin);
        StatusFlag = 1;
        break;
        
      case 69: //i.e., "E"
        if(nmc.elbow_move == 1) //(data == 1)
        {
          ElbowPos_Cur = ElbowPos_Cur - ServoAdjustRate;
        }
        else if(nmc.elbow_move == 0)  //(data == 0)
        {
          ElbowPos_Cur = ElbowPos_Cur + ServoAdjustRate;
        }
        ProcessServoCommand(ElbowPin);
        StatusFlag = 1;
        break;
        
      case 67: //i.e., "C"
        if(ClawPos_Cur == 0)
        {
          ClawPos_Cur = MaxOtherServoAngle;
        }
        else if(ClawPos_Cur == 180)
        {
          ClawPos_Cur = ClawMin;
        }
        servoClaw.writeMicroseconds(ClawPos_Cur);
        StatusFlag = 1;
        break;

      case 87: //i.e., "W"
        if(nmc.wrist_move == 1)  //(data == 1)
        {
          WristPos_Cur = WristPos_Cur - ServoAdjustRate;
        }
        else if(nmc.wrist_move == 0)  //(data == 0)
        {
          WristPos_Cur = WristPos_Cur + ServoAdjustRate;
        }
        ProcessServoCommand(WristPin);
        StatusFlag = 1;        
        break;

      case 90: //i.e., "Z"
      	ShutdownMotors();
        StatusFlag = 1;      
        break; 

      default:
        break;
     }
     
     return StatusFlag; 
}

void RobotController::SetPWM(int WhichMotor, const dagu_trex_atmega328p::TrexServoMotorCmds &nmc)
{
	switch(WhichMotor)
	{
		case LeftMotor:
		  LeftMode = (int)nmc.left_mode;
		  LeftMotorSpeed = (int)nmc.left_speed * SpeedScale;	
		  break;

		case RightMotor:
		  RightMode = (int)nmc.right_mode;
		  RightMotorSpeed = (int)nmc.right_speed * SpeedScale;  
		  break;

		default:
		  break;
	}
}

void RobotController::ProcessMotorCommand(int WhichMotor)
{
	switch(WhichMotor)
	{
		case LeftMotor:
		  digitalWrite(LEFTMOTORBRAKE_pin,LeftMotorBrake > 0);                     // if left brake>0 then engage electronic braking for left motor
		  digitalWrite(LEFTMOTORDIRECTION_pin,LeftMode == 2);                     // if left speed>0 then left motor direction is forward else reverse
		  analogWrite (LEFTMOTORPWM_pin,abs(LeftMotorSpeed));                  // set left PWM to absolute value of left speed - if brake is engaged then PWM controls braking
		  break;

		case RightMotor:
		  digitalWrite(RIGHTMOTORBRAKE_pin,RightMotorBrake > 0);                     // if left brake>0 then engage electronic braking for left motor
		  digitalWrite(RIGHTMOTORDIRECTION_pin,RightMode == 2);                     // if left speed>0 then left motor direction is forward else reverse
		  analogWrite (RIGHTMOTORPWM_pin,abs(RightMotorSpeed));                  // set left PWM to absolute value of left speed - if brake is engaged then PWM controls braking
		  break;

	}
}

void RobotController::ProcessServoCommand(int PinNum)
{
	if(PinNum == PanPin)
	{
		PanPos_Cur = ValidateServoCurPos(PinNum, PanPos_Cur);
		servoPan.writeMicroseconds(PanPos_Cur);
	}
	else if(PinNum == TiltPin)
	{
		TiltPos_Cur = ValidateServoCurPos(PinNum, TiltPos_Cur);
		servoTilt.writeMicroseconds(TiltPos_Cur);
	}
	else if(PinNum == ElbowPin)
	{
		ElbowPos_Cur = ValidateServoCurPos(PinNum, ElbowPos_Cur);
		servoElbow.writeMicroseconds(ElbowPos_Cur);
	}
	else if(PinNum == ClawPin)
	{
		ClawPos_Cur = ValidateServoCurPos(PinNum, ClawPos_Cur);
		servoClaw.writeMicroseconds(ClawPos_Cur);
	}
	else if(PinNum == WristPin)
	{
		WristPos_Cur = ValidateServoCurPos(PinNum, WristPos_Cur);
		servoWrist.writeMicroseconds(WristPos_Cur);
	}	
}

int RobotController::ValidateServoCurPos(int PinNum, int POS)
{
	if(PinNum == TiltPin && POS > MaxTiltAngle)
	{
		return MaxTiltAngle;
	}
	else if(PinNum == ElbowPin && POS > MaxElbowAngle)
	{
		return MaxElbowAngle;
	}
	else if((PinNum != TiltPin && PinNum != ElbowPin) && POS > MaxOtherServoAngle)
	{
		return MaxOtherServoAngle;
	}
	else if(PinNum == PanPin && POS < MinPanAngle)
	{
		return MinPanAngle;
	}
	else if(PinNum != PanPin && POS < MinServoAngle)
	{
		return MinServoAngle;
	}
	else
	{
		return POS;
	}
}

void RobotController::MonitorVoltageLevel(dagu_trex_atmega328p::TrexRobotData &robPwr_msg)
{

	// read  left motor current sensor and convert reading to mA
	LeftMotorCur = (analogRead(LEFTMOTORCURRENT_pin) - 511) * 48.83;          
	// read right motor current sensor and convert reading to mA
	RightMotorCur = (analogRead(RIGHTMOTORCURRENT_pin) - 511) * 48.83;          
	// read battery level and convert to volts with 2 decimal places (eg. 1007 = 10.07 V)	
	voltage = analogRead(VOLTAGE_pin) * 10 / 3.357;             
	
	robPwr_msg.voltage_level = voltage;

	robPwr_msg.timer_msg = millis();

	if(voltage < LOWBAT) 
	{
		ShutdownMotors();
	}		
}

void RobotController::CalculateRobotVelocity(dagu_trex_atmega328p::TrexRobotData &robPwr_msg)
{	
	float RightWheel_DeltaTick = RightMotorEncoderValue - RightMotorEncoderValue_prev;// encoderPos[1] - encoderPos_Prev[1];
	RightMotorEncoderValue_prev = RightMotorEncoderValue;  //encoderPos_Prev[1] = encoderPos[1];
	float RightWheelDistance =  (float)((RightWheel_DeltaTick * WHEEL_CERCUMFERENCE) / TICK_COUNT_PER_REVOLUTION); //(float)(RightWheel_DeltaTick * DISTANCE_PER_COUNT);//
	RightWheelVelocity = RightWheelDistance / VELOCITY_CALC_INTERVAL;

	float LeftWheel_DeltaTick = LeftMotorEncoderValue - LeftMotorEncoderValue_prev;  //encoderPos[0] - encoderPos_Prev[0];
	LeftMotorEncoderValue_prev = LeftMotorEncoderValue;  //encoderPos_Prev[0] = encoderPos[0];
	float LeftWheelDistance = (float)((LeftWheel_DeltaTick * WHEEL_CERCUMFERENCE) / TICK_COUNT_PER_REVOLUTION); //(float)(LeftWheel_DeltaTick * DISTANCE_PER_COUNT);//
	LeftWheelVelocity = LeftWheelDistance / VELOCITY_CALC_INTERVAL;

	robPwr_msg.robot_velocity = sqrt(pow(LeftWheelVelocity, 2) + pow(RightWheelVelocity, 2));	 //meter per sec
}

void RobotController::AdjustHeadingAngle()
{
	if(Robot_HeadingAngle > PI)
	{
		Robot_HeadingAngle = Robot_HeadingAngle - (2 * PI);
	}
	else if(Robot_HeadingAngle <= (-1) * PI)
	{
		Robot_HeadingAngle = Robot_HeadingAngle + (2 * PI);
	}
}

void RobotController::TrackRobot(dagu_trex_atmega328p::TrexRobotData &robPwr_msg)
{
	float deltaLeftCnt = LeftMotorEncoderValue - LeftMotorEncoderPos_TrackPrev; //encoderPos[0] - encoderPos_TrackPrev[0];
	LeftMotorEncoderPos_TrackPrev = LeftMotorEncoderValue;  //encoderPos_TrackPrev[0] = encoderPos[0];
	float deltaRightCnt = RightMotorEncoderValue - RightMotorEncoderPos_TrackPrev; //encoderPos[1] - encoderPos_TrackPrev[1];
	RightMotorEncoderPos_TrackPrev = RightMotorEncoderValue;  //encoderPos_TrackPrev[1] = encoderPos[1];
	long deltaDistance = 0.5 * (deltaLeftCnt + deltaRightCnt) * CERCUMFERENCE_REVOLUTION_RATIO;//((long)WHEEL_CERCUMFERENCE / (long)TICK_COUNT_PER_REVOLUTION);//* DISTANCE_PER_COUNT;
	
	float deltaX = deltaDistance * cos(Robot_HeadingAngle);
	float deltaY = deltaDistance * sin(Robot_HeadingAngle);
	float deltaHeading = (float)((deltaLeftCnt - deltaRightCnt) * HEADING_ANGLE_RATIO);//((PI * (WHEEL_DIAMETER / ROBOT_TRACK_WIDTH)) / TICK_COUNT_PER_REVOLUTION));//* RADIAN_PER_COUNT);

	Robot_XCoord = Robot_XCoord + deltaX;
	Robot_YCoord = Robot_YCoord + deltaY;
	Robot_HeadingAngle = Robot_HeadingAngle + deltaHeading;

	AdjustHeadingAngle();

	robPwr_msg.x_coord = Robot_XCoord;
	robPwr_msg.y_coord = Robot_YCoord;
	robPwr_msg.heading_angle = Robot_HeadingAngle;
}

void RobotController::Accelerometer(dagu_trex_atmega328p::TrexRobotData &robPwr_msg)
{
	static int X_prev, Y_prev, Z_prev, vibration;    // local variables used for impact detection
  // number of 2mS intervals to wait after an impact has occured before a new impact can be recognized
  	byte devibrate = 50; 
  
	X_prev = Xaxis;   // store previous accelerometer readings for comparison
	Y_prev = Yaxis;
  	Z_prev = Zaxis;
  
  	vibration--;    // loop counter prevents false triggering cause by impact vibration
  	if(vibration < 0)
  	{
  		vibration = 0;             // as robot vibrates due to impact
  	}

  	Xaxis = analogRead(XAXIS_pin); // read accelerometer - note analog read takes 260uS for each axis
  	Yaxis = analogRead(YAXIS_pin);
  	Zaxis = analogRead(ZAXIS_pin);

  	if(vibration > 0) 
  	{
  		return;   // until vibration has subsided no further calculations required
  	}
  
  	dX = Xaxis-X_prev;    // difference between old and new axis readings
  	dY = Yaxis-Y_prev;
  	dZ = Zaxis-Z_prev;
  	// magnitude of delta x,y,z using Pythagorus's Theorum  
  	magnitude = sqrt(sq(dX) + sq(dY) + sq(dZ));

 	robPwr_msg.robot_accel = magnitude;
  
  	if (magnitude > sensitivity) // has a new impact occured
  	{
    	vibration = devibrate;   // reset anti-vibration counter
    	return;
  	}
  	else
  	{
   		magnitude = 0;       // no impact detected
    	dX = 0;
    	dY = 0;
    	dZ = 0;
  	}
}
