#include "ros/ros.h"
#include "std_msgs/String.h"
#include "dagu_trex_atmega328p/TrexRobotData.h"

void trexEncdr_pi_messageCb(const dagu_trex_atmega328p::TrexRobotData &robData_value)
{
	ROS_INFO("Left Encoder: %f Right Encoder: %f", (float)robData_value.left_encoder, (float)robData_value.right_encoder);
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "pi_trex_encoders_subscriber");
  ros::NodeHandle encdrNodeHandle;
  ros::Subscriber sub = encdrNodeHandle.subscribe("trex_pi_rob_data_talkback", 1000, trexEncdr_pi_messageCb);//("trex_pi_encoders_talkback", 1000, trexEncdr_pi_messageCb);

  ros::spin();

  return 0;
}
