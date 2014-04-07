#!/usr/bin/env python

import rospy
import roslib

from dagu_trex_atmega328p.msg import TrexServoMotorCmds
roslib.load_manifest('dagu_trex_atmega328p')

from socketIO_client import SocketIO

exploSpeedMin = 60
exploSpeedMax = 100
exploSpeed = 60
goingForward = 0
goingBackward = 0
turningR = 0
turningL = 0
turningAngle = 50
stop = 0

cmd_value = TrexServoMotorCmds()
pub = rospy.Publisher('pi_trex_chatter', TrexServoMotorCmds)
rospy.init_node('pi_trex_publisher')

def setCmdValues(cmdtype, msg, l_mode, l_speed, r_mode, r_speed):
    global cmd_value
    cmd_value.cmd_type = cmdtype
    cmd_value.cmd_msg = msg
    cmd_value.left_mode = l_mode
    cmd_value.left_speed = l_speed
    cmd_value.right_mode = r_mode
    cmd_value.right_speed = r_speed
    cmd_value.elbow_move = -1
    cmd_value.wrist_move = -1

def setPanCmdValues(cmdtype, msg, panpos):
    global cmd_value
    cmd_value.cmd_type = cmdtype
    cmd_value.cmd_msg = msg
    cmd_value.pan_pos = panpos
    cmd_value.elbow_move = -1
    cmd_value.wrist_move = -1

def setTiltCmdValues(cmdtype, msg, tiltpos):
    global cmd_value
    cmd_value.cmd_type = cmdtype
    cmd_value.cmd_msg = msg
    cmd_value.tilt_pos = tiltpos
    cmd_value.elbow_move = -1
    cmd_value.wrist_move = -1

def setElbowCmdValues(cmdtype, msg, updown):
    global cmd_value
    cmd_value.cmd_type = cmdtype
    cmd_value.cmd_msg = msg
    cmd_value.elbow_move = updown
    cmd_value.wrist_move = -1

def setClawCmdValues(cmdtype, msg):
    global cmd_value
    cmd_value.cmd_type = cmdtype
    cmd_value.cmd_msg = msg
    cmd_value.elbow_move = -1
    cmd_value.wrist_move = -1
    
def setWristCmdValues(cmdtype, msg, leftright):
    global cmd_value
    cmd_value.cmd_type = cmdtype
    cmd_value.cmd_msg = msg
    cmd_value.wrist_move = leftright
    cmd_value.elbow_move = -1
    cmd_value.wrist_move = -1    

def resetCmdSpeed(spd):
    global cmd_value
    cmd_value.left_speed = spd
    cmd_value.right_speed = spd

