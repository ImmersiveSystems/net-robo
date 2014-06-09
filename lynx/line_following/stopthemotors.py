import serial
ser = serial.Serial("/dev/ttyUSB0", 9600)


ser.write('H' + chr(2) + chr(0) + chr(2) + chr(0))