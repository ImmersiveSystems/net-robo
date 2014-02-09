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
public:
	UnoRobotEncoder();
	void InitEncoderPins();
	void SetLeftEncoder();
	void SetRightEncoder();	

	void MonitorLeftEncoder();
	void MonitorRightEncoder();
	void ShowEncoderValue();
};

#endif