def pi_trex_publisher(*args):
    global exploSpeedMin
    global exploSpeedMax
    global exploSpeed
    global goingForward
    global goingBackward
    global turningR
    global turningL
    global turningAngle

    if exploSpeed > exploSpeedMax - int(turningAngle / 2):
        topSpeed = exploSpeedMax
        lowSpeed = exploSpeedMax - turningAngle
    elif (exploSpeed - int(turningAngle / 2)) <= 0:
        lowSpeed = 0
        topSpeed = turningAngle
    else:
        topSpeed = exploSpeed + int(turningAngle / 2)
        lowSpeed = exploSpeed - int(turningAngle / 2)

    if args[0] == 'forward':
        if stop:
            pass
        else:
            if turningR:
                setCmdValues(cmdtype = 72, msg = "Move Forward", l_mode = 2, l_speed = lowSpeed, r_mode = 2, r_speed = topSpeed)
            elif turningL:
                setCmdValues(cmdtype = 72, msg = "Move Forward", l_mode = 2, l_speed = topSpeed, r_mode = 2, r_speed = lowSpeed)
            else:
                setCmdValues(cmdtype = 72, msg = "Move Forward", l_mode = 2, l_speed = exploSpeed, r_mode = 2, r_speed = exploSpeed)
            goingForward = 1
    elif(args[0] == '-forward') or (args[0] == '-right') or (args[0] == '-left') or (args[0] == '-backward'):
        if (args[0] == '-right') or (args[0] == '-left') and goingForward:
            setCmdValues(cmdtype = 72, msg = "Move Forward", l_mode = 2, l_speed = exploSpeed, r_mode = 2, r_speed = exploSpeed)
        elif (args[0] == '-right') or (args[0] == '-left') and goingBackward:
            setCmdValues(cmdtype = 72, msg = "Move Forward", l_mode = 0, l_speed = exploSpeed, r_mode = 0, r_speed = exploSpeed)
        else:
            setCmdValues(cmdtype = 72, msg = "Stop", l_mode = 2, l_speed = 0, r_mode = 2, r_speed = 0)
        if args[0] == '-forward':
            goingForward = 0
        elif args[0] == '-backward':
            goingBackward = 0
        elif args[0] == '-right':
            turningR = 0
        elif args[0] == '-left':
            turningL = 0
        setCmdValues(cmdtype = 72, msg = "Stop", l_mode = 2, l_speed = 0, r_mode = 2, r_speed = 0)
    elif args[0] == 'backward':
        if turningR:
            setCmdValues(cmdtype = 72, msg = "Move Backward", l_mode = 0, l_speed = lowSpeed, r_mode = 0, r_speed = topSpeed)
        elif turningL:
            setCmdValues(cmdtype = 72, msg = "Move Backward", l_mode = 0, l_speed = topSpeed, r_mode = 0, r_speed = lowSpeed)
        else:
            setCmdValues(cmdtype = 72, msg = "Move Backward", l_mode = 0, l_speed = exploSpeed, r_mode = 0, r_speed = exploSpeed)
        goingBackward = 1
    elif args[0] == 'left':
        if goingForward == 1:
            setCmdValues(cmdtype = 72, msg = "Turn Left", l_mode = 2, l_speed = lowSpeed, r_mode = 2, r_speed = topSpeed)
        elif goingBackward == 1:
            setCmdValues(cmdtype = 72, msg = "Turn Left", l_mode = 0, l_speed = lowSpeed, r_mode = 0, r_speed = topSpeed)
        else:
            setCmdValues(cmdtype = 72, msg = "Turn Left", l_mode = 0, l_speed = exploSpeed, r_mode = 2, r_speed = exploSpeed)
        turningL = 1
    elif args[0] == 'right':
        if goingForward == 1:
            setCmdValues(cmdtype = 72, msg = "Turn Right", l_mode = 2, l_speed = topSpeed, r_mode = 2, r_speed = lowSpeed)
        elif goingBackward == 1:
            setCmdValues(cmdtype = 72, msg = "Turn Right", l_mode = 0, l_speed = topSpeed, r_mode = 0, r_speed = lowSpeed)
        else:
            setCmdValues(cmdtype = 72, msg = "Turn Right", l_mode = 2, l_speed = exploSpeed, r_mode = 0, r_speed = exploSpeed)
        turningR = 1
    elif args[0] == 'stop':
        setCmdValues(cmdtype = 72, msg = "Stop", l_mode = 2, l_speed = 0, r_mode = 2, r_speed = 0)
    elif args[0] == 'speedup':
        if exploSpeed >= exploSpeedMax:
            exploSpeed = exploSpeedMax
            cmd_value.cmd_msg = "Max. Speed Reached"
        else:
            exploSpeed = exploSpeed + 5
            cmd_value.cmd_msg = "Speed Increased to %s" % exploSpeed

    elif args[0] == 'speeddown':
        if exploSpeed <= exploSpeedMin:
            exploSpeed = exploSpeedMin
            cmd_value.cmd_msg = "Min. Speed Reached"
        else:
            exploSpeed = exploSpeed - 5
            cmd_value.cmd_msg = "Speed Decreased to %s" % exploSpeed

    elif args[0].startswith('pan'):
        setPanCmdValues(cmdtype = 80, msg = "Panning", panpos = int(args[0].strip('pan')))

    elif args[0].startswith('tilt'):
        setTiltCmdValues(cmdtype = 84, msg = "Tilting", tiltpos = int(args[0].strip('tilt')))

    elif args[0].startswith('elbowup'):
        setElbowCmdValues(cmdtype = 69, msg = "Move Elbow Up", updown = 1)

    elif args[0].startswith('elbowdown'):
        setElbowCmdValues(cmdtype = 69, msg = "Move Elbow Down", updown = 0)

    elif args[0].startswith('wristleft'):
        setWristCmdValues(cmdtype = 87, msg = "Move Wrist Left", leftright = 1)

    elif args[0].startswith('wristright'):
        setWristCmdValues(cmdtype = 87, msg = "Move Wrist Right", leftright = 0)

    elif args[0] == 'grab':
        setClawCmdValues(cmdtype = 67, msg = "Use the Claw")

    socketIO.emit('lynxToServer', str(exploSpeed))
    
    if not rospy.is_shutdown():
        rospy.loginfo(cmd_value)
        pub.publish(cmd_value)    


if __name__ == '__main__':
    try:
       socketIO = SocketIO('http://54.213.169.59', 3000)
       rospy.loginfo('Connected to the Cloudy Robotics')
       socketIO.on('serverToLynx', pi_trex_publisher)
       socketIO.emit('clientType', 'Python')
       socketIO.wait(seconds = 6000)
    except rospy.ROSInterruptException:
       pass 
