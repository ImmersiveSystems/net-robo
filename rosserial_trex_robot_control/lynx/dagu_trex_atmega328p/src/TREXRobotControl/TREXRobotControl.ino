
// TREXRobotControl.ino

#include <ros.h>
#include <ros/time.h>
// #include <sensor_msgs/Range.h>

#include "dagu_trex_atmega328p/RobotController.h"
#include "dagu_trex_atmega328p/Constants.h"

#include "dagu_trex_atmega328p/TrexServoMotorCmds.h"
#include "dagu_trex_atmega328p/TrexRobotData.h"

RobotController robot;
ros::NodeHandle  rosNodeHandle;

dagu_trex_atmega328p::TrexRobotData robPwr_msg;
//sensor_msgs::Range range_msg;

ros::Publisher TrexPowertalker("trex_pi_power_talkback", &robPwr_msg);
/*
ros::Publisher TrexRobLoctalker("trex_pi_rob_loc_talkback", &robPwr_msg);
ros::Publisher TrexVelocitytalker("trex_pi_vel_talkback", &robPwr_msg);
ros::Publisher TrexIRangetalker("trex_pi_irange_talkback", &robPwr_msg);
ros::Publisher TrexEncoderstalker("trex_pi_encoders_talkback", &robPwr_msg);
*/
//ros::Publisher TrexAcceltalker("trex_pi_accel_talkback", &robPwr_msg);


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
   //robot.InitIRangeData(range_msg);
   robot.InitEncoders();

   rosNodeHandle.initNode();  
   
   rosNodeHandle.advertise(TrexPowertalker);
/*   
   rosNodeHandle.advertise(TrexRobLoctalker);
   rosNodeHandle.advertise(TrexVelocitytalker);
   rosNodeHandle.advertise(TrexIRangetalker);
   rosNodeHandle.advertise(TrexEncoderstalker);
   // rosNodeHandle.advertise(TrexAcceltalker);
*/
   rosNodeHandle.subscribe(arduino_pi_subscriber);	
}

void loop() 
{
   robot.ProcessMotorCommand(LeftMotor);
   robot.ProcessMotorCommand(RightMotor);

   robot.UpdateEncoderValues(robPwr_msg);
   robot.TrackRobot(robPwr_msg);

   robot.CalculateRobotVelocity(robPwr_msg);
   robot.MonitorVoltageLevel(robPwr_msg);
/*       
   TrexEncoderstalker.publish(&robPwr_msg);
   TrexRobLoctalker.publish(&robPwr_msg);
  */     
   if (millis() - robot.VoltageTimer > BATTERYINFOPUBLISH)
   {   	
    robot.VoltageTimer = millis();		
    TrexPowertalker.publish(&robPwr_msg);
   }
/*
   if (millis() - robot.VelocityTimer > VELOCITY_CALC_INTERVAL)
   {   	
    robot.VelocityTimer = millis();		
    TrexVelocitytalker.publish(&robPwr_msg);
   }   
  */     
/*
   if(micros() - robot.AccelTimer > ACCELVALUEPUBLISH) //i.e., evey 1ms
   {
   	robot.AccelTimer = micros();
    robot.Accelerometer(accel_msg);
    TrexAcceltalker.publish(&accel_msg);
   }
*/
/*
   if (millis() - robot.IRangeTimer > IRANGEDATAPUBLISH)
   {
    robot.IRScan(robPwr_msg);
    // range_msg.header.stamp = rosNodeHandle.now();
    TrexIRangetalker.publish(&robPwr_msg);
    // robot.IRangeTimer =  millis();
   }	
  */        

   rosNodeHandle.spinOnce();
}

