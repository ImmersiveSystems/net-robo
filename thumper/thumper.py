from socketIO_client import SocketIO

import serial
import threading

ser = serial.Serial("/dev/ttyUSB0", 9600)
print 'Opened serial'

exploSpeedMin = 60
exploSpeedMax = 80
exploSpeed = 60
SerialData = '0'
goingForward = 0
goingBackward = 0
turningR = 0
turningL = 0
turningAngle = 50 # this is the difference between the speed of right and left motors

def listener(*args):
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
        print 'Move FORWARD'

        if turningR:
            ser.write('H' + chr(2) + chr(lowSpeed) + chr(2) + chr(topSpeed))
        elif turningL:
            ser.write('H' + chr(2) + chr(topSpeed) + chr(2) + chr(lowSpeed))
        else:
            ser.write('H' + chr(2) + chr(exploSpeed) + chr(2) + chr(exploSpeed))
        goingForward = 1

    elif (args[0] == '-forward') or (args[0] == '-right') or (args[0] == '-left') or (args[0] == '-backward'):
        print 'Stop movement'

        #stop turning but continue going forward/backward
        if (args[0] == '-right' or args[0] == '-left') and goingForward:
            ser.write('H' + chr(2) + chr(exploSpeed) + chr(2) + chr(exploSpeed))
        elif (args[0] == '-right' or args[0] == '-left') and goingBackward:
            ser.write('H' + chr(0) + chr(exploSpeed) + chr(0) + chr(exploSpeed))
        else:     
            ser.write('H' + chr(2) + chr(0) + chr(2) + chr(0))

        if args[0] == '-forward':
            goingForward = 0
        elif args[0] == '-backward':
            goingBackward = 0
        elif args[0] == '-right':
            turningR = 0
        elif args[0] == '-left':
            turningL = 0

        ser.write('H' + chr(2) + chr(0) + chr(2) + chr(0))

    elif args[0] == 'backward':
        print 'Move BACKWARD'
        if turningR:
            ser.write('H' + chr(0) + chr(lowSpeed) + chr(0) + chr(topSpeed))
        elif turningL:
            ser.write('H' + chr(0) + chr(topSpeed) + chr(0) + chr(lowSpeed))
        else:
            ser.write('H' + chr(0) + chr(exploSpeed) + chr(0) + chr(exploSpeed))

        goingBackward = 1

    elif args[0] == 'left':
        print 'Turn LEFT'
        if goingForward == 1:
            ser.write('H' + chr(2) + chr(lowSpeed) + chr(2) + chr(topSpeed))
        elif goingBackward == 1:
            ser.write('H' + chr(0) + chr(lowSpeed) + chr(0) + chr(topSpeed))        
        else:
            ser.write('H' + chr(0) + chr(exploSpeed) + chr(2) + chr(exploSpeed))
        turningL = 1

    elif args[0] == 'right':
        print 'Turn RIGHT'
        if goingForward == 1:
            ser.write('H' + chr(2) + chr(topSpeed) + chr(2) + chr(lowSpeed))
        elif goingBackward == 1:
            ser.write('H' + chr(0) + chr(topSpeed) + chr(0) + chr(lowSpeed)) 
        else:
            ser.write('H' + chr(2) + chr(exploSpeed) + chr(0) + chr(exploSpeed))
        turningR = 1

    elif args[0] == 'stop':
        print 'HALT'
        ser.write('H' + chr(2) + chr(0) + chr(2) + chr(0))

    elif args[0] == 'speedup':
        if exploSpeed >= exploSpeedMax:
            exploSpeed = exploSpeedMax
            print 'Max speed reached'
        else:
            exploSpeed = exploSpeed + 5
            print 'Speed increased to ' + str(exploSpeed)

        socketIO.emit('thumperToServer', str(exploSpeed))
    elif args[0] == 'speeddown':
        if exploSpeed <= exploSpeedMin:
            exploSpeed = exploSpeedMin
            print 'Min speed reached'
        else:
            exploSpeed = exploSpeed - 5
            print 'Speed decreased to ' + str(exploSpeed)

        socketIO.emit('thumperToServer', str(exploSpeed))

socketIO = SocketIO('54.213.169.59', 3000)
socketIO.on('serverToThumper', listener)
socketIO.emit('clientType', 'Python')
socketIO.wait(seconds=6000)