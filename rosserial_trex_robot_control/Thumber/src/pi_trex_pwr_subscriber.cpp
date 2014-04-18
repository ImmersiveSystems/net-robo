#include "ros/ros.h"
#include "std_msgs/String.h"

#include "trex_robot_control/Constants.h"
#include "trex_robot_control/TrexRobotData.h"

static long int lastReadTime = 0;

void trexPwr_pi_messageCb(const trex_robot_control::TrexRobotData &robData_value)
{
  if((int)robData_value.timing_msg - lastReadTime > BATTERYINFOPUBLISH)
   {
     lastReadTime = (int)robData_value.timing_msg;
     int time_value = (int)(robData_value.timing_msg / 1000);
     int hours = time_value / 3600;
     int remainder = time_value % 3600;

     ROS_INFO("Battery Status: %s  Voltage: %f Time: %d : %d : %d", robData_value.power_msg.c_str(), (float)(robData_value.voltage_level / 100), hours, (int)(remainder / 60), (int)(remainder % 60));
   }
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "pi_trex_pwr_subscriber");
  ros::NodeHandle pwrNodeHandle;
  ros::Subscriber sub = pwrNodeHandle.subscribe("trex_pi_rob_data_talkback", 1000, trexPwr_pi_messageCb);

  ros::spin();

  return 0;
}
