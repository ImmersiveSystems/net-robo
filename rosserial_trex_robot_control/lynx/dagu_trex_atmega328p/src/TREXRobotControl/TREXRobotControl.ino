// TREXRobotControl.ino

#include <ros.h>
#include <ros/time.h>

//#include <sensor_msgs/Range.h>

#include "dagu_trex_atmega328p/RobotController.h"
#include "dagu_trex_atmega328p/Constants.h"

#include "dagu_trex_atmega328p/TrexServoMotorCmds.h"
#include "dagu_trex_atmega328p/TrexRobotData.h"

RobotController robot;
ros::NodeHandle  rosNodeHandle;

dagu_trex_atmega328p::TrexRobotData robData_msg;

ros::Publisher TrexRobDatatalker("trex_pi_rob_data_talkback", &robData_msg);

void pi_trex_messageCb(const dagu_trex_atmega328p::TrexServoMotorCmds &nmc)
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

ros::Subscriber<dagu_trex_atmega328p::TrexServoMotorCmds> arduino_pi_subscriber("pi_trex_chatter", &pi_trex_messageCb);

void setup() 
{
   robot.InitMotorsPins();
   robot.InitServos();
//   robot.InitIRangeData(range_msg);   /*Uncomment this if you want to use #include <sensor_msgs/Range.h> YOU MUST UNCOMMENT RESPECTIVE FUNCTIONS IN RobotController.h and .ino as well and introduce a publisher that publishes on sensor_msgs::Range type variable*/
   robot.InitEncoders();

   rosNodeHandle.initNode();  
   
   rosNodeHandle.advertise(TrexRobDatatalker);

   rosNodeHandle.subscribe(arduino_pi_subscriber);	
}

void loop() 
{
   robot.ProcessMotorCommand(LeftMotor);
   robot.ProcessMotorCommand(RightMotor);

   robot.IRScan(robData_msg);
   
   robot.UpdateEncoderValues(robData_msg);
   robot.TrackRobot(robData_msg);

   robot.CalculateRobotVelocity(robData_msg);
   robot.MonitorVoltageLevel(robData_msg);
   robot.Accelerometer(robData_msg);
          
  /* Uncomment the "IF" blcok in case only the robot voltage is to be communicated by the client */
//   if (millis() - robot.VoltageTimer > BATTERYINFOPUBLISH)
//   {   	
//    robot.VoltageTimer = millis();		
    TrexRobDatatalker.publish(&robData_msg);
//   }

   rosNodeHandle.spinOnce();
}

