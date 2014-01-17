from socketIO_client import SocketIO

import serial
import threading

ser = serial.Serial("COM15", 115200)
print 'opened serial'

voltage = 0
charged = 1

controlScheme = 0

exploSpeed1 = 40
exploSpeed2 = 60
exploSpeed3 = 100
exploSpeed = exploSpeed1

driveCommand  = ''
isTurning = False
accelerationRate = 5
decelerationRate = 1
turnRate = 1
shiftRate = 5
driveTurnSpeed = 40
driveSpeedLeft = 0
driveSpeedRight = 0
driveSavedSpeedLeft = 0
driveSavedSpeedRight = 0
driveModeLeft = 0
driveModeRight = 0

def listener(*args):
    global controlScheme
    global driveCommand
    global exploSpeed1
    global exploSpeed2
    global exploSpeed3
    global exploSpeed

    if controlScheme == 0:
        if args[0] == 'forward':
            print 'Move FORWARD'
            #ser.write('HB' + chr(2) + chr(exploSpeed) + chr(2) + chr(exploSpeed))
        elif (args[0] == '-forward') or (args[0] == '-right') or (args[0] == '-left') or (args[0] == '-backward'):
            print 'Stop movement'
            #ser.write('HB' + chr(2) + chr(0) + chr(2) + chr(0))
        elif args[0] == 'backward':
            print 'Move BACKWARD'
            #ser.write('HB' + chr(0) + chr(exploSpeed) + chr(0) + chr(exploSpeed))
        elif args[0] == 'right':
            print 'Turn RIGHT'
            #ser.write('HB' + chr(2) + chr(exploSpeed) + chr(0) + chr(exploSpeed))
        elif args[0] == 'left':
            print 'Turn LEFT'
            #ser.write('HB' + chr(0) + chr(exploSpeed) + chr(2) + chr(exploSpeed))
        elif args[0] == 'stop':
            print 'HALT'
            #ser.write('x')
        elif args[0] == 'shiftr':
            if controlScheme == 0:
                if exploSpeed == exploSpeed2:
                    print 'Speed increased'
                    exploSpeed = exploSpeed3;
                elif exploSpeed == exploSpeed1:
                    print 'Speed increased'
                    exploSpeed = exploSpeed2;
        elif args[0] == 'shiftl':
            if controlScheme == 0:
                if exploSpeed == exploSpeed2:
                    print 'Speed decreased'
                    exploSpeed = exploSpeed1;
                elif exploSpeed == exploSpeed3:
                    print 'Speed decreased'
                    exploSpeed = exploSpeed2;
        elif args[0] == 'toggle':
            print 'Driving mode'
            controlScheme = 1
            driveSpeedLeft = 0
            driveSpeedRight = 0
            #ser.write('HB' + chr(2) + chr(0) + chr(2) + chr(0))
    elif controlScheme == 1:
        driveCommand = args[0]

        if args[0] == 'stop':
            print 'HALT'
            #ser.write('x')
        elif args[0] == 'toggle':
            print 'Exploration mode'
            controlScheme = 0
            exploSpeed = exploSpeed1
            #ser.write('HB' + chr(2) + chr(0) + chr(2) + chr(0))

