// TREXRobotControl.ino

#include <ros.h>
#include "trex_robot_control/TrexMotorCmds.h"
#include "trex_robot_control/RobotController.h"
#include "trex_robot_control/Constants.h"


RobotController robot;
ros::NodeHandle  rosNodeHandle;

void pi_trex_messageCb(const trex_robot_control::TrexMotorCmds &nmc)
{
	if(robot.CheckMsgValidity(nmc) == 1)  
	{                                                     
	  //rosNodeHandle.loginfo("Received Command Is Sound");
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
  rosNodeHandle.subscribe(arduino_pi_subscriber);	
}

void loop() 
{
	robot.ProcessMotorCommand(LeftMotor);
	robot.ProcessMotorCommand(RightMotor);

    rosNodeHandle.spinOnce();
    //delay(500);
}

