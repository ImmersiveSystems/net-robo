
// TREXRobotControl.ino

#include <ros.h>
//#include <std_msgs/Int32.h>
#include <ros/time.h>
#include <sensor_msgs/Range.h>

#include "trex_robot_control/RobotController.h"
#include "trex_robot_control/TrexMotorCmds.h"
#include "trex_robot_control/TrexPowerMsg.h"
#include "trex_robot_control/Constants.h"

RobotController robot;
ros::NodeHandle  rosNodeHandle;

trex_robot_control::TrexPowerMsg robPwr_msg; // stores current power of the robot
//std_msgs::Int32 accel_msg; //stores calculated magnitude by the Accelerometer function
sensor_msgs::Range range_msg;

ros::Publisher TrexPowertalker("trex_pi_power_talkback", &robPwr_msg);
//ros::Publisher TrexAcceltalker("trex_pi_accel_talkback", &accel_msg);
ros::Publisher TrexIRangetalker("trex_pi_irange_talkback", &range_msg);

void pi_trex_messageCb(const trex_robot_control::TrexMotorCmds &nmc)
{
	if(robot.CheckMsgValidity(nmc) == 1)  
	{                                                     
//	  rosNodeHandle.loginfo("Received Command Is Sound");
	  if (robot.SerialCommunicate(nmc) != 1)
	  {
	    rosNodeHandle.loginfo("Unable to Deliver the Command");
	  }
	}
}

ros::Subscriber<trex_robot_control::TrexMotorCmds> arduino_pi_subscriber("pi_trex_chatter", &pi_trex_messageCb);

void setup() 
{
	robot.InitPins();
	robot.InitIRangeData(range_msg);

	rosNodeHandle.initNode();  
	rosNodeHandle.advertise(TrexPowertalker);
//	rosNodeHandle.advertise(TrexAcceltalker);
	rosNodeHandle.advertise(TrexIRangetalker);

	rosNodeHandle.subscribe(arduino_pi_subscriber);	
}

void loop() 
{
	robot.ProcessMotorCommand(LeftMotor);
	robot.ProcessMotorCommand(RightMotor);	
	robot.MonitorVoltageLevel(robPwr_msg);

	if (millis() - robot.VoltageTimer > BATTERYINFOPUBLISH)
	{	
	    robot.VoltageTimer = millis();		
  	    TrexPowertalker.publish(&robPwr_msg);
	}
/*
	if(micros() - robot.AccelTimer > ACCELVALUEPUBLISH) //i.e., evey 1ms
	{
		robot.AccelTimer = micros();
		robot.Accelerometer(accel_msg);
		TrexAcceltalker.publish(&accel_msg);
	}
*/
	if (millis() - robot.IRangeTimer > IRANGEDATAPUBLISH)
	{
	    robot.IRScan(range_msg);
	    range_msg.header.stamp = rosNodeHandle.now();
	    TrexIRangetalker.publish(&range_msg);
	    robot.IRangeTimer =  millis();
  	}	

        rosNodeHandle.spinOnce();
}

