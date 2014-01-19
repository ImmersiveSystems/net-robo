import serial

ser = serial.Serial("/dev/ttyUSB1", 115200)

for i in range(10000):

    #print ser.read()
    #print ser.readline()
    
    speed = raw_input("Enter speed: ")
    speed = int(speed)

    modeL = raw_input("Enter mode1: ")
    modeL = int(modeL)

    modeR = raw_input("Enter mode2: ")
    modeR = int(modeR)
    
    ser.write("HB" + chr(modeL) + chr(speed) + chr(modeR) + chr(speed))

ser.close()

