from socketIO_client import SocketIO
import threading
import serial

class GlobalVariables:
	def __init__(self):
		self.__exploSpeed1 = 40
		self.__exploSpeed2 = 60
		self.__exploSpeed3 = 100
		self.__accelerationRate = 5
		self.__decelerationRate = 1	
		self.__turnRate = 1
		self.__shiftRate = 5
		self.__driveTurnSpeed = 40
		self.__MaxDriveSpeed = 100
		self.__MinDriveSpeed = -100

	def Get_MaxDriveSpeed(self):
		return self.__MaxDriveSpeed

	def Get_MinDriveSpeed(self):
		return self.__MinDriveSpeed
		
	def Get_exploSpeed1(self):
		return self.__exploSpeed1

	def Get_exploSpeed2(self):
		return self.__exploSpeed2

	def Get_exploSpeed3(self):
		return self.__exploSpeed3		

	def Get_AccelRate(self):
		return self.__accelerationRate

	def Get_DecelRate(self):
		return self.__decelerationRate

	def Get_TurnRate(self):
		return self.__turnRate

	def Get_ShiftRate(self):
		return self.__shiftRate

	def Get_DriveTurnSpeed(self):
		return self.__driveTurnSpeed


class Client(GlobalVariables):
	def __init__(self, GlobalVariables):
		self.__voltage = 0
		self.__charged = 1
		self.__controlScheme = 0
		self.__exploSpeed = GlobalVariables.Get_exploSpeed1()
		self.__driveCommand  = ''
		self.__isTurning = False
		self.__driveSpeedLeft = 0
		self.__driveSpeedRight = 0
		self.__driveSavedSpeedLeft = 0
		self.__driveSavedSpeedRight = 0
		self.__driveModeLeft = 0
		self.__driveModeRight = 0

	def Get_ControlScheme(self):
		return self.__controlScheme

	def DrivingThread(self):
	    gVar = GlobalVariables()
	    if self.__controlScheme == 1:
	        if (self.__driveCommand != 'right') and (self.__driveCommand != 'left') and (self.__driveCommand != '-forward') and (self.__driveCommand != '-right') and (self.__driveCommand != '-left') and (self.__driveCommand != '-backward'):
	            if self.__isTurning == True:
	                self.__driveSpeedLeft = self.__driveSavedSpeedLeft
	                self.__driveSpeedRight = self.__driveSavedSpeedRight
	                self.__isTurning = False

	        if self.__driveCommand == 'forward':
	            print 'Go FORWARD'
	            self.__driveSpeedLeft = self.__driveSpeedLeft + gVar.Get_AccelRate()
	            self.__driveSpeedRight = self.__driveSpeedRight + gVar.Get_AccelRate()
	        elif (self.__driveCommand == '-forward') or (self.__driveCommand == '-right') or (self.__driveCommand == '-left') or (self.__driveCommand == '-backward'):
	            print 'Wait'
	        elif self.__driveCommand == 'backward':
	            print 'Go BACKWARD'
	            self.__driveSpeedLeft = self.__driveSpeedLeft - gVar.Get_AccelRate()
	            self.__driveSpeedRight = self.__driveSpeedRight - gVar.Get_AccelRate()
	        elif self.__driveCommand == 'right':
	            print 'Turn RIGHT'
	            if self.__isTurning == False or (self.__driveSpeedLeft > 0 and self.__driveSpeedRight < 0):
	                self.__driveSavedSpeedLeft = self.__driveSpeedLeft
	                self.__driveSavedSpeedRight = self.__driveSpeedRight
	                self.__driveSpeedRight = gVar.Get_DriveTurnSpeed()
	                self.__driveSpeedLeft = -1 * gVar.Get_DriveTurnSpeed()
	                self.__isTurning = True
	            else:
	                self.__driveSpeedRight = self.__driveSpeedRight + gVar.Get_TurnRate()
	                self.__driveSpeedLeft = self.__driveSpeedLeft - gVar.Get_TurnRate()

	        elif self.__driveCommand == 'left':
	            print 'Turn LEFT'
	            if self.__isTurning == False or (self.__driveSpeedLeft < 0 and self.__driveSpeedRight > 0):
	                self.__driveSavedSpeedLeft = self.__driveSpeedLeft
	                self.__driveSavedSpeedRight = self.__driveSpeedRight
	                self.__driveSpeedLeft = gVar.Get_DriveTurnSpeed()
	                self.__driveSpeedRight = -1 * gVar.Get_DriveTurnSpeed()
	                self.__isTurning = True
	            else:
	                self.__driveSpeedLeft = self.__driveSpeedLeft + gVar.Get_TurnRate()
	                self.__driveSpeedRight = self.__driveSpeedRight - gVar.Get_TurnRate()

	        elif self.__driveCommand == 'shiftr':
	            print 'Steer RIGHT'
	            if self.__driveSpeedRight > 0 and self.__driveSpeedLeft > 0:
	                if self.__driveSpeedLeft > self.__driveSpeedRight:
	                    self.__driveSpeedLeft = self.__driveSpeedLeft - gVar.Get_ShiftRate()
	                else:
	                    self.__driveSpeedRight = self.__driveSpeedRight + gVar.Get_ShiftRate()
	            elif self.__driveSpeedRight < 0 and self.__driveSpeedLeft < 0:
	                if self.__driveSpeedRight > self.__driveSpeedLeft:
	                    self.__driveSpeedLeft = self.__driveSpeedLeft + gVar.Get_ShiftRate()
	                else:
	                    self.__driveSpeedRight = self.__driveSpeedRight - gVar.Get_ShiftRate()
	        elif self.__driveCommand == 'shiftl':
	            print 'Steer LEFT'
	            if self.__driveSpeedLeft > 0 and self.__driveSpeedRight > 0:
	                if self.__driveSpeedRight > self.__driveSpeedLeft:
	                    self.__driveSpeedRight = self.__driveSpeedRight - gVar.Get_ShiftRate()
	                else:
	                    self.__driveSpeedLeft = self.__driveSpeedLeft + gVar.Get_ShiftRate()
	            elif self.__driveSpeedLeft < 0 and self.__driveSpeedRight < 0:
	                if self.__driveSpeedLeft > self.__driveSpeedRight:
	                    self.__driveSpeedRight = self.__driveSpeedRight + gVar.Get_ShiftRate()
	                else:
	                    self.__driveSpeedLeft = self.__driveSpeedLeft - gVar.Get_ShiftRate()
	        if self.__driveSpeedLeft > 0:
	            if self.__isTurning == False:
	                self.__driveSpeedLeft = self.__driveSpeedLeft - gVar.Get_DecelRate()
	            self.__driveModeLeft = 2
	        elif self.__driveSpeedLeft < 0:
	            if self.__isTurning == False:
	                self.__driveSpeedLeft = self.__driveSpeedLeft + gVar.Get_DecelRate()
	            self.__driveModeLeft = 0
	        if self.__driveSpeedRight > 0:
	            if self.__isTurning == False:
	                self.__driveSpeedRight = self.__driveSpeedRight - gVar.Get_DecelRate()
	            self.__driveModeRight = 2
	        elif self.__driveSpeedRight < 0:
	            if self.__isTurning == False:
	                self.__driveSpeedRight = self.__driveSpeedRight + gVar.Get_DecelRate()
	            self.__driveModeRight  = 0;
	        
	        self.CheckDriveSpeed()

	    threading.Timer(0.1, self.DrivingThread).start()

	def CheckDriveSpeed(self):
		gVar = GlobalVariables()
		if self.__driveSpeedLeft > gVar.Get_MaxDriveSpeed():
		    self.__driveSpeedLeft = gVar.Get_MaxDriveSpeed()
		elif self.__driveSpeedLeft < gVar.Get_MinDriveSpeed():
		    self.__driveSpeedLeft = gVar.Get_MinDriveSpeed()
		if self.__driveSpeedRight > gVar.Get_MaxDriveSpeed():
		    self.__driveSpeedRight = gVar.Get_MaxDriveSpeed()
		elif self.__driveSpeedRight < gVar.Get_MinDriveSpeed():
		    self.__driveSpeedRight = gVar.Get_MinDriveSpeed()
		print 'Left Speed: ' + str(self.__driveSpeedLeft) + '   Right Speed: ' + str(self.__driveSpeedRight)
		# self.__serial.write('HB' + chr(self.__driveModeLeft) + chr(abs(self.__driveSpeedLeft)) + chr(self.__driveModeRight) + chr(abs(self.__driveSpeedRight)))        

	def read_from_port(self, serialPort):  
	    while True:
	        reading = serialPort.readline()

	        if reading == 'P\r\n':
	            self.__voltage =  serialPort.readline()
	            print self.__voltage

	        if reading == 'C\r\n':
	            self.__charged =  serialPort.readline()
	            if self.__charged == '1\r\n':
	                #print 'its trying to charge!'
	                pass
	            if self.__charged == '0\r\n':
	                #print 'its charged!'
	                pass
	            
	        if reading == 'Timer: \r\n':
	            timer =  serialPort.readline()
	            #print 'Timer: ', timer      

	def OpenSerialPort(self):
		# self.__serial = serial.Serial("COM15", 115200)
		print 'opened serial'

	def BatteryStatusThread(self):
		print 'Uncomment serial_port_threading'
		# thread = threading.Thread(target = self.read_from_port, args = (self.__serial,))
		# thread.start()

	def Talk2Server(self):
		socketIO = SocketIO('localhost', 3000)
		socketIO.on('serverToPython', self.listener)
		socketIO.emit('clientType', 'Python')
		socketIO.wait(seconds = 6000)

	def ListenIO(self):
		socketIO = SocketIO('localhost', 3000)
		socketIO.on('serverToPython', self.listener)
		socketIO.emit('clientType', 'Python')
		socketIO.wait(seconds = 6000)

	def listener(self, *args):
	    gVar = GlobalVariables()
	    if self.__controlScheme == 0:
	    	# print 'Length of args:  ', len(args), '\n'
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
	            if self.__controlScheme == 0:
	                if self.__exploSpeed == gVar.Get_exploSpeed2():
	                    print 'Speed increased'
	                    self.__exploSpeed =  gVar.Get_exploSpeed3();
	                elif self.__exploSpeed == gVar.Get_exploSpeed1():
	                    print 'Speed increased'
	                    self.__exploSpeed = gVar.Get_exploSpeed2();
	        elif args[0] == 'shiftl':
	            if self.__controlScheme == 0:
	                if self.__exploSpeed == gVar.Get_exploSpeed2():
	                    print 'Speed decreased'
	                    self.__exploSpeed = gVar.Get_exploSpeed1();
	                elif self.exploSpeed == gVar.Get_exploSpeed3():
	                    print 'Speed decreased'
	                    self.exploSpeed = gVar.Get_exploSpeed2();
	        elif args[0] == 'toggle':
	            print 'Driving mode'
	            self.__controlScheme = 1
	            self.__driveSpeedLeft = 0
	            self.__driveSpeedRight = 0
	            #ser.write('HB' + chr(2) + chr(0) + chr(2) + chr(0))
	    elif self.__controlScheme == 1:
	        self.__driveCommand = args[0]
	        if args[0] == 'stop':
	            print 'HALT'
	            #ser.write('x')
	        elif args[0] == 'toggle':
	            print 'Exploration mode'
	            self.__controlScheme = 0
	            self.__exploSpeed = gVar.Get_exploSpeed1();
	            #ser.write('HB' + chr(2) + chr(0) + chr(2) + chr(0))