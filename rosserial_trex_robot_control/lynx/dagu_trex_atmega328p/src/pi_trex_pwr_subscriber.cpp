#include "ros/ros.h"
#include "std_msgs/String.h"

#include "dagu_trex_atmega328p/Constants.h"
#include "dagu_trex_atmega328p/TrexRobotData.h"

static long int lastReadTime = 0;

void trexPwr_pi_messageCb(const dagu_trex_atmega328p::TrexRobotData &robData_value)
{
   if((int)robData_value.timer_msg - lastReadTime > BATTERYINFOPUBLISH)
   {
     lastReadTime = (int)robData_value.timer_msg;
     int time_value = (int)(robData_value.timer_msg / 1000);
     int hours;
     int minutes;
     int seconds;
     hours = time_value / 3600;
     int remainder = time_value % 3600;

     ROS_INFO("Time: %d : %d : %d\t  Voltage: %f", hours, (int)(remainder / 60), (int)(remainder % 60), (float)(robData_value.voltage_level / 100));
   }
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "pi_trex_pwr_subscriber");
  ros::NodeHandle pwrNodeHandle;
  ros::Subscriber sub = pwrNodeHandle.subscribe("trex_pi_rob_data_talkback", 1000, trexPwr_pi_messageCb);

  ros::spin();

  return 0;
}
