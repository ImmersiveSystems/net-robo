#include <Servo.h>
#include "IOpins.h"
#include "Constants.h"
#include "Controller.h"

Controller RobotController;

void setup()
{    
  // Initialize Servos Pins
  RobotController.InitServos_IO_Pin();  
  // Set servos to default position  
  RobotController.InitServosDefaultPos(); 

  // change Charger pin to output
  pinMode (Charger,OUTPUT);      
  // disable current regulator to charge battery                             
  digitalWrite (Charger,1);                                   

  // enable serial communications
  Serial.begin(Brate);                                      
  Serial.flush();
}

void loop()
{
  // Check battery voltage and current draw of motors
  RobotController.LeftMotor_MonitorCurrent();
  RobotController.RightMotor_MonitorCurrent();
  
  RobotController.MonitorBatteryVoltage();
  RobotController.DebugInfo();
  RobotController.RechargeBattery();

  RobotController.SerialCommunicate();

  RobotController.CheckVoltageLevel();
  RobotController.ProcessLeftMotorCommands();
  RobotController.ProcessRightMotorCommands();
}