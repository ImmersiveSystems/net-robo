import serial

ser = serial.Serial("COM5", 115200)

var = raw_input("Enter something: ")
print "You entered: ", var

ser.write("HB" + chr(2) + chr(127) + chr(0) + chr(127)) 

var = raw_input("Enter something: ")
print "You entered: ", var
