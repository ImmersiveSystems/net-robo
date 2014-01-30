// ArduinoUno_Sabertooth_Controller.ino

#include "UnoConstants.h"
#include "UnoRobotEncoder.h"
#include "UnoRobotController.h"

#include <Servo.h>

int pos = 30;
static int flag = 0;
UnoRobotController robot;

void setup()
{
  robot.InitServos();
  robot.InitSaber();
  robot.InitEncoderPins();
}

void loop()
{
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
  
  robot.ProcessEncoders(); //inherited from UnoRobotEncoder class
  robot.SerialCommunicate();
  // robot.UpdateServosPos();
}
