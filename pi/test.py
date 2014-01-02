import serial

ser = serial.Serial("COM6", 115200)


for i in range(100):

    speed = raw_input("Enter speed: ")
    print "You entered: ", speed
    speed = int(speed)

    mode = raw_input("Enter mode: ")
    print "You entered: ", mode
    mode = int(mode)

    ser.write("HB" + chr(mode) + chr(speed) + chr(mode) + chr(speed))

    print( ser.read() )
