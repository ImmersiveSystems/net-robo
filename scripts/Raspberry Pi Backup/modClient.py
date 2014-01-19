from socketIO_client import SocketIO

import serial

ser = serial.Serial("/dev/ttyUSB0", 115200)
print 'opened serial'

speed1 = 50
speed2 = 50
command = 'forward'

def listener(*args):
	print 'received a command'
	global command
	command = args[0]
	if args[0] == 'forward':
		print 'w'
	elif args[0] == 'backward':
		print 's'
	elif args[0] == 'left':
		print 'a'
	elif args[0] == 'right':
		print 'd'
	elif args[0] == 'stop':
		print 'x'
	elif args[0] == 'shiftr':
		print 'e'
	elif args[0] == 'shiftl':
		print 'q'

while True:
	if command == 'forward':
		mode1 = 2
		mode2 = 2
		speed1 *= 1.001
		speed2 *= 1.001

	if command == '-forward':
		mode1 = 2
		mode2 = 2
		speed1 *= 0.999
		speed2 *= 0.999

	if speed1 > 99:
		speed1 = 100
	if speed2 > 99:
		speed2 = 100



	print 'HB',  mode1, int(speed1), mode2, int(speed2)
	ser.write('HB')
	ser.write(chr(mode1))
	ser.write(chr(int(speed1)))
	ser.write(chr(mode2))
	ser.write(chr(int(speed2)))
 
socketIO = SocketIO('192.168.1.13', 3000)
socketIO.on('serverToPython', listener)
socketIO.emit('clientType', 'Python')
socketIO.wait(seconds=6000)   
