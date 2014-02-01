from socketIO_client import SocketIO
import threading
import serial
import sys

sys.path.append('./Modules')
import Consts_n_Flags

gVar = Consts_n_Flags.GlobalVariables()
initVar = Consts_n_Flags.InitializerClass()

class Client():
	def __init__(self):
		self.__voltage = initVar.Set2Zero()
		self.__charged = initVar.Set2One()
		self.__controlScheme = initVar.Set2Zero()
		self.__exploSpeed = self.Set_exploSpeed()
		self.__driveCommand  = ''
		self.__HaltSignal = 'x'
		self.__isTurning = initVar.Set2False()
		self.__driveSpeedLeft = initVar.Set2Zero()
		self.__driveSpeedRight = initVar.Set2Zero()
		self.__driveSavedSpeedLeft = initVar.Set2Zero()
		self.__driveSavedSpeedRight = initVar.Set2Zero()
		self.__driveModeLeft = initVar.Set2Zero()
		self.__driveModeRight = initVar.Set2Zero()		
		self.__CommandsDic = dict([('forward', self.ForwardExplore), ('backward', self.BackwardExplore), ('left', self.TurnLeftExplore), ('right', self.TurnRightExplore), ('stop', self.SendHaltSignal), ('shiftr', self.ExploreIncreaseSpeed), ('shiftl', self.ExploreDecreaseSpeed), ('toggle', self.ProcessToggleCommand), ('-forward', self.StopExplore), ('-backward', self.StopExplore), ('-left', self.StopExplore), ('-right', self.StopExplore)])
		self.__DriveCommandDic = dict([('forward', self.ToggledForward_accel), ('backward', self.ToggledBackward_accel), ('left', self.ToggledTurnLeft), ('right', self.ToggledTurnRight), ('shiftl', self.ToggledSteerLeft), ('shiftr', self.ToggledSteerRight)])
		self.__ThreadsFlagDic = dict([('forward', True), ('backward', True), ('left', True), ('right', True), ('shiftl', True), ('shiftr', True), ('-forward', False), ('-backward', False), ('-left', False), ('-right', False)])
		self.__ConflictingCommandsDic = {'forward':'backward', 'backward':'forward', 'left':'right', 'right':'left', 'shiftl':'shiftr', 'shiftr':'shiftl'}
		self.__DriveCommandPairs = dict([('-forward', 'forward'), ('-backward', 'backward'), ('-left', 'left'), ('-right', 'right')])
		self.__ListActiveDriveCmd = []		
		self.__ShiftCommands = ['shiftl', 'shiftr']

		self.__ServosOnlyFlag = 1
		self.__PanValue = gVar.InitPan()
		self.__TiltValue = gVar.InitTilt()
		self.__ElbowValue = gVar.InitElbow()
		self.__ClawValue = gVar.InitClaw()
		self.__WristValue = gVar.InitWrist()


		# self.__ServoCommand = ''
		# self.__PanValue = gVar.Get_ServosStopSignal()
		# self.__TiltValue = gVar.Get_ServosStopSignal()
		# self.__ElbowValue = gVar.Get_ServosStopSignal()
		# self.__ClawValue = gVar.Get_ServosStopSignal()
		# self.__WristValue = gVar.Get_ServosStopSignal()
		# self.__ServosCommandDic = dict([('PanClk', self.IncrementServoValue(gVar.Get_PanPin())), ('PanCntrClk', self.DecrementServoValue(gVar.Get_PanPin())), ('TiltUp', self.IncrementServoValue(gVar.Get_TiltPin())), ('TiltDwn', self.DecrementServoValue(gVar.Get_TiltPin())), ('ElbowUp', self.IncrementServoValue(gVar.Get_ElbowPin())), ('ElbowDwn', self.DecrementServoValue(gVar.Get_ElbowPin())), ('ClawClk', self.IncrementServoValue(gVar.Get_ClawPin())), ('ClawCntrClk', self.DecrementServoValue(gVar.Get_ClawPin())), ('WristClk', self.IncrementServoValue(gVar.Get_WristPin())), ('WristCntrClk', self.DecrementServoValue(gVar.Get_WristPin()))])
		# self.__ServosStopCmdDic = dict([('PanClk', self.StopServo(gVar.Get_PanPin())), ('PanCntrClk', self.StopServo(gVar.Get_PanPin())), ('TiltUp', self.StopServo(gVar.Get_TiltPin())), ('TiltDwn', self.StopServo(gVar.Get_TiltPin())), ('ElbowUp', self.StopServo(gVar.Get_ElbowPin())), ('ElbowDwn', self.StopServo(gVar.Get_ElbowPin())), ('ClawClk', self.StopServo(gVar.Get_ClawPin())), ('ClawCntrClk', self.StopServo(gVar.Get_ClawPin())), ('WristClk', self.StopServo(gVar.Get_WristPin())), ('WristCntrClk', self.StopServo(gVar.Get_WristPin()))])
		# self.__ServosThreadsFlagDic = dict([('PanClk', True), ('PanCntrClk', True), ('TiltUp', True), ('TiltDwn', True), ('ElbowUp', True), ('ElbowDwn', True), ('ClawClk', True), ('ClawCntrClk', True), ('WristClk', True), ('WristCntrClk', True), ('-PanClk', False), ('-PanCntrClk', False), ('-TiltUp', False), ('-TiltDwn', False), ('-ElbowUp', False), ('-ElbowDwn', False), ('-ClawClk', False), ('-ClawCntrClk', False), ('-WristClk', False), ('-WristCntrClk', False)])
		# self.__ServosConflictingCommandsDic = {'PanClk':'PanCntrClk', 'PanCntrClk':'PanClk', 'TiltUp':'TiltDwn', 'TiltDwn':'TiltUp', 'ElbowUp':'ElbowDwn', 'ElbowDwn':'ElbowUp', 'ClawClk':'ClawCntrClk', 'ClawCntrClk':'ClawClk', 'WristClk':'WristCntrClk', 'WristCntrClk':'WristClk'}
		# self.__ServosCommandPairs = dict([('-PanClk', 'PanClk'), ('-PanCntrClk', 'PanCntrClk'), ('-TiltUp', 'TiltUp'), ('-TiltDwn', 'TiltDwn'), ('-ElbowUp', 'ElbowUp'), ('-ElbowDwn', 'ElbowDwn'), ('-ClawClk', 'ClawClk'), ('-ClawCntrClk', 'ClawCntrClk'), ('-WristClk', 'WristClk'), ('-WristCntrClk', 'WristCntrClk')])
		# self.__ServosListActiveCmd = []
		
	# def SendDriveModeSpeeds(self):
	# 	print 'we are here'
		# self.__serial.write('H' + chr(self.__driveModeLeft) + chr(abs(self.__driveSpeedLeft)) + chr(self.__driveModeRight) + chr(abs(self.__driveSpeedRight)))
		# self.__serial.write('H' + chr(self.__driveModeLeft) + chr(abs(self.__driveSpeedLeft)) + chr(self.__driveModeRight) + chr(abs(self.__driveSpeedRight))) + chr(self.__PanValue) + chr(self.__TiltValue) + chr(self.__ElbowValue) + chr(self.__ClawValue) + chr(self.__WristValue)

	def ShowDriveModeInfo(self):
		print 'Left Speed: ' + str(self.__driveSpeedLeft) + '   Right Speed: ' + str(self.__driveSpeedRight)
		print 'Current Active Thread(s): ------------------>  ', self.__ListActiveDriveCmd
		# self.__serial.write('H' + chr(self.__driveModeLeft) + chr(abs(self.__driveSpeedLeft)) + chr(self.__driveModeRight) + chr(abs(self.__driveSpeedRight)))
		# self.__serial.write('H' + chr(self.__driveModeLeft) + chr(abs(self.__driveSpeedLeft)) + chr(self.__driveModeRight) + chr(abs(self.__driveSpeedRight))) + chr(self.__PanValue) + chr(self.__TiltValue) + chr(self.__ElbowValue) + chr(self.__ClawValue) + chr(self.__WristValue)
	
	def Set_exploSpeed(self):
		return gVar.Get_exploSpeed1()

	def Get_ControlScheme(self):
		return self.__controlScheme

	def ToggledForward_accel(self):
		self.__driveSpeedLeft = self.__driveSpeedLeft + gVar.Get_AccelRate()
		self.__driveSpeedRight = self.__driveSpeedRight + gVar.Get_AccelRate()

	def ToggledBackward_accel(self):
		self.__driveSpeedLeft = self.__driveSpeedLeft - gVar.Get_AccelRate()
		self.__driveSpeedRight = self.__driveSpeedRight - gVar.Get_AccelRate()

	def SaveLeftRightSpeeds(self):
		self.__driveSavedSpeedLeft = self.__driveSpeedLeft
		self.__driveSavedSpeedRight = self.__driveSpeedRight

	def ToggledTurnRight(self):
		if self.__isTurning == initVar.Set2False() or (self.__driveSpeedLeft > initVar.Set2Zero() and self.__driveSpeedRight < initVar.Set2Zero()):
			self.SaveLeftRightSpeeds()
			self.__driveSpeedRight = gVar.Get_DriveTurnSpeed()
			self.__driveSpeedLeft = -1 * gVar.Get_DriveTurnSpeed()
			self.__isTurning = initVar.Set2True()
		else:
			self.__driveSpeedRight = self.__driveSpeedRight + gVar.Get_TurnRate()
			self.__driveSpeedLeft = self.__driveSpeedLeft - gVar.Get_TurnRate()

	def ToggledTurnLeft(self):
		if self.__isTurning == initVar.Set2False() or (self.__driveSpeedLeft < initVar.Set2Zero() and self.__driveSpeedRight > initVar.Set2Zero()):
			self.SaveLeftRightSpeeds()
			self.__driveSpeedLeft = gVar.Get_DriveTurnSpeed()
			self.__driveSpeedRight = -1 * gVar.Get_DriveTurnSpeed()
			self.__isTurning = initVar.Set2True()
		else:
			self.__driveSpeedLeft = self.__driveSpeedLeft + gVar.Get_TurnRate()
			self.__driveSpeedRight = self.__driveSpeedRight - gVar.Get_TurnRate()

	def ToggledSteerRight(self):
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

	def ToggledSteerLeft(self):
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

	def ToggledDecelLeft_NoTurning(self):
		if self.__driveSpeedLeft > initVar.Set2Zero():
			if self.__isTurning == initVar.Set2False():
				self.__driveSpeedLeft = self.__driveSpeedLeft - gVar.Get_DecelRate()
			self.__driveModeLeft = initVar.Set2Two()
		elif self.__driveSpeedLeft < initVar.Set2Zero():
			if self.__isTurning == initVar.Set2False():
				self.__driveSpeedLeft = self.__driveSpeedLeft + gVar.Get_DecelRate()
			self.__driveModeLeft = initVar.Set2Zero()

	def ToggledDecelRight_NoTurning(self):
		if self.__driveSpeedRight > initVar.Set2Zero():
			if self.__isTurning == initVar.Set2False():
				self.__driveSpeedRight = self.__driveSpeedRight - gVar.Get_DecelRate()
			self.__driveModeRight = initVar.Set2Two()
		elif self.__driveSpeedRight < initVar.Set2Zero():
			if self.__isTurning == initVar.Set2False():
				self.__driveSpeedRight = self.__driveSpeedRight + gVar.Get_DecelRate()
			self.__driveModeRight  = initVar.Set2Zero();

	def ToggledResumeSpeed_AfterTurn(self):
		if (self.__driveCommand != 'right') and (self.__driveCommand != 'left') and (self.__driveCommand != '-forward') and (self.__driveCommand != '-right') and (self.__driveCommand != '-left') and (self.__driveCommand != '-backward'):
			if self.__isTurning == initVar.Set2True():
				self.__driveSpeedLeft = self.__driveSavedSpeedLeft
				self.__driveSpeedRight = self.__driveSavedSpeedRight
				self.__isTurning = initVar.Set2False()

	def RunActiveThreads(self):			    
		if self.__ListActiveDriveCmd != []:
			for Cmd in self.__ListActiveDriveCmd:
				threading.Timer(0.1, self.__DriveCommandDic[Cmd]).start()
	
	def UpdateActiveCommandList(self):	    
		if self.__driveCommand != '' and self.__driveCommand in self.__ShiftCommands and self.__driveSpeedLeft == 0 and self.__driveSpeedRight == 0:
			pass
		elif self.__driveCommand != '' and self.__driveCommand in self.__ThreadsFlagDic and self.__ThreadsFlagDic[self.__driveCommand] == True:
			if self.__driveCommand not in self.__ListActiveDriveCmd:
				self.__ListActiveDriveCmd.append(self.__driveCommand)
				self.ResolveConflict()

	def CleanActiveCommandList(self):	    
		if self.__driveCommand != '' and self.__driveCommand in self.__ThreadsFlagDic and self.__ThreadsFlagDic[self.__driveCommand] == False:
			print self.__driveCommand
			if self.__ListActiveDriveCmd != [] and self.__DriveCommandPairs[self.__driveCommand] in self.__ListActiveDriveCmd:
				print 'REMOVED:       ', self.__DriveCommandPairs[self.__driveCommand]
				self.__ListActiveDriveCmd.remove(self.__DriveCommandPairs[self.__driveCommand])

	def MonitorShiftCommand(self):
		if len(self.__ListActiveDriveCmd) == 1:
			if self.__ListActiveDriveCmd[0] in self.__ShiftCommands:
				print 'Removing Ineffective Command', self.__ListActiveDriveCmd
				self.__ListActiveDriveCmd.remove(self.__ListActiveDriveCmd[0])
	
	def ResolveConflict(self):
		if self.__driveCommand in self.__ConflictingCommandsDic and self.__ConflictingCommandsDic[self.__driveCommand] in self.__ListActiveDriveCmd:
			print self.__driveCommand + "  and  " + self.__ConflictingCommandsDic[self.__driveCommand] + "  are Conflicting Commands"
			print 'Removing The Old Conflicting Command:   ', self.__ConflictingCommandsDic[self.__driveCommand]
			self.__ListActiveDriveCmd.remove(self.__ConflictingCommandsDic[self.__driveCommand])

	def DrivingThread(self):	    
		self.MonitorShiftCommand()
		if self.__controlScheme == initVar.Set2One():
			self.ToggledResumeSpeed_AfterTurn()
			self.UpdateActiveCommandList()	        
			self.CleanActiveCommandList()
			self.ToggledDecelLeft_NoTurning()
			self.ToggledDecelRight_NoTurning()
			self.CheckDriveSpeed_Left()
			self.CheckDriveSpeed_Right()
			self.SendDriveModeSpeeds()
			self.ShowDriveModeInfo()
				
		threading.Timer(0.1, self.DrivingThread).start()	    
		self.RunActiveThreads()
	
	def CheckDriveSpeed_Left(self):
		if self.__driveSpeedLeft > gVar.Get_MaxDriveSpeed():
			self.__driveSpeedLeft = gVar.Get_MaxDriveSpeed()
		elif self.__driveSpeedLeft < gVar.Get_MinDriveSpeed():
			self.__driveSpeedLeft = gVar.Get_MinDriveSpeed()

	def CheckDriveSpeed_Right(self):
		if self.__driveSpeedRight > gVar.Get_MaxDriveSpeed():
			self.__driveSpeedRight = gVar.Get_MaxDriveSpeed()
		elif self.__driveSpeedRight < gVar.Get_MinDriveSpeed():
			self.__driveSpeedRight = gVar.Get_MinDriveSpeed()

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
		print 'opened serial - Uncomment the function call'		
		# self.__serial = serial.Serial("/dev/ttyACM1", 9600)
		# self.__serial = serial.Serial("COM4", 9600)

	def BatteryStatusThread(self):
		print 'Uncomment serial_port_threading'
		# thread = threading.Thread(target = self.read_from_port, args = (self.__serial,))
		# thread.start()

	def ListenIO(self):
		# socketIO = SocketIO('192.168.1.33', 3000)		
		# socketIO = SocketIO('192.168.1.223', 3000)		
		socketIO = SocketIO('localhost', 3000)		
		socketIO.on('serverToPython', self.listener)
		socketIO.emit('clientType', 'Python')
		socketIO.wait(seconds = 6000)

	def ExploreIncreaseSpeed(self):
		if self.__controlScheme == initVar.Set2Zero():
			# if self.__exploSpeed == gVar.Get_exploSpeed2():
			# 	print 'Speed increased'
			# 	self.__exploSpeed =  gVar.Get_exploSpeed3();
			# elif self.__exploSpeed == gVar.Get_exploSpeed1():
			# 	print 'Speed increased'
			# 	self.__exploSpeed = gVar.Get_exploSpeed2();
			if self.__exploSpeed >= gVar.Get_exploMaxSpeed():
				self.__exploSpeed = gVar.Get_exploMaxSpeed()
				print 'Max speed reached'
			else:
				self.__exploSpeed = self.__exploSpeed + gVar.Get_exploSpeedRate()
				print 'Speed increased to ' + str(self.__exploSpeed)

	def ExploreDecreaseSpeed(self):
		if self.__controlScheme == initVar.Set2Zero():
			# if self.__exploSpeed == gVar.Get_exploSpeed2():
			# 	print 'Speed decreased'
			# 	self.__exploSpeed = gVar.Get_exploSpeed1();
			# elif self.__exploSpeed == gVar.Get_exploSpeed3():
			# 	print 'Speed decreased'
			# 	self.__exploSpeed = gVar.Get_exploSpeed2();
			if self.__exploSpeed <= gVar.Get_exploMinSpeed():
				self.__exploSpeed = gVar.Get_exploMinSpeed()
				print 'Min speed reached'
			else:
				self.__exploSpeed = self.__exploSpeed - gVar.Get_exploSpeedRate()
				print 'Speed decreased to ' + str(self.__exploSpeed)

	def Switch2DrivingMode(self):
		print 'Driving mode'
		self.__controlScheme = initVar.Set2One()
		self.__driveSpeedLeft = initVar.Set2Zero()
		self.__driveSpeedRight = initVar.Set2Zero()
		# self.__serial.write('H' + chr(2) + chr(0) + chr(2) + chr(0))
		# self.__serial.write('H' + chr(2) + chr(0) + chr(2) + chr(0)) + chr(self.__PanValue) + chr(self.__TiltValue) + chr(self.__ElbowValue) + chr(self.__ClawValue) + chr(self.__WristValue)        

	def Switch2ExploreMode(self):
		print 'Exploration mode'
		self.__controlScheme = initVar.Set2Zero()		
		self.__exploSpeed = gVar.Get_exploSpeed1();
		# self.__serial.write('H' + chr(2) + chr(0) + chr(2)
		# self.__serial.write('H' + chr(2) + chr(0) + chr(2) + chr(0)) + chr(self.__PanValue) + chr(self.__TiltValue) + chr(self.__ElbowValue) + chr(self.__ClawValue) + chr(self.__WristValue)        

	def ForwardExplore(self):
		print 'Move FORWARD'
		self.__ServosOnlyFlag = 0
		# self.__serial.write('H' + chr(2) + chr(self.__exploSpeed) + chr(2) + chr(self.__exploSpeed))
		# self.__serial.write('H' + chr(2) + chr(self.__exploSpeed) + chr(2) + chr(self.__exploSpeed)) + chr(self.__PanValue) + chr(self.__TiltValue) + chr(self.__ElbowValue) + chr(self.__ClawValue) + chr(self.__WristValue)        

	def BackwardExplore(self):
		print 'Move BACKWARD'
		self.__ServosOnlyFlag = 0
		# self.__serial.write('H' + chr(0) + chr(self.__exploSpeed) + chr(0) + chr(self.__exploSpeed))
		# self.__serial.write('H' + chr(0) + chr(self.__exploSpeed) + chr(0) + chr(self.__exploSpeed)) + chr(self.__PanValue) + chr(self.__TiltValue) + chr(self.__ElbowValue) + chr(self.__ClawValue) + chr(self.__WristValue)        
	
	def TurnLeftExplore(self):
		print 'Turn LEFT'
		self.__ServosOnlyFlag = 0
		# self.__serial.write('H' + chr(2) + chr(self.__exploSpeed) + chr(0) + chr(self.__exploSpeed))
		# self.__serial.write('H' + chr(2) + chr(self.__exploSpeed) + chr(0) + chr(self.__exploSpeed)) + chr(self.__PanValue) + chr(self.__TiltValue) + chr(self.__ElbowValue) + chr(self.__ClawValue) + chr(self.__WristValue)        

	def TurnRightExplore(self):
		print 'Turn RIGHT'
		self.__ServosOnlyFlag = 0
		# self.__serial.write('H' + chr(0) + chr(self.__exploSpeed) + chr(2) + chr(self.__exploSpeed))
		# self.__serial.write('H' + chr(0) + chr(self.__exploSpeed) + chr(2) + chr(self.__exploSpeed)) + chr(self.__PanValue) + chr(self.__TiltValue) + chr(self.__ElbowValue) + chr(self.__ClawValue) + chr(self.__WristValue)        

	def StopExplore(self):
		print 'Stop movement'
		self.__ServosOnlyFlag = 1
		self.__exploSpeed = self.Set_exploSpeed()
		# self.__serial.write('H' + chr(2) + chr(0) + chr(2) + chr(0))
		# self.__serial.write('H' + chr(2) + chr(0) + chr(2) + chr(0)) + chr(self.__PanValue) + chr(self.__TiltValue) + chr(self.__ElbowValue) + chr(self.__ClawValue) + chr(self.__WristValue)        

	def SendHaltSignal(self):
		print 'HALT'
		self.__ServosOnlyFlag = 1
		# self.__serial.write(self.__HaltSignal)
		# self.__serial.write(self.__HaltSignal) + chr(self.__PanValue) + chr(self.__TiltValue) + chr(self.__ElbowValue) + chr(self.__ClawValue) + chr(self.__WristValue)        

	def ProcessToggleCommand(self):
		print 'Toggling is disabled for now'
		# if self.__controlScheme == initVar.Set2One():
		# 	threading.Timer(0.1, self.Switch2ExploreMode).start()
		# else:
		# 	threading.Timer(0.1, self.Switch2DrivingMode).start()

	# def StopServo(self, PinNum):
	# 	if PinNum == gVar.Get_PanPin():
	# 		self.__PanValue = gVar.Get_ServosStopSignal()
	# 	elif PinNum == gVar.Get_TiltPin():
	# 		self.__PanValue = gVar.Get_ServosStopSignal()
	# 	elif PinNum == gVar.Get_ElbowPin():
	# 		self.__ElbowValue = gVar.Get_ServosStopSignal()
	# 	elif PinNum == gVar.Get_ClawPin():
	# 		self.__ClawValue = gVar.Get_ServosStopSignal()
	# 	elif PinNum == gVar.Get_WristPin():
	# 		self.__WristValue = gVar.Get_ServosStopSignal()

	# def IncrementServoValue(self, PinNum):
	# 	if PinNum == gVar.Get_PanPin():
	# 		self.__PanValue = gVar.Get_ServosIncrementRate()
	# 	elif PinNum == gVar.Get_TiltPin():
	# 		self.__PanValue = gVar.Get_ServosDecrementRate()
	# 	elif PinNum == gVar.Get_ElbowPin():
	# 		self.__ElbowValue = gVar.Get_ServosIncrementRate()
	# 	elif PinNum == gVar.Get_ClawPin():
	# 		self.__ClawValue = gVar.Get_ServosIncrementRate()
	# 	elif PinNum == gVar.Get_WristPin():
	# 		self.__WristValue = gVar.Get_ServosIncrementRate

	# def DecrementServoValue(self, PinNum):
	# 	if PinNum == gVar.Get_PanPin():
	# 		self.__PanValue = gVar.Get_ServosDecrementRate()
	# 	elif PinNum == gVar.Get_TiltPin():
	# 		self.__PanValue = gVar.Get_ServosDecrementRate()
	# 	elif PinNum == gVar.Get_ElbowPin():
	# 		self.__ElbowValue = gVar.Get_ServosDecrementRate()
	# 	elif PinNum == gVar.Get_ClawPin():
	# 		self.__ClawValue = gVar.Get_ServosDecrementRate()
	# 	elif PinNum == gVar.Get_WristPin():
	# 		self.__WristValue = gVar.Get_ServosDecrementRate()

	# def RunServosActiveThreads(self):		    
	# 	if self.__ServosListActiveCmd != []:
	# 		for Cmd in self.__ServosListActiveCmd:
	# 			threading.Timer(0.1, self.__ServosCommandDic[Cmd]).start()

	# def UpdateServosActiveCommandList(self):	    
	# 	if self.__ServoCommand != '' and self.__ServoCommand in self.__ServosThreadsFlagDic and self.__ServosThreadsFlagDic[self.__ServoCommand] == True:
	# 		if self.__ServoCommand not in self.__ServosListActiveCmd:
	# 			self.__ServosListActiveCmd.append(self.__ServoCommand)
	# 			self.ResolveServosCommandsConflict()

	# def CleanServosActiveCommandList(self):	    
	# 	if self.__ServoCommand != '' and self.__ServoCommand in self.__ServosThreadsFlagDic and self.__ServosThreadsFlagDic[self.__ServoCommand] == False:
	# 		print self.__ServoCommand
	# 		if self.__ServosListActiveCmd != [] and self.__ServosCommandPairs[self.__ServoCommand] in self.__ServosListActiveCmd:
	# 			print 'REMOVED:       ', self.__ServosCommandPairs[self.__ServoCommand]
	# 			self.__ServosListActiveCmd.remove(self.__ServosCommandPairs[self.__ServoCommand])
	# 			self.__ServosStopCmdDic[self.__ServosCommandPairs[self.__ServoCommand]]

	# def ResolveServosCommandsConflict(self):
	# 	if self.__ServoCommand in self.__ServosConflictingCommandsDic and self.__ServosConflictingCommandsDic[self.__ServoCommand] in self.__ServosListActiveCmd:
	# 		print self.__ServoCommand + "  and  " + self.__ServosConflictingCommandsDic[self.__ServoCommand] + "  are Conflicting Commands"
	# 		print 'Removing The Old Conflicting Command:   ', self.__ServosConflictingCommandsDic[self.__ServoCommand]
	# 		self.__ServosListActiveCmd.remove(self.__ServosConflictingCommandsDic[self.__ServoCommand])
	# 		self.__ServosStopCmdDic[self.__ServosConflictingCommandsDic[self.__ServoCommand]]
	
	# def ServosThread(self):	    
	# 	# print 'Inside ServosThread Function'
	# 	self.UpdateActiveCommandList()	        
	# 	self.CleanActiveCommandList()
	# 	if self.__ServosOnlyFlag == 1 and self.__ServosListActiveCmd != []:
	# 	   print 'Only Servos Running'
	# 	   self.__serial.write('H' + chr(2) + chr(0) + chr(2) + chr(0)) + chr(self.__PanValue) + chr(self.__TiltValue) + chr(self.__ElbowValue) + chr(self.__ClawValue) + chr(self.__WristValue)        
	# 	threading.Timer(0.1, self.ServosThread).start()	    
	# 	self.RunServosActiveThreads()

	def MonitorServosCommand(self, *args):
		if args:
			Cmd = args[:2]
			if Cmd.find("PAN"):
				self.__PanValue = int(args[3:])
			elif Cmd.find("TIL"):
				self.__TiltValue = int(args[3:])
			elif Cmd.find("ELW"):
				self.__ElbowValue = int(args[3:])
			elif Cmd.find("WRT"):
				self.__WristValue = int(args[3:])
			elif Cmd.find("CLW"):
				self.__ClawValue = int(args[3:])

			if self.__ServosOnlyFlag == 1:
				self.__serial.write('H' + chr(2) + chr(0) + chr(2) + chr(0)) + chr(self.__PanValue) + chr(self.__TiltValue) + chr(self.__ElbowValue) + chr(self.__ClawValue) + chr(self.__WristValue)		

	def listener(self, *args):
		if self.__controlScheme == initVar.Set2One():
			self.__driveCommand = args[0]
		if args[0] in self.__CommandsDic:
			threading.Timer(0.1, self.__CommandsDic[args[0]]).start()
		# if args[0] in self.__ServosCommandDic:
		# 	self.__ServoCommand = args[0]
		elif len(args) > 3:
			self.MonitorServosCommand(args)