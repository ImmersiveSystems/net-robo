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

float UnoRobotController::LeftWheelVelocity;	
float UnoRobotController::RightWheelVelocity;
float UnoRobotController::RobotVelocity;

unsigned long UnoRobotController::Timer_Cur;
unsigned long UnoRobotController::Timer_Prev;

UnoRobotController::UnoRobotController(): ElbowPin(ELBOW), ClawPin(CLAW), WristPin(WRIST), ElbowInitPos(ELBOW_INIT), ClawInitPos(CLAW_INIT), WristInitPos(WRIST_INIT), PanPin(PAN), TiltPin(TILT), PanInitPos(PAN_INIT), TiltInitPos(TILT_INIT)
{
  data = 0;
  Leftmode = 0;
  Rightmode = 0;
  LeftPWM = 0;                                       
  RightPWM = 0;

  LeftWheelVelocity = 0.0;	
  RightWheelVelocity = 0.0;
  RobotVelocity = 0.0;

  LeftWheelVelocity = 0.0;	
  RightWheelVelocity = 0.0;
  RobotVelocity = 0.0;


  Timer_Cur = 0;  
  Timer_Prev = 0;
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
    int command = Serial.read();

    switch(command)
    {
      case 'H':
        Set_PWM(RightMotor);
        Set_PWM(LeftMotor);
        break;
      case 'P':
        Serialread();
        PanPos_Cur = data;          
        ProcessServoCommand(PanPin);
        break;
      case 'T':
        Serialread();
        TiltPos_Cur = data;          
        ProcessServoCommand(TiltPin);
        break;
      case 'E':
        Serialread();
        if(data == 1)
        {
          ElbowPos_Cur = ElbowPos_Cur - ServoAdjustRate;
        }
        else if(data == 0)
        {
          ElbowPos_Cur = ElbowPos_Cur + ServoAdjustRate;
        }
        ProcessServoCommand(ElbowPin);
        break;
      case 'C':
        if(ClawPos_Cur == 0)
        {
          ClawPos_Cur = MaxOtherServoAngle;
        }
        else if(ClawPos_Cur == 180)
        {
          ClawPos_Cur = ClawMin;
        }
        servoClaw.write(ClawPos_Cur);
        break;
      case 'W':
        Serialread();
        if(data == 1)
        {
          WristPos_Cur = WristPos_Cur - ServoAdjustRate;
        }
        else if(data == 0)
        {
          WristPos_Cur = WristPos_Cur + ServoAdjustRate;
        }
        ProcessServoCommand(WristPin);
        break;
      default:
        Serial.flush();
    }
  }	
}

void UnoRobotController::Set_PWM(int MotorNum)
{
	switch(MotorNum)
	{
	  case RightMotor://1:
	    Serialread();
	    Rightmode = data;
	    Serialread();
	    RightPWM = data * PWMRate;  
	    ProcessMotorCommand(Rightmode, RightMotor, RightPWM);
	    break;    
	 
	  case LeftMotor://2:
	    Serialread();
	    Leftmode = data;
	    Serialread();
	    LeftPWM = data * PWMRate;
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
	  case FORWARD://2: //Move Forward
	    SaberTooth.motor(MotorNum, PWMVal);
	    break;
	 
	  case STOP://1: //Stop
	    SaberTooth.stop();
	    // SaberTooth.motor(MotorNum, 0);
	    break;
	 
	  case BACKWARD: //0: //Move Backward
	    SaberTooth.motor(MotorNum, -1 * PWMVal);  
	    break;
	}
}

void UnoRobotController::CalculateRobotVelocity()
{
	Timer_Cur = millis();
	if(Timer_Cur - Timer_Prev >= VELOCITY_CALC_INTERVAL)
	{
	  Timer_Prev = Timer_Cur;
	  long RightWheel_DeltaTick = encoderPos[1] - encoderPos_Prev[1];
	  encoderPos_Prev[1] = encoderPos[1];
	  float RightWheelDistance = ((RightWheel_DeltaTick * WHEEL_CERCUMFERENCE) / TICK_COUNT_PER_REVOLUTION);
	  RightWheelVelocity = RightWheelDistance / VELOCITY_CALC_INTERVAL;

	  long LeftWheel_DeltaTick = encoderPos[0] - encoderPos_Prev[0];
	  encoderPos_Prev[0] = encoderPos[0];
	  float LeftWheelDistance = ((LeftWheel_DeltaTick * WHEEL_CERCUMFERENCE) / TICK_COUNT_PER_REVOLUTION);
	  LeftWheelVelocity = LeftWheelDistance / VELOCITY_CALC_INTERVAL;

	  RobotVelocity = sqrt(pow(LeftWheelVelocity, 2) + pow(RightWheelVelocity, 2));	
	  Serial.println(RobotVelocity); //meter per sec
	}
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
	PanPos_Cur = PanPos_Cur + val;
	ProcessServoCommand(PanPin);

	TiltPos_Cur = TiltPos_Cur + val;
	ProcessServoCommand(TiltPin);
	
	ElbowPos_Cur = ElbowPos_Cur + val;
	ProcessServoCommand(ElbowPin);
	
	ClawPos_Cur = ClawPos_Cur + val;
	ProcessServoCommand(ClawPin);
	
	WristPos_Cur = WristPos_Cur + val;	
	ProcessServoCommand(WristPin);
}
