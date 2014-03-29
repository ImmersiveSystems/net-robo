#ifndef RobotController_h
#define RobotController_h

#include "trex_robot_control/TrexMotorCmds.h"

class RobotController 
{
	private:
		int LeftMode;
		int RightMode;		
		int LeftMotorSpeed;
		int RightMotorSpeed;
	public:
		RobotController();
		void InitPins();
		int CheckMsgValidity(const trex_robot_control::TrexMotorCmds &nmc);
		int SerialCommunicate(const trex_robot_control::TrexMotorCmds &nmc);
		void SetPWM(int WhichMotor, const trex_robot_control::TrexMotorCmds &nmc);
		void ProcessMotorCommand(int WhichMotor);
		int ShutdownMotors();
};

#endif