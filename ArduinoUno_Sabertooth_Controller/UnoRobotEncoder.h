#ifndef UnoRobotEncoder_h
#define UnoRobotEncoder_h

// #include <Servo.h> 

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
};

/*
class PanTilt
{
private:
  	const int PanPin 9
	const int TiltPin 10
	const int PanInitPos 90
	const int TiltInitPos 125
public:
	Servo servoPan;  // create servo object to control a servo, a maximum of eight servo objects
	Servo servoTilt;

    PanTilt(int pp, int tp, int ppos, int tpos) : PanPin(pp), TiltPin(tp), PanInitPos(ppos), TiltInitPos(tpos) {}
    void InitServos();
    inline void Show()
    {
    	cout<<<<"\t"<<endl;
    }
};
*/
#endif