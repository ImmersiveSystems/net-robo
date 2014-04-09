#include "ros/ros.h"
#include "std_msgs/String.h"
#include "dagu_trex_atmega328p/TrexRobotData.h"

void trexRobLoc_pi_messageCb(const dagu_trex_atmega328p::TrexRobotData &robData_value)
{
   ROS_INFO("X: %d Y: %d  Theta: %d", (int)robData_value.x_coord, (int)robData_value.y_coord, (int)robData_value.heading_angle);
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "pi_trex_rob_loc_subscriber");
  ros::NodeHandle pwrNodeHandle;
  ros::Subscriber sub = pwrNodeHandle.subscribe("trex_pi_rob_data_talkback", 1000, trexRobLoc_pi_messageCb);//("trex_pi_rob_loc_talkback", 1000, trexPwr_pi_messageCb);

  ros::spin();

  return 0;
}
