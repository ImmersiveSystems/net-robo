#include <Servo.h>
#include "IOpins.h"
#include "Constants.h"
#include "Controller.h"

Controller RobotController;

void setup()
{    
  RobotController.InitServos_IO_Pin();    //Initialize Servos Pins
  RobotController.InitServosDefaultPos(); //Set servos to default position
  //------------------------------------------------------------ Initialize I/O pins --------------------------------------------------

  pinMode (Charger,OUTPUT);                                   // change Charger pin to output
  digitalWrite (Charger,1);                                   // disable current regulator to charge battery

  if (Cmode == 1) 
  {
    Serial.begin(Brate);                                      // enable serial communications if Cmode=1
    Serial.flush();                                           // flush buffer
  } 
  //Serial.begin(57600);
}


void loop()
{
  //------------------------------------------------------------ Check battery voltage and current draw of motors ---------------------
  RobotController.LeftMotor_MonitorCurrent();
  RobotController.RightMotor_MonitorCurrent();
  RobotController.MonitorBatteryVoltage();
  //------------------------------------------------------------ CHARGE BATTERY -------------------------------------------------------
  if (RobotController.ReturnChargedFlagStatus() == 0) //&& (Volts-startVolts>65))  // if battery is flat and charger has been connected (voltage has increased by at least 1V)
  {
    RobotController.RechargeBattery();
  }
  else
  {//----------------------------------------------------------- GOOD BATTERY speed controller opperates normally ----------------------
    RobotController.SelectOperationMode();
    // --------------------------------------------------------- Code to drive dual "H" bridges --------------------------------------
    RobotController.MonitorLeftPWM_HBridge();
    RobotController.MonitorRightPWM_HBridge();

    if (RobotController.ReturnChargedFlagStatus() == 1)                                           // Only power motors if battery voltage is good
    {
      RobotController.ProcessLeftMotorCommands();
      RobotController.ProcessRightMotorCommands();
    }
    else                                                      // Battery is flat
    {
      RobotController.TurnOffMotors();
    }
  }
}

