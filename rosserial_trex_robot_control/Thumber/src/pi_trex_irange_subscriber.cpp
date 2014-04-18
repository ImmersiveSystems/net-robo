#include "ros/ros.h"
#include "std_msgs/String.h"
//#include "sensor_msgs/Range.h"
#include "trex_robot_control/TrexRobotData.h"

void trexIRange_pi_messageCb(const trex_robot_control::TrexRobotData &robData_value)//(const sensor_msgs::Range &range_msg)
{
  	ROS_INFO("IR Range: %d", (int)robData_value.irange_dist);//range_msg.range);
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "pi_trex_irange_subscriber");
  ros::NodeHandle irangeNodeHandle;
  ros::Subscriber sub = irangeNodeHandle.subscribe("trex_pi_rob_data_talkback", 1000, trexIRange_pi_messageCb);

  ros::spin();

  return 0;
}
