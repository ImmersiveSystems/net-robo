#!/usr/bin/env python

import rospy
import roslib

from trex_robot_control.msg import TrexPowerMsg

roslib.load_manifest('trex_robot_control')

from socketIO_client import SocketIO

def trex_pi_PwrCb(data):
#    rospy.loginfo("Power: %f", data.voltage_level)
    socketIO.emit('thumperToServer', str(data.voltage_level/100))

def pi_trex_Pwr_Subscriber():
    rospy.init_node('pi_trex_Pwr_Subscriber', anonymous = True)
    rospy.Subscriber("trex_pi_power_talkback", TrexPowerMsg, trex_pi_PwrCb)

    rospy.spin()
    


if __name__ == '__main__':
    try:
       socketIO = SocketIO('http://54.213.169.59', 3000)
       pi_trex_Pwr_Subscriber()
       rospy.loginfo('Power Subscriber - Connected to the Cloudy Robotics')
       socketIO.emit('clientType', 'Python')
       socketIO.wait(seconds = 6000)
    except rospy.ROSInterruptException:
       pass 

