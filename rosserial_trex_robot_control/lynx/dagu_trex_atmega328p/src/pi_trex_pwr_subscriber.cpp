#include "ros/ros.h"
#include "std_msgs/String.h"

#include "dagu_trex_atmega328p/TrexRobotData.h"

void trexPwr_pi_messageCb(const dagu_trex_atmega328p::TrexRobotData &pwr_value)
{
   ROS_INFO("Volatge: %f Time: %d sec.", (float)(pwr_value.voltage_level / 100), ((int)pwr_value.timer_msg / 1000));
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "pi_trex_pwr_subscriber");
  ros::NodeHandle pwrNodeHandle;
  ros::Subscriber sub = pwrNodeHandle.subscribe("trex_pi_power_talkback", 1000, trexPwr_pi_messageCb);

  ros::spin();

  return 0;
}
