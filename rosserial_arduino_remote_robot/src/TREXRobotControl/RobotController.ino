// RobotController.ino

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "trex_robot_control/RobotController.h"
#include "trex_robot_control/IOpins.h"
#include "trex_robot_control/Constants.h"

byte LeftMotorBrake;
byte RightMotorBrake;

RobotController::RobotController()
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
}

void RobotController::InitPins()
{
  pinMode(LEFTMOTORPWM_pin,OUTPUT);          // configure left  motor PWM       pin for output
  pinMode(LEFTMOTORDIRECTION_pin,OUTPUT);   // configure left  motor direction pin for output
  pinMode(LEFTMOTORBRAKE_pin,OUTPUT);      // configure left  motor brake     pin for output
  
  pinMode(RIGHTMOTORPWM_pin,OUTPUT);        // configure right motor PWM       pin for output
  pinMode(RIGHTMOTORDIRECTION_pin,OUTPUT); // configure right motor direction pin for output
  pinMode(RIGHTMOTORBRAKE_pin,OUTPUT);     // configure right motor brake     pin for output	
}

int RobotController::ShutdownMotors()
{	
	LeftMotorSpeed = 0;      // set left  motor speed to 0 (off)
	RightMotorSpeed = 0;      // set right motor speed to 0 (off)
	ProcessMotorCommand(LeftMotor);       // update H bridges
	ProcessMotorCommand(RightMotor);       // update H bridges
}

int RobotController::CheckMsgValidity(const trex_robot_control::TrexMotorCmds &nmc)
{
	if((int)nmc.cmd_type == 72 || (int)nmc.cmd_type == 90) // i.e., decimal equivalents for "H" (mode and speed values for the motors) or for "Z" (hibernate/power_save mode for the robot)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
int RobotController::SerialCommunicate(const trex_robot_control::TrexMotorCmds &nmc)
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

      case 90: //i.e., "Z"
      	ShutdownMotors();
        StatusFlag = 1;      
        break; 

      default:
        break;
     }
     
     return StatusFlag; 
}

void RobotController::SetPWM(int WhichMotor, const trex_robot_control::TrexMotorCmds &nmc)
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

void RobotController::MonitorVoltageLevel(trex_robot_control::TrexPowerMsg &tpm)
{

	// read  left motor current sensor and convert reading to mA
	LeftMotorCur = (analogRead(LEFTMOTORCURRENT_pin) - 511) * 48.83;          
	// read right motor current sensor and convert reading to mA
	RightMotorCur = (analogRead(RIGHTMOTORCURRENT_pin) - 511) * 48.83;          
	// read battery level and convert to volts with 2 decimal places (eg. 1007 = 10.07 V)	
	voltage = analogRead(VOLTAGE_pin) * 10 / 3.357;             
	
	tpm.voltage_level = voltage;
	tpm.timing_msg = millis();

	if(voltage < LOWBAT) 
	{
		tpm.power_msg = "Out of Charge";
		ShutdownMotors();
	}		
	else
	{
		tpm.power_msg = "Charged";
	}
}
