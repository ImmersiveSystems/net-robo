import serial
ser = serial.Serial("/dev/ttyUSB0", 9600)
uno = serial.Serial("/dev/ttyACM0", 9600)

print 'Opened serial'

threshold = 150


while(True):
	data = uno.read()
	if data == 'L':
		dataIn = uno.readline()

		sensors = dataIn.split()
		values = list()

		for sensor in sensors:
			print sensor
			if (sensor.isdigit()):
				values.append(int(sensor))
			else:
				break
				

		print(values)


