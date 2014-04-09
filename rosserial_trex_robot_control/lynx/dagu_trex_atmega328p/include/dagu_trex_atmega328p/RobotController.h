#ifndef RobotController_h
#define RobotController_h

#include <std_msgs/Int32.h>
#include <sensor_msgs/Range.h>

#include "dagu_trex_atmega328p/TrexServoMotorCmds.h"
#include "dagu_trex_atmega328p/TrexRobotData.h"

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
		int Xaxis;    // X, Y, Z accelerometer readings
		int Yaxis;
		int Zaxis;
		int dX,dY,dZ;    // X, Y, Z impact readings 
		int magnitude;  // impact magnitude

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

		static float LeftWheelVelocity;	
		static float RightWheelVelocity;
		static float RobotVelocity;

		static long Robot_XCoord;
		static long Robot_YCoord;
		static float Robot_HeadingAngle;

		static int ElbowPos_Cur;
		static int ClawPos_Cur;
		static int WristPos_Cur;

		static int PanPos_Cur;
		static int TiltPos_Cur;		

		static float LeftMotorEncoderValue;
		static float RightMotorEncoderValue;
		static float LeftMotorEncoderValue_prev;
		static float RightMotorEncoderValue_prev;		
		static float LeftMotorEncoderPos_TrackPrev;
		static float RightMotorEncoderPos_TrackPrev;

	public:
		static unsigned long VoltageTimer;
		static unsigned long AccelTimer;
		static unsigned long IRangeTimer;
		static unsigned long VelocityTimer;

		RobotController();
		void InitMotorsPins();

//		void InitIRangeData(sensor_msgs::Range &range_msg);
		void IRScan(dagu_trex_atmega328p::TrexRobotData &robData_msg);
	
		void InitServos();
		void InitServosCurPos();

		void InitEncoders();
		void UpdateEncoderValues(dagu_trex_atmega328p::TrexRobotData &robData_msg);

		int CheckMsgValidity(const dagu_trex_atmega328p::TrexServoMotorCmds &nmc);
		int SerialCommunicate(const dagu_trex_atmega328p::TrexServoMotorCmds &nmc);

		void SetPWM(int WhichMotor, const dagu_trex_atmega328p::TrexServoMotorCmds &nmc);
		void ProcessMotorCommand(int WhichMotor);		

		void ProcessServoCommand(int PinNum);
		int ValidateServoCurPos(int PinNum, int POS);
		
		int ShutdownMotors();

		void MonitorVoltageLevel(dagu_trex_atmega328p::TrexRobotData &robData_msg);
		void CalculateRobotVelocity(dagu_trex_atmega328p::TrexRobotData &robData_msg);
		void TrackRobot(dagu_trex_atmega328p::TrexRobotData &robData_msg);
		void AdjustHeadingAngle();

		void Accelerometer(dagu_trex_atmega328p::TrexRobotData &robData_msg);
};

#endif
