#include "ros/ros.h"
#include "std_msgs/String.h"

#include "trex_robot_control/Constants.h"
#include "trex_robot_control/TrexRobotData.h"


void trexUltrasonic_pi_messageCb(const trex_robot_control::TrexRobotData &robData_value)
{
    ROS_INFO("Ultrasonic Distance: %d", (int)robData_value.ultrasonic_dist);
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "pi_trex_ultrasonic_subscriber");
  ros::NodeHandle ultrasonicNodeHandle;
  ros::Subscriber sub = ultrasonicNodeHandle.subscribe("trex_pi_rob_data_talkback", 1000, trexUltrasonic_pi_messageCb);

  ros::spin();

  return 0;
}
