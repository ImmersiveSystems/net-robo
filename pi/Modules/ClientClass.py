from socketIO_client import SocketIO
import threading
import serial
import sys

sys.path.append('./Modules')
import Consts_n_Flags

class Client():
	def __init__(self):
		initVar = Consts_n_Flags.InitializerClass()
		self.__voltage = initVar.Set2Zero()
		self.__charged = initVar.Set2One()
		self.__controlScheme = initVar.Set2Zero()
		self.__exploSpeed = self.Set_exploSpeed()
		self.__driveCommand  = ''
		self.__isTurning = initVar.Set2False()
		self.__driveSpeedLeft = initVar.Set2Zero()
		self.__driveSpeedRight = initVar.Set2Zero()
		self.__driveSavedSpeedLeft = initVar.Set2Zero()
		self.__driveSavedSpeedRight = initVar.Set2Zero()
		self.__driveModeLeft = initVar.Set2Zero()
		self.__driveModeRight = initVar.Set2Zero()

	def Set_exploSpeed(self):
		return Consts_n_Flags.GlobalVariables().Get_exploSpeed1()

	def Get_ControlScheme(self):
		return self.__controlScheme

	def ToggledForward_accel(self, gVar):
		self.__driveSpeedLeft = self.__driveSpeedLeft + gVar.Get_AccelRate()
		self.__driveSpeedRight = self.__driveSpeedRight + gVar.Get_AccelRate()

	def ToggledBackward_accel(self, gVar):
		self.__driveSpeedLeft = self.__driveSpeedLeft - gVar.Get_AccelRate()
		self.__driveSpeedRight = self.__driveSpeedRight - gVar.Get_AccelRate()

	def ToggledTurnRight(self, gVar, initVar):
	    if self.__isTurning == initVar.Set2False() or (self.__driveSpeedLeft > initVar.Set2Zero() and self.__driveSpeedRight < initVar.Set2Zero()):
	        self.__driveSavedSpeedLeft = self.__driveSpeedLeft		
	        self.__driveSavedSpeedRight = self.__driveSpeedRight
	        self.__driveSpeedRight = gVar.Get_DriveTurnSpeed()
	        self.__driveSpeedLeft = -1 * gVar.Get_DriveTurnSpeed()
	        self.__isTurning = initVar.Set2True()
	    else:
	    	self.__driveSpeedRight = self.__driveSpeedRight + gVar.Get_TurnRate()
	    	self.__driveSpeedLeft = self.__driveSpeedLeft - gVar.Get_TurnRate()

	def ToggledTurnLeft(self, gVar, initVar):
	    if self.__isTurning == initVar.Set2False() or (self.__driveSpeedLeft < initVar.Set2Zero() and self.__driveSpeedRight > initVar.Set2Zero()):
	        self.__driveSavedSpeedLeft = self.__driveSpeedLeft
	        self.__driveSavedSpeedRight = self.__driveSpeedRight
	        self.__driveSpeedLeft = gVar.Get_DriveTurnSpeed()
	        self.__driveSpeedRight = -1 * gVar.Get_DriveTurnSpeed()
	        self.__isTurning = initVar.Set2True()
	    else:
	    	self.__driveSpeedLeft = self.__driveSpeedLeft + gVar.Get_TurnRate()
	    	self.__driveSpeedRight = self.__driveSpeedRight - gVar.Get_TurnRate()

	def ToggledSteerRight(self, gVar, initVar):
	    if self.__driveSpeedRight > initVar.Set2Zero() and self.__driveSpeedLeft > initVar.Set2Zero():
	        if self.__driveSpeedLeft > self.__driveSpeedRight:
	        	self.__driveSpeedLeft = self.__driveSpeedLeft - gVar.Get_ShiftRate()
	       	else:
	       		self.__driveSpeedRight = self.__driveSpeedRight + gVar.Get_ShiftRate()
	    elif self.__driveSpeedRight < initVar.Set2Zero() and self.__driveSpeedLeft < initVar.Set2Zero():
	    	if self.__driveSpeedRight > self.__driveSpeedLeft:
	    		self.__driveSpeedLeft = self.__driveSpeedLeft + gVar.Get_ShiftRate()
	    	else:
	    		self.__driveSpeedRight = self.__driveSpeedRight - gVar.Get_ShiftRate()

	def ToggledSteerLeft(self, gVar, initVar):
	    if self.__driveSpeedLeft > initVar.Set2Zero() and self.__driveSpeedRight > initVar.Set2Zero():
	    	if self.__driveSpeedRight > self.__driveSpeedLeft:
	    		self.__driveSpeedRight = self.__driveSpeedRight - gVar.Get_ShiftRate()
	       	else:
	       		self.__driveSpeedLeft = self.__driveSpeedLeft + gVar.Get_ShiftRate()	       		
	    elif self.__driveSpeedLeft < initVar.Set2Zero() and self.__driveSpeedRight < initVar.Set2Zero():	    	
	    	if self.__driveSpeedLeft > self.__driveSpeedRight:	    		
	    		self.__driveSpeedRight = self.__driveSpeedRight + gVar.Get_ShiftRate()
	    	else:
	    		self.__driveSpeedLeft = self.__driveSpeedLeft - gVar.Get_ShiftRate()

	def ToggledDecelLeft_NoTurning(self, gVar, initVar):
	    if self.__driveSpeedLeft > initVar.Set2Zero():
	    	if self.__isTurning == initVar.Set2False():
	    		self.__driveSpeedLeft = self.__driveSpeedLeft - gVar.Get_DecelRate()
	       	self.__driveModeLeft = initVar.Set2Two()
	    elif self.__driveSpeedLeft < initVar.Set2Zero():
	    	if self.__isTurning == initVar.Set2False():
	    		self.__driveSpeedLeft = self.__driveSpeedLeft + gVar.Get_DecelRate()
	    	self.__driveModeLeft = initVar.Set2Zero()

	def ToggledDecelRight_NoTurning(self, gVar, initVar):
	    if self.__driveSpeedRight > initVar.Set2Zero():
	    	if self.__isTurning == initVar.Set2False():
	    		self.__driveSpeedRight = self.__driveSpeedRight - gVar.Get_DecelRate()
	       	self.__driveModeRight = initVar.Set2Two()
	    elif self.__driveSpeedRight < initVar.Set2Zero():
	    	if self.__isTurning == initVar.Set2False():
	    		self.__driveSpeedRight = self.__driveSpeedRight + gVar.Get_DecelRate()
	    	self.__driveModeRight  = initVar.Set2Zero();

	def ToggledResumeSpeed_AfterTurn(self, gVar, initVar):
	    if (self.__driveCommand != 'right') and (self.__driveCommand != 'left') and (self.__driveCommand != '-forward') and (self.__driveCommand != '-right') and (self.__driveCommand != '-left') and (self.__driveCommand != '-backward'):
	    	if self.__isTurning == initVar.Set2True():
	    		self.__driveSpeedLeft = self.__driveSavedSpeedLeft
	    		self.__driveSpeedRight = self.__driveSavedSpeedRight
	    		self.__isTurning = initVar.Set2False()

	def DrivingThread(self):
	    gVar = Consts_n_Flags.GlobalVariables()
	    initVar = Consts_n_Flags.InitializerClass()
	    if self.__controlScheme == initVar.Set2One():
	        self.ToggledResumeSpeed_AfterTurn(gVar, initVar)	                        	                	                
	        if self.__driveCommand == 'forward':
	            print 'Go FORWARD'
	            self.ToggledForward_accel(gVar)
	        elif (self.__driveCommand == '-forward') or (self.__driveCommand == '-right') or (self.__driveCommand == '-left') or (self.__driveCommand == '-backward'):
	            print 'Wait'
	        elif self.__driveCommand == 'backward':
	            print 'Go BACKWARD'
	            self.ToggledBackward_accel(gVar)	            
	        elif self.__driveCommand == 'right':
	            print 'Turn RIGHT'
	            self.ToggledTurnRight(gVar, initVar)
	        elif self.__driveCommand == 'left':
	            print 'Turn LEFT'
	            self.ToggledTurnLeft(gVar, initVar)
	        elif self.__driveCommand == 'shiftr':
	            print 'Steer RIGHT'
	            self.ToggledSteerRight(gVar, initVar)
	        elif self.__driveCommand == 'shiftl':
	            print 'Steer LEFT'
	            self.ToggledSteerLeft(gVar, initVar)
	        self.ToggledDecelLeft_NoTurning(gVar, initVar)
	        self.ToggledDecelRight_NoTurning(gVar, initVar)
	        self.CheckDriveSpeed(gVar)

	    threading.Timer(0.1, self.DrivingThread).start()

	def CheckDriveSpeed(self, gVar):
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
		print 'opened serial - Uncomment the function call'

	def BatteryStatusThread(self):
		print 'Uncomment serial_port_threading'
		# thread = threading.Thread(target = self.read_from_port, args = (self.__serial,))
		# thread.start()

	def ListenIO(self):
		socketIO = SocketIO('localhost', 3000)
		socketIO.on('serverToPython', self.listener)
		socketIO.emit('clientType', 'Python')
		socketIO.wait(seconds = 6000)

	def ExploreIncreaseSpeed(self, gVar, initVar):
	    if self.__controlScheme == initVar.Set2Zero():
	        if self.__exploSpeed == gVar.Get_exploSpeed2():
	        	print 'Speed increased'
	        	self.__exploSpeed =  gVar.Get_exploSpeed3();
	        elif self.__exploSpeed == gVar.Get_exploSpeed1():
	        	print 'Speed increased'
	        	self.__exploSpeed = gVar.Get_exploSpeed2();

	def ExploreDecreaseSpeed(self, gVar, initVar):
	    if self.__controlScheme == initVar.Set2Zero():
	        if self.__exploSpeed == gVar.Get_exploSpeed2():
	        	print 'Speed decreased'
	        	self.__exploSpeed = gVar.Get_exploSpeed1();
	        elif self.__exploSpeed == gVar.Get_exploSpeed3():
	        	print 'Speed decreased'
	        	self.__exploSpeed = gVar.Get_exploSpeed2();

	def Switch2DrivingMode(self, initVar):
		print 'Driving mode'
		self.__controlScheme = initVar.Set2One()
		self.__driveSpeedLeft = initVar.Set2Zero()
		self.__driveSpeedRight = initVar.Set2Zero()
		#ser.write('HB' + chr(2) + chr(0) + chr(2) + chr(0))

	def Switch2ExploreMode(self, gVar, initVar):
		print 'Exploration mode'
		self.__controlScheme = initVar.Set2Zero()		
		self.__exploSpeed = gVar.Get_exploSpeed1();
		#ser.write('HB' + chr(2) + chr(0) + chr(2) + chr(0))

	def listener(self, *args):
	    gVar = Consts_n_Flags.GlobalVariables()
	    initVar = Consts_n_Flags.InitializerClass()
	    if self.__controlScheme == initVar.Set2Zero():
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
	            self.ExploreIncreaseSpeed(gVar, initVar)                	                    	                    	                	                   	                   
	        elif args[0] == 'shiftl':
	            self.ExploreDecreaseSpeed(gVar, initVar)
	        elif args[0] == 'toggle':
	            self.Switch2DrivingMode(initVar)
	    elif self.__controlScheme == initVar.Set2One():
	        self.__driveCommand = args[0]
	        if args[0] == 'stop':
	            print 'HALT'
	            #ser.write('x')
	        elif args[0] == 'toggle':
	            self.Switch2ExploreMode(gVar, initVar)