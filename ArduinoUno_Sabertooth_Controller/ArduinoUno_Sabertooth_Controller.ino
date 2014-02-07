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
  
  attachInterrupt(0, InterruptEnableLeftEncoder, CHANGE); // pin 2
  attachInterrupt(1, InterruptEnableRightEncoder, CHANGE); // pin 3
}

void loop()
{
  robot.ShowEncoderValue();

  for(int pos = 0; pos < 180; pos++)
  {
  	robot.TestServos(1);
	  robot.UpdateServosPos();
	  delay(ServoDelayValue);
  }
  for(int pos = 180; pos >= 0; pos--)
  {
  	robot.TestServos(-1);
  	robot.UpdateServosPos();
	  delay(ServoDelayValue);
  }
}

void InterruptEnableLeftEncoder()
{
  // robot.ReadEncoder(0);
  robot.encoderPinChange_Left();
}

void InterruptEnableRightEncoder()
{
  // robot.ReadEncoder(1);
  robot.encoderPinChange_Right();
}