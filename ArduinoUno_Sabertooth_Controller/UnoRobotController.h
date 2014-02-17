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
	static float LeftWheelVelocity;	
	static float RightWheelVelocity;
	static float RobotVelocity;

	static unsigned long Timer_Cur;
	static unsigned long Timer_Prev;

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
	void CheckRobotTrajectory();
	void MaintainStarightMotion(int DirFlag); // during the forward or backward motion only
	void CalculateRobotVelocity();

	void UpdateEncoderValuesMsg();

	int ValidateServoCurPos(int PinNum, int POS);
	void ProcessServoCommand(int PinNum);

	void SendVelocity_Encoders_Msg();

	void TestServos(int val);
};

#endif