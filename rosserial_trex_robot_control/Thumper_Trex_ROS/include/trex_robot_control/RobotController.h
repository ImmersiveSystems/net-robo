#ifndef RobotController_h
#define RobotController_h

#include <std_msgs/Int32.h>
#include <sensor_msgs/Range.h>

#include "trex_robot_control/TrexMotorCmds.h"
#include "trex_robot_control/TrexRobotData.h"

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

        static long sonarDistance;
        static unsigned long pingTimer;     // Holds the next ping time.

	public:
		static unsigned long VoltageTimer;
		static unsigned long AccelTimer;
		static unsigned long IRangeTimer;

		RobotController();
		void InitPins();

//		void InitIRangeData(sensor_msgs::Range &range_msg);
		void IRScan(trex_robot_control::TrexRobotData &robData_msg);//(sensor_msgs::Range &range_msg);

		void UltrasonicScan(int WhichMetric, trex_robot_control::TrexRobotData &robData_msg);
		long MicroSecToInch(long microsec);
		long MicroSecToCentimeter(long microsec);

		friend void echoCheck();
		void UpdateSonarDistance();
		void SonarScan(trex_robot_control::TrexRobotData &robData_msg);

		int CheckMsgValidity(const trex_robot_control::TrexMotorCmds &nmc);
		int SerialCommunicate(const trex_robot_control::TrexMotorCmds &nmc);
		void SetPWM(int WhichMotor, const trex_robot_control::TrexMotorCmds &nmc);
		void ProcessMotorCommand(int WhichMotor);
		int ShutdownMotors();

		void MonitorVoltageLevel(trex_robot_control::TrexRobotData &robData_msg);

		void Accelerometer(trex_robot_control::TrexRobotData &robData_msg);//(std_msgs::Int32 &accel_msg);
};

#endif
