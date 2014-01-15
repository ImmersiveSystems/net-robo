"""
 this script reads what arduino writes and outputs it in an easy to read format
It also runs the motors at full speed all the time
"""

import serial
ser = serial.Serial("COM15", 115200)

while True:
	ser.write("HB" + chr(2) + chr(99) + chr(2) + chr(99))

	reading = ser.readline()

	if reading == 'P\r\n':
		voltage =  ser.readline()
		print voltage,

	if reading == 'C\r\n':
		charged =  ser.readline()
		if charged == '1\r\n':
			print 'its trying to charge!'
		if charged == '0\r\n':
			print 'its charged!'
		
	if reading == 'Timer: \r\n':
		timer =  ser.readline()
		print 'Timer: ', timer		