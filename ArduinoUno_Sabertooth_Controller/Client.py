import serial
import sys


ser = serial.Serial("COM4", 9600)


while True:
    speed = raw_input("Enter speed: ")
##    print "You entered: ", speed
    speed = int(speed)

    modeL = raw_input("Enter mode1: ")
##    print "You entered: ", modeL
    modeL = int(modeL)

    modeR = raw_input("Enter mode2: ")
##    print "You entered: ", modeR
    modeR = int(modeR)

    if modeL > 3 or modeR > 3 or modeL < 0 or modeR < 0:
        continue;
    else:
        ser.write("H" + chr(modeL) + chr(speed) + chr(modeR) + chr(speed))
    flag = raw_input("Continue (y/n)?")
    if flag == 'n' or flag == 'N':
        break;
    
ser.close()
sys.exit()