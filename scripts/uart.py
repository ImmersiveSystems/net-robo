import serial

ser = serial.Serial("/dev/ttyAMA0", 115200)

while True:
	var = raw_input("Enter something: ")
	print "You entered: ", var
	ser.write(var) 
