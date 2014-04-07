#include "ros/ros.h"
#include "std_msgs/String.h"
#include "dagu_trex_atmega328p/TrexRobotData.h"

void trexPwr_pi_messageCb(const dagu_trex_atmega328p::TrexRobotData &pwr_value)
{
	ROS_INFO("Left Encoder: %f Right Encoder: %f", (float)pwr_value.left_encoder, (float)pwr_value.right_encoder);
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "pi_trex_encoders_subscriber");
  ros::NodeHandle encdrNodeHandle;
  ros::Subscriber sub = encdrNodeHandle.subscribe("trex_pi_encoders_talkback", 1000, trexPwr_pi_messageCb);

  ros::spin();

  return 0;
}
