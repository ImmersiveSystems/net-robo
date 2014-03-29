// TREXRobotControl.ino

#include <ros.h>
#include "trex_robot_control/RobotController.h"
#include "trex_robot_control/TrexMotorCmds.h"
#include "trex_robot_control/TrexPowerMsg.h"
#include "trex_robot_control/Constants.h"

int static BatteryInfoCnt = 0;

RobotController robot;
ros::NodeHandle  rosNodeHandle;

trex_robot_control::TrexPowerMsg robPwr_msg; // stores current power of the robot

ros::Publisher TrexPowertalker("trex_pi_power_talkback", &robPwr_msg);

void pi_trex_messageCb(const trex_robot_control::TrexMotorCmds &nmc)
{
	if(robot.CheckMsgValidity(nmc) == 1)  
	{                                                     
	  rosNodeHandle.loginfo("Received Command Is Sound");
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

  rosNodeHandle.initNode();  
  rosNodeHandle.advertise(TrexPowertalker);
  rosNodeHandle.subscribe(arduino_pi_subscriber);	
}

void loop() 
{
	robot.ProcessMotorCommand(LeftMotor);
	robot.ProcessMotorCommand(RightMotor);
	robot.MonitorVoltageLevel(robPwr_msg);

	if (BatteryInfoCnt > BATTERYINFOPUBLISH)
	{	
	   BatteryInfoCnt = 0;
	   TrexPowertalker.publish(&robPwr_msg);
	}
	else
	{
	   BatteryInfoCnt ++;
	}
	
    rosNodeHandle.spinOnce();
}

