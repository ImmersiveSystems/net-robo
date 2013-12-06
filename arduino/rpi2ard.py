#short script to control robot from RPi
import serial
ser = serial.Serial('/dev/ttyUSB0', 9600)  #name of the port where arduino is connected

print('w - left side \ns - right side, \nq - stop all motors \ne - stop the program')
while 1:
        input = raw_input('enter w,s,q or e and hit enter: ')
        input = input[0]
        ser.write(input)
        if input == 'e':
                break
