from socketIO_client import SocketIO
import serial

ser = serial.Serial("/dev/ttyUSB0", 115200)

# commands sent to the controller:

speed1 = 50
speed2 = 70
speed3 = 100

#defualt speed
Leftspeed = speed1
Rightspeed = speed1


controlmode = 1 # 1 = exploration, -1 = race

#modes 0=reverse, 1=brake, 2=forward

startcommand = "HB" # start of the message


def listener(*args):
    
        Leftmode = 1
        Rightmode = 1

        if args[0] == '-toggle':  #toggles the mode when the toggle button is released
            controlmode *= -1

        if controlmode == 1: # if exploration mode

            if args[0] == 'speedlow':
                Leftspeed = speed1
                Rightspeed = speed1

                
                
            elif args[0] == 'speedmed':
                Leftspeed = speed2
                Rightspeed = speed2

                
            elif args[0] == 'speedhi':
                Leftspeed = speed3
                Rightspeed = speed3
               
                
            elif args[0] == 'forward':
                Leftmode = 2
                Rightmode = 2
                
            elif args[0] == '-forward':
                Leftmode = 1
                Rightmode = 1
                
            elif args[0] == 'backward':
                Leftmode = 0
                Rightmode = 0

            elif args[0] == '-backward':
                Leftmode = 0
                Rightmode = 0
                
        if controlmode == -1: # if race mode
            #logic here
            pass


        ser.write(chr(startcommand))
        ser.write(chr(Leftmode))
        ser.write(chr(Leftspeed))    
        ser.write(chr(Rightmode))
        ser.write(chr(Rightspeed))


        
socketIO = SocketIO('192.168.1.33', 3000)
socketIO.on('serverToPython', listener)
socketIO.emit('clientType', 'Python')
socketIO.wait(seconds=6000)
