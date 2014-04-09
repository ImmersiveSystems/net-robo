#include "ros/ros.h"
#include "std_msgs/String.h"
#include "dagu_trex_atmega328p/TrexRobotData.h"

void trexVel_pi_messageCb(const dagu_trex_atmega328p::TrexRobotData &robData_value)
{
	ROS_INFO("Velocity: %d", (int)robData_value.robot_velocity);
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "pi_trex_vel_subscriber");
  ros::NodeHandle velNodeHandle;
  ros::Subscriber sub = velNodeHandle.subscribe("trex_pi_rob_data_talkback", 1000, trexVel_pi_messageCb);//("trex_pi_vel_talkback", 1000, trexPwr_pi_messageCb);

  ros::spin();

  return 0;
}
