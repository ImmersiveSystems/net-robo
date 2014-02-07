from socketIO_client import SocketIO

import serial
import threading

ser = serial.Serial("/dev/ttyUSB0", 115200)
print 'Opened serial'

exploSpeedMin = 40
exploSpeedMax = 60
exploSpeed = 30

def listener(*args):
    global exploSpeedMin
    global exploSpeedMax
    global exploSpeed

    if args[0] == 'forward':
        print 'Move FORWARD'
        ser.write('H' + chr(2) + chr(exploSpeed) + chr(2) + chr(exploSpeed))
    elif (args[0] == '-forward') or (args[0] == '-right') or (args[0] == '-left') or (args[0] == '-backward'):
        print 'Stop movement'
        ser.write('H' + chr(2) + chr(0) + chr(2) + chr(0))
    elif args[0] == 'backward':
        print 'Move BACKWARD'
        ser.write('H' + chr(0) + chr(exploSpeed) + chr(0) + chr(exploSpeed))
    elif args[0] == 'right':
        print 'Turn RIGHT'
        ser.write('H' + chr(0) + chr(exploSpeed) + chr(2) + chr(exploSpeed))
    elif args[0] == 'left':
        print 'Turn LEFT'
        ser.write('H' + chr(2) + chr(exploSpeed) + chr(0) + chr(exploSpeed))
    elif args[0] == 'stop':
        print 'HALT'
        ser.write('H' + chr(2) + chr(0) + chr(2) + chr(0))
    elif args[0] == 'shiftr':
        if exploSpeed >= exploSpeedMax:
            exploSpeed = exploSpeedMax
            print 'Max speed reached'
        else:
            exploSpeed = exploSpeed + 5
            print 'Speed increased to ' + str(exploSpeed)
    elif args[0] == 'shiftl':
        if exploSpeed <= exploSpeedMin:
            exploSpeed = exploSpeedMin
            print 'Min speed reached'
        else:
            exploSpeed = exploSpeed - 5
            print 'Speed decreased to ' + str(exploSpeed)
    elif args[0].startswith('pan'):
        ser.write('P' + chr(int(args[0].strip('pan'))))
        print 'pan'
    elif args[0].startswith('tilt'):
        ser.write('T' + chr(int(args[0].strip('tilt'))))
        print 'tilt'
    elif args[0].startswith('elbowup'):
        ser.write('E' + chr(1))
        print 'elbow up'
    elif args[0].startswith('elbowdown'):
        ser.write('E' + chr(0))
        print 'elbow down'
    elif args[0].startswith('wristleft'):
        ser.write('W' + chr(1))
        print 'wrist left'
    elif args[0].startswith('wristright'):
        ser.write('W' + chr(0))
        print 'wrist right'
    elif args[0].startswith('clawopen'):
        ser.write('C')# + chr(1))
        print 'claw open'
    elif args[0].startswith('clawclose'):
        ser.write('C') #+ chr(0))
        print 'claw close'

socketIO = SocketIO('192.168.1.223', 3000)
socketIO.on('serverToThumper', listener)
socketIO.emit('clientType', 'Python')
socketIO.wait(seconds=6000)