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

	def ShowDriveModeInfo(self):
		print 'Left Speed: ' + str(self.__driveSpeedLeft) + '   Right Speed: ' + str(self.__driveSpeedRight)
		print 'Current Active Thread(s): ------------------>  ', self.__ListActiveDriveCmd
		self.__serial.write('H' + chr(self.__driveModeLeft) + chr(abs(self.__driveSpeedLeft)) + chr(self.__driveModeRight) + chr(abs(self.__driveSpeedRight)))        
	
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
		self.__serial = serial.Serial("/dev/ttyUSB0", 115200)
		print 'opened serial - Uncomment the function call'

	def BatteryStatusThread(self):
		print 'Uncomment serial_port_threading'
		# thread = threading.Thread(target = self.read_from_port, args = (self.__serial,))
		# thread.start()

	def ListenIO(self):
		socketIO = SocketIO('192.168.0.100', 3000)
		socketIO.on('serverToPython', self.listener)
		socketIO.emit('clientType', 'Python')
		socketIO.wait(seconds = 6000)

	def ExploreIncreaseSpeed(self):
		if self.__controlScheme == initVar.Set2Zero():
			if self.__exploSpeed == gVar.Get_exploSpeed2():
				print 'Speed increased'
				self.__exploSpeed =  gVar.Get_exploSpeed3();
			elif self.__exploSpeed == gVar.Get_exploSpeed1():
				print 'Speed increased'
				self.__exploSpeed = gVar.Get_exploSpeed2();

	def ExploreDecreaseSpeed(self):
		if self.__controlScheme == initVar.Set2Zero():
			if self.__exploSpeed == gVar.Get_exploSpeed2():
				print 'Speed decreased'
				self.__exploSpeed = gVar.Get_exploSpeed1();
			elif self.__exploSpeed == gVar.Get_exploSpeed3():
				print 'Speed decreased'
				self.__exploSpeed = gVar.Get_exploSpeed2();

	def Switch2DrivingMode(self):
		print 'Driving mode'
		self.__controlScheme = initVar.Set2One()
		self.__driveSpeedLeft = initVar.Set2Zero()
		self.__driveSpeedRight = initVar.Set2Zero()
		self.__serial.write('H' + chr(2) + chr(0) + chr(2) + chr(0))

	def Switch2ExploreMode(self):
		print 'Exploration mode'
		self.__controlScheme = initVar.Set2Zero()		
		self.__exploSpeed = gVar.Get_exploSpeed1();
		self.__serial.write('H' + chr(2) + chr(0) + chr(2) + chr(0))

	def ForwardExplore(self):
		print 'Move FORWARD'
		self.__serial.write('H' + chr(2) + chr(self.__exploSpeed) + chr(2) + chr(self.__exploSpeed))

	def BackwardExplore(self):
		print 'Move BACKWARD'
		self.__serial.write('H' + chr(0) + chr(self.__exploSpeed) + chr(0) + chr(self.__exploSpeed))
	
	def TurnLeftExplore(self):
		print 'Turn LEFT'
		self.__serial.write('H' + chr(2) + chr(self.__exploSpeed) + chr(0) + chr(self.__exploSpeed))

	def TurnRightExplore(self):
		print 'Turn RIGHT'
		self.__serial.write('H' + chr(0) + chr(self.__exploSpeed) + chr(2) + chr(self.__exploSpeed))

	def StopExplore(self):
		print 'Stop movement'
		#self.__exploSpeed = self.Set_exploSpeed()
		self.__serial.write('H' + chr(2) + chr(0) + chr(2) + chr(0))

	def SendHaltSignal(self):
		print 'HALT'
		self.__serial.write(self.__HaltSignal)

	def ProcessToggleCommand(self):
		if self.__controlScheme == initVar.Set2One():
			threading.Timer(0.1, self.Switch2ExploreMode).start()
		else:
			threading.Timer(0.1, self.Switch2DrivingMode).start()

	def listener(self, *args):
		if self.__controlScheme == initVar.Set2One():
			self.__driveCommand = args[0]
		if args[0] in self.__CommandsDic:
			threading.Timer(0.1, self.__CommandsDic[args[0]]).start()