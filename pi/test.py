import serial
import sys


ser = serial.Serial("COM6", 115200)


for i in range(100):

    print (ser.read())


    speed = raw_input("Enter speed: ")
    print "You entered: ", speed
    speed = int(speed)

    modeL = raw_input("Enter mode1: ")
    print "You entered: ", modeL
    modeL = int(modeL)

    modeR = raw_input("Enter mode2: ")
    print "You entered: ", modeR
    modeR = int(modeR)

    
    if modeL == 9:
        ser.close()
        sys.exit()

        

    ser.write("HB" + chr(modeL) + chr(speed) + chr(modeR) + chr(speed))

ser.close()
