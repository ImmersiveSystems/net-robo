#!/usr/bin/env python

import rospy
import roslib

from dagu_trex_atmega328p.msg import TrexRobotData

roslib.load_manifest('dagu_trex_atmega328p')

from socketIO_client import SocketIO

def trex_pi_LocCb(data):
    rospy.loginfo("X: %d Y: %d Theta: %d", data.x_coord, data.y_coord, data.heading_angle)
    socketIO.emit('lynxToServer', str(data.x_coord) + str(data.y_coord) + str(data.heading_angle))

def pi_trex_RobLoc_subscriber():
    rospy.init_node('pi_trex_RobLoc_subscriber', anonymous = True)
    rospy.Subscriber('trex_pi_rob_loc_talkback', TrexRobotData, trex_pi_LocCb)

    rospy.spin()


if __name__ == "__main__":
    try:
        socketIO = SocketIO('http://54.213.169.59', 3000)
        pi_trex_Pwr_subscriber()
        rospy.loginfo('Position Subscriber - Connected to the Cloudy Robotics')
        socketIO.emit('clientType', 'Pyhton')
        socketIO.wait(secs=6000)
    except rospy.ROSInterruptException:
        pass
