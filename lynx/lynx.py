from socketIO_client import SocketIO

import serial
import threading

ser = serial.Serial("/dev/ttyACM0", 9600)
print 'Opened serial'

exploSpeed1 = 40
exploSpeed2 = 60
exploSpeed3 = 100
exploSpeed = exploSpeed1

def listener(*args):
    global exploSpeed1
    global exploSpeed2
    global exploSpeed3
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
        if exploSpeed == exploSpeed2:
            print 'Speed increased'
            exploSpeed = exploSpeed3;
        elif exploSpeed == exploSpeed1:
            print 'Speed increased'
            exploSpeed = exploSpeed2;
    elif args[0] == 'shiftl':
        if exploSpeed == exploSpeed2:
            print 'Speed decreased'
            exploSpeed = exploSpeed1;
        elif exploSpeed == exploSpeed3:
            print 'Speed decreased'
            exploSpeed = exploSpeed2;
    elif args[0].startswith('pan'):
        ser.write('P' + chr(int(args[0].strip('pan'))))
        print int(args[0].strip('pan'))
    elif args[0].startswith('tilt'):
        ser.write('T' + chr(int(args[0].strip('tilt'))))
        print int(args[0].strip('tilt'))

socketIO = SocketIO('192.168.1.33', 3000)
socketIO.on('serverToPython', listener)
socketIO.emit('clientType', 'Python')
socketIO.wait(seconds=6000)