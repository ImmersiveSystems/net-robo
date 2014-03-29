#!/usr/bin/env python
import rospy
import roslib
from trex_robot_control.msg import TrexMotorCmds
roslib.load_manifest('trex_robot_control')

from socketIO_client import SocketIO

exploSpeedMin = 60
exploSpeedMax = 80
exploSpeed = 60
goingForward = 0
goingBackward = 0
turningR = 0
turningL = 0
turningAngle = 50 # this is the difference between the speed of right and left motors

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
    global goingForward
    global goingBackward
    global turningR
    global turningL

    # this is to calculate turning speed 
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
        if turningR: 
            setCmdValues(cmdtype = 72, msg = "Move Forward", l_mode = 2, l_speed = lowSpeed, r_mode = 2, r_speed = topSpeed) # 72 : decimal equivalent for "H"
        elif turningL:
            setCmdValues(cmdtype = 72, msg = "Move Forward", l_mode = 2, l_speed = topSpeed, r_mode = 2, r_speed = lowSpeed)
        else:
            setCmdValues(cmdtype = 72, msg = "Move Forward", l_mode = 2, l_speed = exploSpeed, r_mode = 2, r_speed = exploSpeed)
        goingForward = 1
    elif (args[0] == '-forward') or (args[0] == '-right') or (args[0] == '-left') or (args[0] == '-backward'):
        #stop turning but continue going forward/backward
        if (args[0] == '-right' or args[0] == '-left') and goingForward:
            setCmdValues(cmdtype = 72, msg = "Move Forward", l_mode = 2, l_speed = exploSpeed, r_mode = 2, r_speed = exploSpeed)
        elif (args[0] == '-right' or args[0] == '-left') and goingBackward:
            setCmdValues(cmdtype = 72, msg = "Move Backward", l_mode = 0, l_speed = exploSpeed, r_mode = 0, r_speed = exploSpeed)
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
        setCmdValues(cmdtype = 72, msg = "Halt", l_mode = 2, l_speed = 0, r_mode = 2, r_speed = 0)
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
        rospy.loginfo(cmd_value)
        pub.publish(cmd_value)
        # rospy.sleep(1.0)    


socketIO = SocketIO('http://54.213.169.59', 3000)
rospy.loginfo("Connected to the server")
socketIO.on('serverToThumper', pi_trex_publisher)
socketIO.emit('clientType', 'Python')
socketIO.wait(seconds=6000)

# if __name__ == '__main__':
#     try:
#        socketIO = SocketIO('http://54.213.169.59', 3000)    
#        rospy.loginfo("Connected to the Server")
#        socketIO.on('serverToThumper', pi_trex_publisher)
#        socketIO.emit('clientType', 'Python')
#        socketIO.wait(seconds=6000)
#        pi_trex_publisher('forward')
#        pi_trex_publisher('backward')
#        pi_trex_publisher('left')
#        pi_trex_publisher('right')
#        pi_trex_publisher('speedup')
#        pi_trex_publisher('speeddown')
#        pi_trex_publisher('stop')
#     except rospy.ROSInterruptException:
#          pass
