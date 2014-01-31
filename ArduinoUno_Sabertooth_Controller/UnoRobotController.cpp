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

int UnoRobotController::ElbowPos_Cur;
int UnoRobotController::ClawPos_Cur;
int UnoRobotController::WristPos_Cur;

int UnoRobotController::PanPos_Cur;
int UnoRobotController::TiltPos_Cur;

int UnoRobotController::ElbowFlag;
int UnoRobotController::ClawFlag;
int UnoRobotController::WristFlag;

int UnoRobotController::PanFlag;
int UnoRobotController::TiltFlag;

UnoRobotController::UnoRobotController(): ElbowPin(ELBOW), ClawPin(CLAW), WristPin(WRIST), ElbowInitPos(ELBOW_INIT), ClawInitPos(CLAW_INIT), WristInitPos(WRIST_INIT), PanPin(PAN), TiltPin(TILT), PanInitPos(PAN_INIT), TiltInitPos(TILT_INIT)
{
  int data = 0;
  int Leftmode = 0;
  int Rightmode = 0;
  int LeftPWM = 0;                                       
  int RightPWM = 0;
}

void UnoRobotController::InitServosCurPos()
{
	ElbowPos_Cur = ElbowInitPos;
	ClawPos_Cur = ClawInitPos;
	WristPos_Cur = WristInitPos;

	PanPos_Cur = PanInitPos;
	TiltPos_Cur = TiltInitPos;
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

  InitServosCurPos();
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
	    Set_ServoPos();
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
	    SaberTooth.stop();
	    // SaberTooth.motor(MotorNum, 0);
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

void UnoRobotController::Set_ServoPos()
{
	/* 
	  data values:
	  0 : do othing
	  1 : increment current position by one
	  -1 : decrement current position by one
	*/
	Serialread();
	PanFlag = data;
	PanPos_Cur = PanPos_Cur; //+ PanFlag;

	Serialread();
	TiltFlag = data;
	TiltPos_Cur = TiltPos_Cur; //+ TiltFlag;
	
	Serialread();
	ElbowFlag = data;
	ElbowPos_Cur = ElbowPos_Cur; //+ ElbowFlag;
	
	Serialread();
	ClawFlag = data;
	ClawPos_Cur = ClawPos_Cur; //+ ClawFlag;
	
	Serialread();
	WristFlag = data;
	WristPos_Cur = WristPos_Cur; //+ WristFlag;	
}

void UnoRobotController::UpdateServosPos()
{
	ProcessServoCommand(PanPin);	
	ProcessServoCommand(TiltPin);
	ProcessServoCommand(ElbowPin);
	ProcessServoCommand(ClawPin);
	ProcessServoCommand(WristPin);
/*	
	if(PanFlag != 0)
	{
		ProcessServoCommand(PanPin);
	}
	if(TiltFlag != 0)
	{
		ProcessServoCommand(TiltPin);
	}
	if(ElbowFlag != 0)
	{
		ProcessServoCommand(ElbowPin);
	}
	if(ClawFlag != 0)
	{
		ProcessServoCommand(ClawPin);
	}
	if(WristFlag != 0)
	{
		ProcessServoCommand(WristPin);
	}
	*/
}

void UnoRobotController::ProcessServoCommand(int PinNum)
{
	if(PinNum == PanPin)
	{
		PanPos_Cur = ValidateServoCurPos(PinNum, PanPos_Cur);
		servoPan.write(PanPos_Cur);
	}
	else if(PinNum == TiltPin)
	{
		TiltPos_Cur = ValidateServoCurPos(PinNum, TiltPos_Cur);
		servoTilt.write(TiltPos_Cur);
	}
	else if(PinNum == ElbowPin)
	{
		ElbowPos_Cur = ValidateServoCurPos(PinNum, ElbowPos_Cur);
		servoElbow.write(ElbowPos_Cur);
	}
	else if(PinNum == ClawPin)
	{
		ClawPos_Cur = ValidateServoCurPos(PinNum, ClawPos_Cur);
		servoClaw.write(ClawPos_Cur);
	}
	else if(PinNum == WristPin)
	{
		WristPos_Cur = ValidateServoCurPos(PinNum, WristPos_Cur);
		servoWrist.write(WristPos_Cur);
	}	
}

int UnoRobotController::ValidateServoCurPos(int PinNum, int POS)
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

void UnoRobotController::TestServos(int val)
{
	/* 
	  data values:
	  0 : do nothing
	  1 : increment current position by one
	  -1 : decrement current position by one
	*/
	PanFlag = val;
	PanPos_Cur = PanPos_Cur + PanFlag;

	TiltFlag = val;
	TiltPos_Cur = TiltPos_Cur + TiltFlag;
	
	ElbowFlag = val;
	ElbowPos_Cur = ElbowPos_Cur + ElbowFlag;
	
	ClawFlag = val;
	ClawPos_Cur = ClawPos_Cur + ClawFlag;
	
	WristFlag = val;
	WristPos_Cur = WristPos_Cur + WristFlag;	
}
