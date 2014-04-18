
// TREXRobotControl.ino

#include <ros.h>
#include <ros/time.h>

#include "trex_robot_control/RobotController.h"
#include "trex_robot_control/TrexMotorCmds.h"
#include "trex_robot_control/TrexRobotData.h"
#include "trex_robot_control/Constants.h"

RobotController robot;
ros::NodeHandle  rosNodeHandle;

trex_robot_control::TrexRobotData robData_msg; // stores current power of the robot

ros::Publisher TrexRobDatatalker("trex_pi_rob_data_talkback", &robData_msg);

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

	rosNodeHandle.initNode();  
	rosNodeHandle.advertise(TrexRobDatatalker);

	rosNodeHandle.subscribe(arduino_pi_subscriber);	
}

void loop() 
{
	robot.ProcessMotorCommand(LeftMotor);
	robot.ProcessMotorCommand(RightMotor);	

	// robot.IRScan(robData_msg);	//IR ranger function
    //robot.UltrasonicScan(ULTRASONIC_cm, robData_msg); // Ultrasonic, conventional - lower update rate
    robot.SonarScan(robData_msg); // Ultrasonic, new library - faster - adjustable update rate
        
    robot.MonitorVoltageLevel(robData_msg);
    // robot.Accelerometer(robData_msg);


    TrexRobDatatalker.publish(&robData_msg);        
    rosNodeHandle.spinOnce();
}

