#!/usr/bin/env python

import rospy
import roslib

from dagu_trex_atmega328p.msg import TrexRobotData 

roslib.load_manifest('dagu_trex_atmega328p')

from socketIO_client import SocketIO

lastReadTimer = 0
PUBLISH_TIME = 10000

def trex_pi_PwrCb(data):
    global lastReadTimer 
    global PUBLISH_TIME
    if (data.timer_msg - lastReadTimer > PUBLISH_TIME):
     lastReadTimer = data.timer_msg
     rospy.loginfo("Power: %f Time: %d", (data.voltage_level / 100), (data.timer_msg / 1000))
     socketIO.emit('lynxToServer', str(data.voltage_level/100))

def pi_trex_Pwr_subscriber():
    rospy.init_node('pi_trex_Pwr_subscriber', anonymous = True)
    rospy.Subscriber('trex_pi_rob_data_talkback', TrexRobotData, trex_pi_PwrCb)

    rospy.spin()


if __name__ == "__main__":
    try:
        socketIO = SocketIO('http://54.213.169.59', 3000)
        rospy.loginfo('Power Subscriber - Connected to the Cloudy Robotics')
        pi_trex_Pwr_subscriber()
        socketIO.emit('clientType', 'Pyhton')
        socketIO.wait(seconds=6000)
    except rospy.ROSInterruptException:
        pass
