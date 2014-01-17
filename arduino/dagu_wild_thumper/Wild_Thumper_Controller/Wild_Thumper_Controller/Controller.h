#ifndef Controller_h
#define Controller_h

#include <Servo.h>
#include "IOpins.h"
#include "Constants.h"

class Controller
{
  private:
  	// define servos
  	int servo[7];
		Servo Servo0;	
		Servo Servo1;
		Servo Servo2;
		Servo Servo3;
		Servo Servo4;
		Servo Servo5;
		Servo Servo6;

		int data;
		int LeftPWMStartFlag;
		int RightPWMStartFlag;

		unsigned int Volts;
		unsigned int LeftAmps;
		unsigned int RightAmps;
		unsigned long chargeTimer;
		unsigned long leftoverload;
		unsigned long rightoverload;
		double ConvertedVolts;

		// 0=Flat battery  1=Charged battery
		static int Charged;
		int highVolts;
		int startVolts;	

		// 0=reverse, 1=brake, 2=forward
		int Leftmode;                                               
		int Rightmode;

		// PWM values for left and right motor speeds / brake
		int LeftPWM;                                                  
		int RightPWM;
		int LeftPWM_Prev;
		int RightPWM_Prev;

		unsigned long prevTime;
		
  public:
		Controller();

		void InitServos_IO_Pin();
		void InitServosDefaultPos();
		void SetServosPos();

		int ReturnChargedFlagStatus();
  	void SendPowerLevel();
  	void DebugInfo();
  	void MonitorBatteryVoltage();
  	void RechargeBattery();

  	void SerialCommunicate();
    void Serialread();

    void SetLeft_PWM();
    void SetRight_PWM();

    void MonitorRightPWM_HBridge();
    void RightMotor_MonitorCurrent();
    void ProcessRightMotorCommands();
    void RightMotorForward();
    void RightMotorBrake();
    void RightMotorBackward();
    void CheckRightPWM();
    void CheckRightPWM_Received();
    void AdjustRightPWM2Normal();

    void MonitorLeftPWM_HBridge();
    void LeftMotor_MonitorCurrent();
    void ProcessLeftMotorCommands();
    void LeftMotorForward();
    void LeftMotorBrake();
    void LeftMotorBackward();
    void CheckVoltageLevel();
    void CheckLeftPWM();
    void CheckLeftPWM_Received();
    void AdjustLeftPWM2Normal();
};

#endif
