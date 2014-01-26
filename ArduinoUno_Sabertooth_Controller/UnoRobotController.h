#ifndef UnoRobotController_h
#define UnoRobotController_h

#include "UnoRobotEncoder.h"

class UnoRobotController:public UnoRobotEncoder
{	
private:
	int data;
	int Leftmode;
	int Rightmode;
	int LeftPWM;
	int RightPWM;
public:
	UnoRobotController();
	void InitSaber();
	void SerialCommunicate();
	void Set_PWM(int MotorNum);
	void Serialread();
	void ProcessMotorCommand(int Mode, int MotorNum, int PWMVal);
	void ProcessEncoders();
};

#endif