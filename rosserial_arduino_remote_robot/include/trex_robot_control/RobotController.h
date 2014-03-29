#ifndef RobotController_h
#define RobotController_h

#include "trex_robot_control/TrexMotorCmds.h"
#include "trex_robot_control/TrexPowerMsg.h"

class RobotController 
{
	private:
		int LeftMode;
		int RightMode;		
		int LeftMotorSpeed;
		int RightMotorSpeed;
		float voltage;
		int LeftMotorCur;
		int RightMotorCur;
	public:
		RobotController();
		void InitPins();
		int CheckMsgValidity(const trex_robot_control::TrexMotorCmds &nmc);
		int SerialCommunicate(const trex_robot_control::TrexMotorCmds &nmc);
		void SetPWM(int WhichMotor, const trex_robot_control::TrexMotorCmds &nmc);
		void ProcessMotorCommand(int WhichMotor);
		void MonitorVoltageLevel(trex_robot_control::TrexPowerMsg &tpm);
		int ShutdownMotors();
};

#endif