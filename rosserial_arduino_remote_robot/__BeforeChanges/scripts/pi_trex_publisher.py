#!/usr/bin/env python
import rospy
import roslib
from trex_robot_control.msg import TrexMotorCmds
roslib.load_manifest('trex_robot_control')

from socketIO_client import SocketIO

exploSpeedMin = 30
exploSpeedMax = 60
exploSpeed = 30

cmd_value = TrexMotorCmds()
pub = rospy.Publisher('pi_trex_chatter', TrexMotorCmds)
rospy.init_node('pi_trex_publisher')

def setCmdValues(cmdtype, msg, l_mode, l_speed, r_mode, r_speed):
    global cmd_value
    cmd_value.cmd_type = cmdtype
    cmd_value.cmd_msg = msg
    cmd_value.left_mode = l_mode
    cmd_value.right_mode = r_mode
    cmd_value.left_speed = l_speed
    cmd_value.right_speed = r_speed

def resetCmdSpeed(spd):
    global cmd_value
    cmd_value.left_speed = spd
    cmd_value.right_speed = spd

def pi_trex_publisher(*args):
    global exploSpeedMin
    global exploSpeedMax
    global exploSpeed
    global cmd_value
    
    if args[0] == 'forward':
        setCmdValues(cmdtype = 72, msg = "Move Forward", l_mode = 2, l_speed = exploSpeed, r_mode = 2, r_speed = exploSpeed)   # 72 : decimal equivalent for "H"
    elif (args[0] == '-forward') or (args[0] == '-right') or (args[0] == '-left') or (args[0] == '-backward'):
        setCmdValues(cmdtype = 72, msg = "Stop", l_mode = 2, l_speed=0, r_mode = 2, r_speed = 0)
    elif args[0] == 'backward':
        setCmdValues(cmdtype = 72, msg = "Move Backward", l_mode = 0, l_speed = exploSpeed, r_mode = 0, r_speed = exploSpeed)
    elif args[0] == 'right':
        setCmdValues(cmdtype = 72, msg = "Turn Right", l_mode = 0, l_speed = exploSpeed, r_mode = 2, r_speed = exploSpeed)
    elif args[0] == 'left':
        setCmdValues(cmdtype = 72, msg = "Turn Left", l_mode = 2, l_speed = exploSpeed, r_mode = 0, r_speed = exploSpeed)
    elif args[0] == 'stop':
        setCmdValues(cmdtype = 72, msg = "Stop", l_mode = 2, l_speed = 0, r_mode = 2, r_speed = 0)
    elif args[0] == 'sleep':
        setCmdValues(cmdtype = 90, msg = "Going to Sleep", l_mode = 2, l_speed = 0, r_mode = 2, r_speed = 0) # 90 : decimal equivalent for "Z"
    elif args[0] == 'speedup':
        if exploSpeed >= exploSpeedMax:
            exploSpeed = exploSpeedMax
            cmd_value.cmd_msg = "Max. Speed Reached"
            resetCmdSpeed(spd = exploSpeed)
        else:
            exploSpeed = exploSpeed + 5
            cmd_value.cmd_msg = "Speed increased to %s" % exploSpeed 
            resetCmdSpeed(spd = exploSpeed)

        socketIO.emit('thumperToServer', str(exploSpeed))
    elif args[0] == 'speeddown':
        if exploSpeed <= exploSpeedMin:
            exploSpeed = exploSpeedMin
            cmd_value.cmd_msg = "Min. Speed Reached"
            resetCmdSpeed(spd = exploSpeed)
        else:
            exploSpeed = exploSpeed - 5
            cmd_value.cmd_msg = "Speed Decreased to %s" % exploSpeed
            resetCmdSpeed(spd = exploSpeed)        

        socketIO.emit('thumperToServer', str(exploSpeed))

    if not rospy.is_shutdown():
        # rospy.loginfo(cmd_value)
        pub.publish(cmd_value)



if __name__ == '__main__':
    try:
       socketIO = SocketIO('http://54.213.169.59', 3000)    
       rospy.loginfo("Connected to the Server")
       socketIO.on('serverToThumper', pi_trex_publisher)
       socketIO.emit('clientType', 'Python')
       socketIO.wait(seconds=6000)
    except rospy.ROSInterruptException:
         pass
