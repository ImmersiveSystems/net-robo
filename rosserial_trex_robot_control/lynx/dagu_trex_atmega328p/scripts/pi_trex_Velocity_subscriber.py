#!/usr/bin/env python

import rospy
import roslib

from dagu_trex_atmega328p.msg import TrexRobotData

roslib.load_manifest('dagu_trex_atmega328p')

from socketIO_client import SocketIO

def trex_pi_VelCb(data):
#    rospy.loginfo("Velocity: %f  Time: %f", data.robot_velocity, (data.timer_msg / 1000))
    socketIO.emit('lynxToServer', str(data.robot_velocity))

def pi_trex_Velocity_subscriber():
    rospy.init_node('pi_trex_Velocity_subscriber', anonymous = True)
    rospy.Subscriber('trex_pi_vel_talkback', TrexRobotData, trex_pi_VelCb)

    rospy.spin()


if __name__ == "__main__":
    try:
        socketIO = SocketIO('http://54.213.169.59', 3000)
        pi_trex_Velocity_subscriber()
        rospy.loginfo('Velocity Subscriber - Connected to the Cloudy Robotics')
        socketIO.emit('clientType', 'Pyhton')
        socketIO.wait(secs=6000)
    except rospy.ROSInterruptException:
        pass
