#ifndef UnoRobotEncoder_h
#define UnoRobotEncoder_h

// #include <Servo.h> 

class UnoRobotEncoder
{
protected:	
	volatile int encoderPinA[2];
	volatile int encoderPinA_Set[2];
	volatile int encoderPinB[2];
	volatile int encoderPinB_Set[2];
	volatile int encoderPos[2];
	// volatile int encoderPinA_Cur[2];
	// volatile int encoderPinA_Last[2];
public:
	UnoRobotEncoder();
	void InitEncoderPins();
	void EncoderPinChange_Left();
	void EncoderPinChange_Right();
	// void ReadEncoder(int Mode);
	void ShowEncoderValue(int Mode);
};

#endif