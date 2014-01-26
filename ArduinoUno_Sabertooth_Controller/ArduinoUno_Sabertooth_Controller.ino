#include "UnoConstants.h"
#include "UnoRobotEncoder.h"
#include "UnoRobotController.h"

UnoRobotController robot;

void setup()
{
  robot.InitSaber();
  robot.InitEncoderPins();
}

void loop()
{
  robot.SerialCommunicate();
  robot.ProcessEncoders(); //inherited from UnoRobotEncoder class
}