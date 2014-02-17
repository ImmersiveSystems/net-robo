// ArduinoUno_Sabertooth_Controller.ino

#include "UnoConstants.h"
#include "UnoRobotEncoder.h"
#include "UnoRobotController.h"

#include <Servo.h>

UnoRobotController robot;

void setup()
{
  robot.InitServos();
  robot.InitSaber();
  robot.InitEncoderPins();  
  
  attachInterrupt(IntteruptPin_LeftEncoder, InterruptEnableLeftEncoder, CHANGE); // pin 2
  attachInterrupt(InterruptPin_RightEncoder, InterruptEnableRightEncoder, CHANGE); // pin 3
}

void loop()
{   
  robot.SerialCommunicate();
  robot.SendVelocity_Encoders_Msg();  
  // robot.SendLeftEncoderValue();
  // robot.SendRightEncoderValue();

  // for(int pos = 0; pos < 180; pos++)
  // {
  // 	robot.TestServos(1);
	 //  delay(ServoDelayValue);
  // }
  // for(int pos = 180; pos >= 0; pos--)
  // {
  // 	robot.TestServos(-1);
	 //  delay(ServoDelayValue);
  // }    
}

void InterruptEnableLeftEncoder()
{
  robot.MonitorLeftEncoder();
}

void InterruptEnableRightEncoder()
{
  robot.MonitorRightEncoder();
}