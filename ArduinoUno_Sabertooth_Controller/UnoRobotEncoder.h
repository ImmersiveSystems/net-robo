#ifndef UnoRobotEncoder_h
#define UnoRobotEncoder_h

class UnoRobotEncoder
{
protected:	
	int encoderPinA[2];
	int encoderPinB[2];
	int encoderPos[2];
	int encoderPinA_Cur[2];
	int encoderPinA_Last[2];
public:
	UnoRobotEncoder();
	void InitEncoderPins();
	void ReadEncoder(int Mode);
	void WhichEncoder(int Mode);
};

#endif