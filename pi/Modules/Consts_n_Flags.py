class InitializerClass:
	def __init__(self):
		self.__GlobalZeroInitializer = 0
		self.__GlobalOneInitializer = 1
		self.__GlobalTwoInitializer = 2
		self.__GlobalTrueFlag = True
		self.__GlobalFlaseFlag = False

	def Set2Zero(self):
		return self.__GlobalZeroInitializer

	def Set2True(self):
		return self.__GlobalTrueFlag

	def Set2False(self):
		return self.__GlobalFlaseFlag

	def Set2One(self):
		return self.__GlobalOneInitializer

	def Set2Two(self):
		return self.__GlobalTwoInitializer

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
