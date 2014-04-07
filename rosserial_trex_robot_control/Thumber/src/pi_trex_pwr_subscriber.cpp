#include "ros/ros.h"
#include "std_msgs/String.h"

#include "trex_robot_control/TrexPowerMsg.h"

void trexPwr_pi_messageCb(const trex_robot_control::TrexPowerMsg &pwr_value)
{
  	ROS_INFO("Battery Status: %s Voltage: %f Time: %f sec.", pwr_value.power_msg.c_str(), (float)(pwr_value.voltage_level / 100), (float)(pwr_value.timing_msg/1000));
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "pi_trex_pwr_subscriber");
  ros::NodeHandle rosNodeHandle;
  ros::Subscriber sub = rosNodeHandle.subscribe("trex_pi_power_talkback", 1000, trexPwr_pi_messageCb);

  ros::spin();

  return 0;
}
