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

	Servo servoElbow;  // create servo object to control a servo (max. of eight objects)
	Servo servoClaw;
	Servo servoWrist;

	Servo servoPan;
	Servo servoTilt;

public:
	UnoRobotController(); //(int Pan_Pin, int Tilt_Pin, int Pan_Init_Pos, int Tilt_Init_Pos);
	void InitSaber();
	void InitServos();
	void SerialCommunicate();
	void Set_PWM(int MotorNum);
	void Serialread();
	void ProcessMotorCommand(int Mode, int MotorNum, int PWMVal);
	void ProcessEncoders();
	void MoveServoMotor(int PinNum, int POS);
	int CheckAngleValue(int PinNum, int POS);
};

#endif