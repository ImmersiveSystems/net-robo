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
		self.__ForwardThread = False		
		self.__BackwardThread = False
		self.__RightTurnThread = False
		self.__LeftTurnThread = False
		self.__SteerRightThread = False
		self.__SteerLeftThread = False

	def Get_SteerRightThread(self):
		return self.__SteerRightThread

	def Set_SteerRightThread(self, Value):
		self.__SteerRightThread = Value

	def Get_SteerLeftThread(self):
		return self.__SteerLeftThread

	def Set_SteerLeftThread(self, Value):
		self.__SteerLeftThread = Value
	def Get_BackwardThread(self):
		return self.__BackwardThread

	def Set_BackwardThread(self, Value):
		self.__BackwardThread = Value

	def Get_RightTurnThread(self):
		return self.__RightTurnThread

	def Set_RightTurnThread(self, Value):
		self.__RightTurnThread = Value

	def Get_LeftTurnThread(self):
		return self.__LeftTurnThread

	def Set_LeftTurnThread(self, Value):
		self.__LeftTurnThread = Value

	def Get_ForwardThread(self):
		return self.__ForwardThread

	def Set_ForwardThread(self, Value):
		self.__ForwardThread = Value

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

	def CheckThreadStatus(self):
	    if self.Get_ForwardThread() == True:
	    	self.Set_ForwardThread(False)
	    if self.Get_BackwardThread() == True:
	    	self.Set_BackwardThread(False)
	    if self.Get_RightTurnThread() == True:
	    	self.Set_RightTurnThread(False)
	    if self.Get_LeftTurnThread() == True:
	    	self.Set_LeftTurnThread(False)
	    if self.Get_SteerRightThread() == True:
	    	self.Set_SteerRightThread(False)
	    if self.Get_SteerLeftThread() == True:
	    	self.Set_SteerLeftThread(False)

	def RunActiveThreads(self, gVar, initVar):
	    if self.Get_ForwardThread() == True:
	    	threading.Timer(0.1, self.ToggledForward_accel, args = (gVar, )).start()
	    if self.Get_BackwardThread() == True:
	    	threading.Timer(0.1, self.ToggledBackward_accel, args = (gVar, )).start()
	    if self.Get_RightTurnThread() == True:
	    	threading.Timer(0.1, self.ToggledTurnRight, args = (gVar, initVar, )).start()
	    if self.Get_LeftTurnThread() == True:
	    	threading.Timer(0.1, self.ToggledTurnLeft, args = (gVar, initVar, )).start()
	    if self.Get_SteerRightThread() == True:
	    	threading.Timer(0.1, self.ToggledSteerRight, args = (gVar, initVar, )).start()
	    if self.Get_SteerLeftThread() == True:
	    	threading.Timer(0.1, self.ToggledSteerLeft, args = (gVar, initVar, )).start()

	def DrivingThread(self):	    
	    gVar = Consts_n_Flags.GlobalVariables()
	    initVar = Consts_n_Flags.InitializerClass()
	    if self.__controlScheme == initVar.Set2One():
	        self.ToggledResumeSpeed_AfterTurn(gVar, initVar)	                        	                	                
	        if self.__driveCommand == 'forward':
	            print 'Go FORWARD'
	            if self.Get_ForwardThread() == False:
	            	self.Set_ForwardThread(True)	            
	            # self.ToggledForward_accel(gVar)
	        elif (self.__driveCommand == '-forward') or (self.__driveCommand == '-right') or (self.__driveCommand == '-left') or (self.__driveCommand == '-backward'):	            
	            self.CheckThreadStatus()
	            print 'Wait'
	        elif self.__driveCommand == 'backward':	            
	            print 'Go BACKWARD'
	            if self.Get_BackwardThread() == False:
	            	self.Set_BackwardThread(True)
	            # self.ToggledBackward_accel(gVar)	            
	        elif self.__driveCommand == 'right':
	            print 'Turn RIGHT'
	            if self.Get_RightTurnThread() == False:
	            	self.Set_RightTurnThread(True)
	            # self.ToggledTurnRight(gVar, initVar)
	        elif self.__driveCommand == 'left':
	            print 'Turn LEFT'
	            if self.Get_LeftTurnThread() == False:
	            	self.Set_LeftTurnThread(True)
	            # self.ToggledTurnLeft(gVar, initVar)
	        elif self.__driveCommand == 'shiftr':
	            print 'Steer RIGHT'
	            if self.Get_SteerRightThread() == False:
	            	self.Set_SteerRightThread(True)
	            # self.ToggledSteerRight(gVar, initVar)
	        elif self.__driveCommand == 'shiftl':
	            print 'Steer LEFT'
	            if self.Get_SteerLeftThread() == False:
	            	self.Set_SteerLeftThread(True)
	            # self.ToggledSteerLeft(gVar, initVar)
	        self.ToggledDecelLeft_NoTurning(gVar, initVar)
	        self.ToggledDecelRight_NoTurning(gVar, initVar)
	        self.CheckDriveSpeed(gVar)
	    
	    threading.Timer(0.1, self.DrivingThread).start()	    
	    self.RunActiveThreads(gVar, initVar)

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