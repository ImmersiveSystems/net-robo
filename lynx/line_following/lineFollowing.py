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

posn = 0
proportional = 0
integral = 0
derivative = 0
last_proportional = 0
set_point = 10
offset = 80
sensor_average = 0
sensor_sum = 0
Kp = 0.37
Ki = 1/100000
Kd = 0.7
error_value = 0
max_difference=55
max_speed = 40
set_point = 2000
right_speed = 1
left_speed = 1
left_direction = 2
right_direction = 2

def TurningLeft():
	ser.write('H' + chr(0) + chr(exploSpeed) + chr(2) + chr(exploSpeed))

def TurningLeftFast():
	ser.write('H '+ chr(0) + chr(exploSpeed*2) + chr(2) + chr(exploSpeed*2))

def TurningRight():
	ser.write('H' + chr(2) + chr(exploSpeed) + chr(0) + chr(exploSpeed))

def TurningRightFast():
	ser.write('H' + chr(2) + chr(exploSpeed*2) + chr(0) + chr(exploSpeed*2))

def GoingStraight():
	ser.write('H' + chr(2) + chr(gospeed) + chr(2) + chr(gospeed))

def Motor_Drive(right_speed, left_speed):
	print (right_speed, left_speed)
	global right_direction, left_direction
	ser.write('H' + chr(left_direction) + chr(left_speed) + chr(right_direction) + chr(right_speed))

def CalPID():
	global posn, proportional, integral, derivative, last_proportional, sensor_average, sensor_sum, error_value, Kp, Ki, Kd
	posn = sensor_average / sensor_sum
	proportional = posn - set_point
	integral = integral + proportional
	derivative = proportional - last_proportional
	last_proportional = proportional
	error_value = int(proportional * Kp + integral * Ki + derivative * Kd)/10
	print ('proportional: '+str(proportional) +' integral: '+ str(integral) +' derivative: '+ str(derivative) + ' error_value: ' + str(error_value))
	
def read_sensors(value1, value2, value3):
	global sensor_sum, sensor_average
	sensor_average = (value1*1+value2*2+value3*3)*1000
	sensor_sum = value1+value2+value3

def Cal_Turn():
	global error_value, max_difference, right_speed, left_speed, max_speed, right_direction, left_direction
	if (error_value > max_difference):
		error_value = max_difference
	if(error_value < -max_difference):
		error_value = -max_difference

	#print('error_value1: ' + str(error_value))
	if (error_value > 2):
		right_speed = max_speed + error_value + 30
		if(right_speed > 100):
			right_speed = 95
		left_speed = 50
		right_direction = 2
		left_direction = 0

	elif (error_value > 0 and error_value < 2):
		right_speed = max_speed
		left_speed = max_speed
		right_direction = 2
		left_direction = 0

	else:
		right_speed = 50
		left_speed = max_speed - error_value + 30
		if(left_speed > 100):
			left_speed = 95
		right_direction = 0
		left_direction = 2

while (True):
	global foundLine
	firstChar = uno.read()
	if firstChar == 'L':  #Light sensor value

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
			print("badData")
			continue



		print(values)
		sens1 = values[0]
		sens2 = values[1]
		sens3 = values[2]

		read_sensors(sens1, sens2, sens3)
		CalPID()
		Cal_Turn()
		Motor_Drive(right_speed, left_speed)

		# if(lowBattery and not foundLine):	
		# 	foundLine = sens1 > threshold or sens2 > threshold or sens3 > threshold
		# 	GoingStraight()



		# else:
		# 	#follow the line
		# 	if (sens1 > threshold and sens2 < threshold and sens3 < threshold):
		# 		TurningRight()
		# 		print("Turning Right 001")

		# 	elif (sens1 < threshold and sens2 > threshold and sens3 < threshold):
		# 	 	GoingStraight()
		# 	 	print("Going Straight 010")

		# 	elif (sens1 < threshold and sens2 < threshold and sens3 > threshold):
		# 		TurningLeft()
		# 		print("Turning Left 100")

		# 	elif (sens1 > threshold and sens2 > threshold and sens3 < threshold):
		# 		TurningRightFast()
		# 		# for x in xrange(1,10):
		# 		# 	pass
		# 		print("Turning Right 011")

		# 	elif (sens1 < threshold and sens2 > threshold and sens3 > threshold):
		# 		TurningLeftFast()
		# 		# for x in xrange(1,10):
		# 		# 	pass
		# 		print("Turning Left 110")

		# 	elif (sens1 > threshold and sens2 > threshold and sens3 > threshold):
		# 		TurningLeft()
		# 		print("Turning Left 111")
			
		# 	elif (sens1 < threshold and sens2 < threshold and sens3 < threshold):
		# 		pass
		# 		print("Going Straight 000")

		# 	elif (sens1 > threshold and sens2 < threshold and sens3 > threshold):
		# 		pass
		# 		print("Going Straight 101")
		# 		# ser.write('H' + chr(2) + chr(0) + chr(2) + chr(0))
				
		# 	else:
		# 	 	print("pass")
		# 	 	pass










