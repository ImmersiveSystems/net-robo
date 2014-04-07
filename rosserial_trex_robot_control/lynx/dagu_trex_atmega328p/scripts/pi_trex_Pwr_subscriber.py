#!/usr/bin/env python

import rospy
import roslib

from dagu_trex_atmega328p.msg import TrexRobotData 

roslib.load_manifest('dagu_trex_atmega328p')

from socketIO_client import SocketIO

def trex_pi_PwrCb(data):
    rospy.loginfo("Power: %f Time: %d", (data.voltage_level / 100), (data.timer_msg / 1000))
    socketIO.emit('lynxToServer', str(data.voltage_level/100))

def pi_trex_Pwr_subscriber():
    rospy.init_node('pi_trex_Pwr_subscriber', anonymous = True)
    rospy.Subscriber('trex_pi_power_talkback', TrexRobotData, trex_pi_PwrCb)

    rospy.spin()


if __name__ == "__main__":
    try:
        socketIO = SocketIO('http://54.213.169.59', 3000)
        pi_trex_Pwr_subscriber()
        rospy.loginfo('Power Subscriber - Connected to the Cloudy Robotics')
        socketIO.emit('clientType', 'Pyhton')
        socketIO.wait(secs=6000)
    except rospy.ROSInterruptException:
        pass
