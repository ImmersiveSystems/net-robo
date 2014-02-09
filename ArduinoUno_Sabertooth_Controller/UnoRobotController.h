#ifndef UnoRobotController_h
#define UnoRobotController_h

#include <Servo.h>

#include "UnoRobotEncoder.h"

class UnoRobotController:public UnoRobotEncoder
{	
protected:
	int data;
	int Leftmode;
	int Rightmode;
	int LeftPWM;
	int RightPWM;

	const int ElbowPin;
	const int ClawPin;
	const int WristPin;
	const int ElbowInitPos;
	const int ClawInitPos;
	const int WristInitPos;

	const int PanPin;
	const int TiltPin;
	const int PanInitPos;
	const int TiltInitPos;

	static int ElbowPos_Cur;
	static int ClawPos_Cur;
	static int WristPos_Cur;

	static int PanPos_Cur;
	static int TiltPos_Cur;

	Servo servoElbow;  // create servo object to control a servo (max. of eight objects)
	Servo servoClaw;
	Servo servoWrist;

	Servo servoPan;
	Servo servoTilt;

public:
	UnoRobotController();
	void InitServosCurPos();
	void InitSaber();
	void InitServos();

	void Serialread();
	void SerialCommunicate();

	void Set_PWM(int MotorNum);	
	void ProcessMotorCommand(int Mode, int MotorNum, int PWMVal);

	int ValidateServoCurPos(int PinNum, int POS);
	void ProcessServoCommand(int PinNum);

	void TestServos(int val);
};

#endif