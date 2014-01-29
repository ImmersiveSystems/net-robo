#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Servo.h>
#include <Sabertooth.h>
#include "UnoRobotController.h"
#include "UnoConstants.h"

Sabertooth SaberTooth(128);

UnoRobotController::UnoRobotController(): ElbowPin(ELBOW), ClawPin(CLAW), WristPin(WRIST), ElbowInitPos(ELBOW_INIT), ClawInitPos(CLAW_INIT), WristInitPos(WRIST_INIT), PanPin(PAN), TiltPin(TILT), PanInitPos(PAN_INIT), TiltInitPos(TILT_INIT)
{
  int data = 0;
  int Leftmode = 0;
  int Rightmode = 0;
  int LeftPWM = 0;                                       
  int RightPWM = 0;
}

void UnoRobotController::InitServos()
{
  servoElbow.attach(ElbowPin);
  servoClaw.attach(ClawPin);  
  servoWrist.attach(WristPin);

  servoElbow.write(ElbowInitPos);
  servoClaw.write(ClawInitPos);  
  servoWrist.write(WristInitPos);

  servoPan.attach(PanPin);
  servoTilt.attach(TiltPin);

  servoPan.write(PanInitPos);
  servoTilt.write(TiltInitPos);	
}

void UnoRobotController::InitSaber()
{
  SabertoothTXPinSerial.begin(9600); // 9600 is the default baud rate for Sabertooth packet serial.
  SaberTooth.autobaud(); // Send the autobaud command to the Sabertooth controller(s).
                 // NOTE: *Not all* Sabertooth controllers need this command.
                 //       It doesn't hurt anything, but V2 controllers use an
                 //       EEPROM setting (changeable with the function setBaudRate) to set
                 //       the baud rate instead of detecting with autobaud.
                 //       If you have a 2x12, 2x25 V2, 2x60 or SyRen 50, you can remove
                 //       the autobaud line and save yourself two seconds of startup delay.  
  SaberTooth.drive(StopSignal);
  SaberTooth.turn(StopSignal);	
}

void UnoRobotController::Serialread() 
{
	do
	{
	  data = Serial.read();  
	}
	while (data < 0);
}

void UnoRobotController::SerialCommunicate()
{
	if (Serial.available() > 0)                                   // command available
	{
	  if(Serial.read() == 'H')
	  {
	    Set_PWM(RightMotor);
	    Set_PWM(LeftMotor);
	  }
	  else
	  {
	    Serial.flush();
	  }
	}
    ReadEncoder(RightMotor - 1);
    ReadEncoder(LeftMotor - 1);
}

void UnoRobotController::Set_PWM(int MotorNum)
{
	switch(MotorNum)
	{
	  case 1:
	    Serialread();
	    Rightmode = data;
	    Serialread();
	    RightPWM = data * SpeedScale;  
	    ProcessMotorCommand(Rightmode, RightMotor, RightPWM);
	    break;    
	 
	  case 2:
	    Serialread();
	    Leftmode = data;
	    Serialread();
	    LeftPWM = data * SpeedScale;
	    ProcessMotorCommand(Leftmode, LeftMotor, LeftPWM);
	    break;
	 
	  default:
	    break;
	}
}

void UnoRobotController::ProcessMotorCommand(int Mode, int MotorNum, int PWMVal)
{
	switch(Mode)
	{
	  case 2: //Move Forward
	    SaberTooth.motor(MotorNum, PWMVal);
	    break;
	 
	  case 1: //Stop
	    SaberTooth.motor(MotorNum, 0);
	    break;
	 
	  case 0: //Move Backward
	    SaberTooth.motor(MotorNum, -1 * PWMVal);  
	    break;
	}
}

void UnoRobotController::ProcessEncoders()
{
    ReadEncoder(RightMotor - 1);
    ReadEncoder(LeftMotor - 1);
}

void UnoRobotController::MoveServoMotor(int PinNum, int POS)
{
	POS = CheckAngleValue(PinNum, POS);

	if(PinNum == PAN)
	{
		servoPan.write(POS);
	}
	else if(PinNum == TILT)
	{
		servoTilt.write(POS);
	}
	else if(PinNum == ELBOW)
	{
		servoElbow.write(POS);
	}
	else if(PinNum == CLAW)
	{
		servoClaw.write(POS);
	}
	else if(PinNum == WRIST)
	{
		servoWrist.write(POS);
	}
}

int UnoRobotController::CheckAngleValue(int PinNum, int POS)
{
	if(PinNum == TILT && POS > MaxTiltAngle)
	{
		return MaxTiltAngle;
	}
	else if(PinNum == ELBOW && POS > MaxElbowAngle)
	{
		return MaxElbowAngle;
	}
	else if(PinNum != TILT && POS > MaxOtherServoAngle)
	{
		return MaxOtherServoAngle;
	}
	else if(POS < MinServoAngle)
	{
		return MinServoAngle;
	}
	else
	{
		return POS;
	}
}