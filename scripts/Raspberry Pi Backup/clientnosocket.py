from socketIO_client import SocketIO
import serial

ser = serial.Serial("/dev/ttyUSB0", 115200)

# commands sent to the controller:

speed1 = 100  #whatever the min speed is
speed2 = 70
speed3 = 100


accel = 1.001
decel = 0.999


#defualt speed
Leftspeed = speed1
Rightspeed = speed1
Leftmode = 2
Rightmode = 2
command = 'default'

minspeed = 60/2.25

controlmode = 1 # 1 = exploration, -1 = race


#modes 0=reverse, 1=brake, 2=forward

startcommand = "HB" # start of the message


def listener(*arg):
    global controlmode, command, Leftspeed, Rightspeed
    command = args[0]
    #correct indentation!

    if command == '-toggle':  #toggles the mode when the toggle button is released
        controlmode *= -1

    if controlmode == 1: # if exploration mode

        if command == 'speedlow':
            Leftspeed = speed1
            Rightspeed = speed1

        elif command == 'speedmed':
            Leftspeed = speed2
            Rightspeed = speed2

        elif command == 'speedhi':
            Leftspeed = speed3
            Rightspeed = speed3


        elif command == 'forward':
            Leftmode = 2
            Rightmode = 2
            if Leftspeed == 0 and Rightspeed == 0:
                Leftspeed = speed1
                Rightspeed = speed1    

        elif command == '-forward':
            Leftmode = 1
            Rightmode = 1

        elif command == 'backward':
            Leftmode = 0
            Rightmode = 0

        elif command == '-backward':
            Leftmode = 0
            Rightmode = 0

        elif command == 'left':
            Leftmode = 2
            Rightmode = 0

        elif command == 'right':
            Leftmode = 0
            Rightmode = 2

        elif command == '-left':
            Leftmode = 1
            Rightmode = 1

        elif command == '-right':
            Leftmode = 1
            Rightmode = 1



    if controlmode == -1: # if race mode
        if command == 'forward':
            if Leftspeed <= 0 and Rightspeed <= 0:
                Leftmode = 2
                Rightmode = 2                    
                Leftspeed = speed1
                Rightspeed = speed1
            else:
                pass

        if command == 'backward':
            if Leftspeed >= 0 and Rightspeed >= 0:
                Leftmode = 0
                Rightmode = 0                    
                Leftspeed = speed1
                Rightspeed = speed1
            else:
                pass

socketIO = SocketIO('192.168.1.13', 3000)
socketIO.on('serverToPython', listener)
socketIO.emit('clientType', 'Python')
socketIO.wait(seconds=6000) 

while True:

#    if controlmode == 1:
#        accel = 1.01
#        decel = 0.99
#    if contolmode == -1:
#        accel = 1
#        decel = 1       
  
    
    accel = 1.001
    decel = 0.999
    
    if command == 'forward':
        Rightspeed *= accel
        Leftspeed *= accel

    if command == '-forward':
        Rightspeed *= decel
        Leftspeed *= decel

    if Rightspeed > 225:
        Rightspeed = 225

    if Leftspeed > 225:
        Leftspeed = 225
        
    if Rightspeed < 60:
        Rightspeed = 0

    if Leftspeed < 60:
        Leftspeed = 0

        
    print Rightspeed, Leftspeed, "we are writing", startcommand, (Leftmode), (int(Leftspeed)), (Rightmode), (int(Rightspeed)) 
    ser.write((startcommand))
    ser.write(chr(Leftmode))
    ser.write(chr(int(Leftspeed)))    
    ser.write(chr(Rightmode))
    ser.write(chr(int(Rightspeed)))
                
            
        
    

