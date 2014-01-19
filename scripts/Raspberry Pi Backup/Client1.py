from socketIO_client import SocketIO

import serial

ser = serial.Serial("/dev/ttyUSB1", 115200)

def listener(*args):
	if args[0] == 'forward':
		print 'w'
		ser.write('w')
	elif args[0] == 'backward':
		print 's'
		ser.write('s')
	elif args[0] == 'left':
		print 'a'
		ser.write('a')
	elif args[0] == 'right':
		print 'd'
		ser.write('d')
	elif args[0] == 'stop':
		print 'x'
		ser.write('x')
	elif args[0] == 'shiftr':
		print 'e'
		ser.write('e')
	elif args[0] == 'shiftl':
		print 'q'
		ser.write('q')

socketIO = SocketIO('192.168.1.33', 3000)
socketIO.on('serverToPython', listener)
socketIO.emit('clientType', 'Python')
socketIO.wait(seconds=6000)
