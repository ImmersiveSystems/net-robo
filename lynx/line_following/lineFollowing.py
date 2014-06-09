import serial
import time
ser = serial.Serial("/dev/ttyUSB0", 9600)
uno = serial.Serial("/dev/ttyACM0", 9600)

print 'Opened serial'

threshold = 800  #more than that = the line, less that that = backgroung
exploSpeed = 50
exploSpeed2 = 40
gospeed = 35
lowBattery = 1
foundLine = 0

def TurningLeft():
	# pass
	ser.write('H' + chr(0) + chr(exploSpeed2) + chr(2) + chr(exploSpeed))
def TurningRight():
	# pass
	ser.write('H' + chr(2) + chr(exploSpeed) + chr(0) + chr(exploSpeed2))
def GoingStraight():
	# pass
	ser.write('H' + chr(2) + chr(gospeed) + chr(2) + chr(gospeed))

	

while (True):

	firstChar = uno.read()
	if firstChar == 'L':  #Light sensor

		dataIn = uno.readline()

		sensors = dataIn.split()
		values = []

		for sensor in sensors:
			if (sensor.isdigit()):
				values.append(int(sensor))
				badData = 0
			else:
				badData = 1
				break


		if badData or len(values) < 3:
			continue



		print(values)
		sens1 = values[0]
		sens2 = values[1]
		sens3 = values[2]





		if(lowBattery and not foundLine):	
			foundLine = sens1 > threshold or sens2 > threshold or sens3 > threshold
			GoingStraight()



		else:
			#follow the line
			if sens1 > threshold :
				TurningRight()

			# elif sens2 > threshold:
			# 	GoingStraight()

			elif (sens3 > threshold):
				TurningLeft()

			else:
				# GoingStraight()
				# ser.write('H' + chr(2) + chr(0) + chr(2) + chr(0))
				pass











