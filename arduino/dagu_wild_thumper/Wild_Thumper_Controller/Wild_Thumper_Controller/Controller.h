#ifndef Controller_h
#define Controller_h

#include <Servo.h>
#include "IOpins.h"
#include "Constants.h"

class Controller
{
  private:
  	int servo[7];
	Servo Servo0;                                                 // define servos
	Servo Servo1;                                                 // define servos
	Servo Servo2;                                                 // define servos
	Servo Servo3;                                                 // define servos
	Servo Servo4;                                                 // define servos
	Servo Servo5;                                                 // define servos
	Servo Servo6;                                                 // define servos

	int LeftPWMStartFlag;
	int RightPWMStartFlag;

	unsigned int Volts;
	unsigned int LeftAmps;
	unsigned int RightAmps;
	unsigned long chargeTimer;
	unsigned long leftoverload;
	unsigned long rightoverload;

	//-----  used for RC mode -----
	int Leftspeed;
	int Rightspeed;
	int Speed;
	int Steer;
	//-----------------------------

	int highVolts;
	int startVolts;	
	static int Charged;                                               // 0=Flat battery  1=Charged battery
	
	int Leftmode;                                               // 0=reverse, 1=brake, 2=forward
	int Rightmode;                                              // 0=reverse, 1=brake, 2=forward

	int LeftPWM;                                                  // PWM value for left  motor speed / brake
	int RightPWM;                                                 // PWM value for right motor speed / brake
	int LeftPWM_Prev;
	int RightPWM_Prev;
	
	int data;
  public:
	Controller();

	void InitServos_IO_Pin();
	void InitServosDefaultPos();
	void SetServosPos();
	
	void SCmode();
	void I2Cmode();	
	void RCmode();

  	void SendPowerLevel();
        int ReturnChargedFlagStatus();
  	void MonitorBatteryVoltage();
  	void RechargeBattery();

  	void SelectOperationMode();
        void Serialread();
    
        void TurnOffMotors();
        void SetLeft_PWM();
        void SetRight_PWM();
    

        void MonitorRightPWM_HBridge();
        void RightMotor_MonitorCurrent();
        void ProcessRightMotorCommands();
        void RightMorotForward();
        void RightMotorBrake();
        void RightMotorBackward();
        void CheckRightPWM();
        void CheckRightPWM_Received();
        void AdjustRightPWM2Normal();

        void MonitorLeftPWM_HBridge();
        void LeftMotor_MonitorCurrent();
        void ProcessLeftMotorCommands();
        void LeftMorotForward();
        void LeftMotorBrake();
        void LeftMotorBackward();
        void CheckLeftPWM();
        void CheckLeftPWM_Received();
        void AdjustLeftPWM2Normal();
};

#endif
