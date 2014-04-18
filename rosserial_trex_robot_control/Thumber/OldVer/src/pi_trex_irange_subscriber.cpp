#include "ros/ros.h"
#include "std_msgs/String.h"
#include "sensor_msgs/Range.h"

void trexIRange_pi_messageCb(const sensor_msgs::Range &range_msg)
{
  	ROS_INFO("IR Range: %f", (float)range_msg.range);
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "pi_trex_irange_subscriber");
  ros::NodeHandle rosNodeHandle;
  ros::Subscriber sub = rosNodeHandle.subscribe("trex_pi_irange_talkback", 1000, trexIRange_pi_messageCb);

  ros::spin();

  return 0;
}