def drivingThread():
    global controlScheme
    global driveCommand
    global isTurning
    global accelerationRate
    global decelerationRate
    global turnRate
    global shiftRate
    global driveInitTurnSpeed
    global driveSpeedLeft
    global driveSpeedRight
    global driveSavedSpeedLeft
    global driveSavedSpeedRight
    global driveModeLeft
    global driveModeRight

    if controlScheme == 1:
        if (driveCommand != 'right') and (driveCommand != 'left') and (driveCommand != '-forward') and (driveCommand != '-right') and (driveCommand != '-left') and (driveCommand != '-backward'):
            if isTurning == True:
                driveSpeedLeft = driveSavedSpeedLeft
                driveSpeedRight = driveSavedSpeedRight
                isTurning = False

        if driveCommand == 'forward':
            print 'Go FORWARD'
            driveSpeedLeft = driveSpeedLeft + accelerationRate
            driveSpeedRight = driveSpeedRight + accelerationRate
        elif (driveCommand == '-forward') or (driveCommand == '-right') or (driveCommand == '-left') or (driveCommand == '-backward'):
            print 'Wait'
        elif driveCommand == 'backward':
            print 'Go BACKWARD'
            driveSpeedLeft = driveSpeedLeft - accelerationRate
            driveSpeedRight = driveSpeedRight - accelerationRate
        elif driveCommand == 'right':
            print 'Turn RIGHT'
            if isTurning == False or (driveSpeedLeft > 0 and driveSpeedRight < 0):
                driveSavedSpeedLeft = driveSpeedLeft
                driveSavedSpeedRight = driveSpeedRight
                driveSpeedRight = driveTurnSpeed
                driveSpeedLeft = -1 * driveTurnSpeed
                isTurning = True
            else:
                driveSpeedRight = driveSpeedRight + turnRate
                driveSpeedLeft = driveSpeedLeft - turnRate

        elif driveCommand == 'left':
            print 'Turn LEFT'
            if isTurning == False or (driveSpeedLeft < 0 and driveSpeedRight > 0):
                driveSavedSpeedLeft = driveSpeedLeft
                driveSavedSpeedRight = driveSpeedRight
                driveSpeedLeft = driveTurnSpeed
                driveSpeedRight = -1 * driveTurnSpeed
                isTurning = True
            else:
                driveSpeedLeft = driveSpeedLeft + turnRate
                driveSpeedRight = driveSpeedRight - turnRate

        elif driveCommand == 'shiftr':
            print 'Steer RIGHT'
            if driveSpeedRight > 0 and driveSpeedLeft > 0:
                if driveSpeedLeft > driveSpeedRight:
                    driveSpeedLeft = driveSpeedLeft - shiftRate;
                else:
                    driveSpeedRight = driveSpeedRight + shiftRate;
            elif driveSpeedRight < 0 and driveSpeedLeft < 0:
                if driveSpeedRight > driveSpeedLeft:
                    driveSpeedLeft = driveSpeedLeft + shiftRate
                else:
                    driveSpeedRight = driveSpeedRight - shiftRate
        elif driveCommand == 'shiftl':
            print 'Steer LEFT'
            if driveSpeedLeft > 0 and driveSpeedRight > 0:
                if driveSpeedRight > driveSpeedLeft:
                    driveSpeedRight = driveSpeedRight - shiftRate;
                else:
                    driveSpeedLeft = driveSpeedLeft + shiftRate;
            elif driveSpeedLeft < 0 and driveSpeedRight < 0:
                if driveSpeedLeft > driveSpeedRight:
                    driveSpeedRight = driveSpeedRight + shiftRate
                else:
                    driveSpeedLeft = driveSpeedLeft - shiftRate

        if driveSpeedLeft > 0:
            if isTurning == False:
                driveSpeedLeft = driveSpeedLeft - decelerationRate;
            driveModeLeft = 2
        elif driveSpeedLeft < 0:
            if isTurning == False:
                driveSpeedLeft = driveSpeedLeft + decelerationRate;
            driveModeLeft = 0
        if driveSpeedRight > 0:
            if isTurning == False:
                driveSpeedRight = driveSpeedRight - decelerationRate;
            driveModeRight = 2
        elif driveSpeedRight < 0:
            if isTurning == False:
                driveSpeedRight = driveSpeedRight + decelerationRate;
            driveModeRight  = 0;
        
        if driveSpeedLeft > 100:
            driveSpeedLeft = 100
        elif driveSpeedLeft < -100:
            driveSpeedLeft = -100
        if driveSpeedRight > 100:
            driveSpeedRight = 100
        elif driveSpeedRight < -100:
            driveSpeedRight = -100

        print 'Left Speed: ' + str(driveSpeedLeft) + '   Right Speed: ' + str(driveSpeedRight)
        #ser.write('HB' + chr(driveModeLeft) + chr(abs(driveSpeedLeft)) + chr(driveModeRight) + chr(abs(driveSpeedRight)))

    threading.Timer(0.1, drivingThread).start()

def read_from_port(ser):  
    global voltage, charged

    while True:
        reading = ser.readline()

        if reading == 'P\r\n':
            voltage =  ser.readline()
            print voltage

        if reading == 'C\r\n':
            charged =  ser.readline()
            if charged == '1\r\n':
                #print 'its trying to charge!'
                pass
            if charged == '0\r\n':
                #print 'its charged!'
                pass
            
        if reading == 'Timer: \r\n':
            timer =  ser.readline()
            #print 'Timer: ', timer      


thread = threading.Thread(target=read_from_port, args=(ser,))
thread.start()



drivingThread()

socketIO = SocketIO('localhost', 3000)
socketIO.on('serverToPython', listener)
socketIO.emit('clientType', 'Python')
socketIO.wait(seconds=6000)