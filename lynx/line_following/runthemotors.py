import time
import serial
ser = serial.Serial("/dev/ttyUSB0", 9600)
uno = serial.Serial("/dev/ttyACM0", 9600)

while (True):
	start = time.time()
	ser.write('H' + chr(2) + chr(50) + chr(2) + chr(50))
	end = time.time()
	print "ser" + str(end - start)

	start = time.time()
	firstChar = uno.read()
	end = time.time()
	print "uno" + str(end - start)