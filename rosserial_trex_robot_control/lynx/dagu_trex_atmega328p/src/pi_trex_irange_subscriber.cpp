#include "ros/ros.h"
#include "std_msgs/String.h"
#include "dagu_trex_atmega328p/TrexRobotData.h"

void trexIRange_pi_messageCb(const dagu_trex_atmega328p::TrexRobotData &pwr_value)
{
	   ROS_INFO("IR Distance: %d", (int)pwr_value.irange_dist);
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "pi_trex_irange_subscriber");
  ros::NodeHandle irangeNodeHandle;
  ros::Subscriber sub = irangeNodeHandle.subscribe("trex_pi_irange_talkback", 1000, trexIRange_pi_messageCb);

  ros::spin();

  return 0;
}
