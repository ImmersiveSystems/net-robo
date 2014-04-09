#include "ros/ros.h"
#include "std_msgs/String.h"
//#include "std_msgs/Int32.h"
#include "dagu_trex_atmega328p/TrexRobotData.h"

void trexAccel_pi_messageCb(const dagu_trex_atmega328p::TrexRobotData &robData_value)
{
  	ROS_INFO("Acceleration: %f", (float)robData_value.robot_accel);
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "pi_trex_accel_subscriber");
  ros::NodeHandle accelNodeHandle;
  ros::Subscriber sub = accelNodeHandle.subscribe("trex_pi_rob_data_talkback", 1000, trexAccel_pi_messageCb);//("trex_pi_accel_talkback", 1000, trexAccel_pi_messageCb);

  ros::spin();

  return 0;
}
