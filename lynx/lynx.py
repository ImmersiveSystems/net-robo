from socketIO_client import SocketIO

import serial
import threading

ser = serial.Serial("/dev/ttyUSB0", 9600)
print 'Opened serial'

exploSpeedMin = 30
exploSpeedMax = 100
exploSpeed = 60
SerialData = '0'

def Get_Encoders_Velcoity_Values():
    global SerialData

    SerialData = SerialData + ser.read(ser.inWaiting())
    if '\n' in SerialData:    
        RawData = SerialData.split('\n')
        if RawData[-1]:
               Values = RawData[-2] 
               print Values
               if Values != '':
                   Vel_Encoders = Values.split(' ') 
                   socketIO.emit('lynxToServer', Vel_Encoders[0])
               else:
                   socketIO.emit('lynxToServer', '0')
        else:
            socketIO.emit('lynxToServer', '0')
        SerialData = RawData[-1]
            #print VelocityData         

def listener(*args):
    global exploSpeedMin
    global exploSpeedMax
    global exploSpeed

    Get_Encoders_Velcoity_Values()

    if args[0] == 'forward':
        print 'Move FORWARD'
        ser.write('H' + chr(2) + chr(exploSpeed) + chr(2) + chr(exploSpeed))
        Get_Encoders_Velcoity_Values()
    elif (args[0] == '-forward') or (args[0] == '-right') or (args[0] == '-left') or (args[0] == '-backward'):
        print 'Stop movement'
        ser.write('H' + chr(2) + chr(0) + chr(2) + chr(0))
        Get_Encoders_Velcoity_Values()
    elif args[0] == 'backward':
        print 'Move BACKWARD'
        ser.write('H' + chr(0) + chr(exploSpeed) + chr(0) + chr(exploSpeed))
        Get_Encoders_Velcoity_Values()
    elif args[0] == 'right':
        print 'Turn RIGHT'
        ser.write('H' + chr(0) + chr(exploSpeed) + chr(2) + chr(exploSpeed))
        Get_Encoders_Velcoity_Values()
    elif args[0] == 'left':
        print 'Turn LEFT'
        ser.write('H' + chr(2) + chr(exploSpeed) + chr(0) + chr(exploSpeed))
        Get_Encoders_Velcoity_Values()
    elif args[0] == 'stop':
        print 'HALT'
        ser.write('H' + chr(2) + chr(0) + chr(2) + chr(0))
        Get_Encoders_Velcoity_Values()
    elif args[0] == 'speedup':
        if exploSpeed >= exploSpeedMax:
            exploSpeed = exploSpeedMax
            print 'Max speed reached'
        else:
            exploSpeed = exploSpeed + 5
            # print 'Speed increased to ' + str(exploSpeed)

        Get_Encoders_Velcoity_Values()
        # socketIO.emit('lynxToServer', str(exploSpeed))
    elif args[0] == 'speeddown':
        if exploSpeed <= exploSpeedMin:
            exploSpeed = exploSpeedMin
            print 'Min speed reached'
        else:
            exploSpeed = exploSpeed - 5
            # print 'Speed decreased to ' + str(exploSpeed)

        Get_Encoders_Velcoity_Values()
        # socketIO.emit('lynxToServer', str(exploSpeed))
    elif args[0].startswith('pan'):
        ser.write('P' + chr(int(args[0].strip('pan'))))
        print 'PANNING'
        Get_Encoders_Velcoity_Values()
    elif args[0].startswith('tilt'):
        ser.write('T' + chr(int(args[0].strip('tilt'))))
        print 'TILTING'
        Get_Encoders_Velcoity_Values()
    elif args[0] == 'elbowup':
        ser.write('E' + chr(1))
        print 'Move ELBOW UP'
        Get_Encoders_Velcoity_Values()
    elif args[0] == 'elbowdown':
        ser.write('E' + chr(0))
        print 'Move ELBOW DOWN'
        Get_Encoders_Velcoity_Values()
    elif args[0] == 'wristleft':
        ser.write('W' + chr(1))
        print 'Move WRIST LEFT'
        Get_Encoders_Velcoity_Values()
    elif args[0] == 'wristright':
        ser.write('W' + chr(0))
        print 'Move WRIST RIGHT'
        Get_Encoders_Velcoity_Values()
    elif args[0] == 'grab':
        ser.write('C')
        print 'Use CLAW'
        Get_Encoders_Velcoity_Values()

socketIO = SocketIO('192.168.1.223', 3000)
socketIO.on('serverToLynx', listener)
socketIO.emit('clientType', 'Python')
socketIO.wait(seconds=6000)