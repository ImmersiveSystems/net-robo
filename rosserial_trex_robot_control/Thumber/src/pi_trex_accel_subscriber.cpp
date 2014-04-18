#include "ros/ros.h"
#include "std_msgs/String.h"
//#include "std_msgs/Int32.h"
#include "trex_robot_control/TrexRobotData.h"

void trexAccel_pi_messageCb(const trex_robot_control::TrexRobotData &robData_value)//std_msgs::Int32 &accel_value)
{
  	ROS_INFO("Acceleration: %f", (float)robData_value.robot_accel);//%d", (int)accel_value.data);
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "pi_trex_accel_subscriber");
  ros::NodeHandle rosNodeHandle;
  ros::Subscriber sub = rosNodeHandle.subscribe("trex_pi_rob_data_talkback", 1000, trexAccel_pi_messageCb);

  ros::spin();

  return 0;
}
