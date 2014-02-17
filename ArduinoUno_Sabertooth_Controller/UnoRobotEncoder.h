#ifndef UnoRobotEncoder_h
#define UnoRobotEncoder_h

class UnoRobotEncoder
{
protected:	
	volatile int encoderPinA[2];
	volatile int encoderPinA_Set[2];
	volatile int encoderPinB[2];
	volatile int encoderPinB_Set[2];
	volatile long encoderPos[2];
	static long encoderPos_Prev[2];
	String EncoderValuesMsg;
public:
	UnoRobotEncoder();
	void InitEncoderPins();
	void SetLeftEncoder();
	void SetRightEncoder();	

	void Long2Str();

	void MonitorLeftEncoder();
	void MonitorRightEncoder();

	void UpdateEncoderValues();
};

#endif