from socketIO_client import SocketIO

import serial

ser = serial.Serial("/dev/ttyUSB0", 115200)
print 'opened serial'

speed1 = 40
speed2 = 60
speed3 = 100

speed = speed1

def listener(*args):
        global speed
        print 'recieved a command'
        if args[0] == 'forward':
                print 'w'
                ser.write('HB' + chr(2) + chr(speed) + chr(2) + chr(speed))

        elif args[0] == '-forward' or args[0] == '-right' or args[0] == '-left'  o$
                print 'stop'
                ser.write('HB' + chr(2) + chr(0) + chr(2) + chr(0))

        elif args[0] == 'backward':
                print 's'
                ser.write('HB' + chr(0) + chr(speed) + chr(0) + chr(speed))

        elif args[0] == 'right':
                print 'r'
                ser.write('HB' + chr(2) + chr(speed) + chr(0) + chr(speed))

        elif args[0] == 'left':
                print 'l'
                ser.write('HB' + chr(0) + chr(speed) + chr(2) + chr(speed))       $

        elif args[0] == 'stop':
                print 'x'
                ser.write('x')


        elif args[0] == 'speed1':
                speed = speed1
        elif args[0] == 'speed2':
                speed = speed2
        elif args[0] == 'speed3':
                speed = speed3

        elif args[0] == 'shiftr':
                speed = speed1

        elif args[0] == 'shiftl':
                speed = speed3



socketIO = SocketIO('192.168.1.13', 3000)
socketIO.on('serverToPython', listener)
socketIO.emit('clientType', 'Python')
socketIO.wait(seconds=6000)

