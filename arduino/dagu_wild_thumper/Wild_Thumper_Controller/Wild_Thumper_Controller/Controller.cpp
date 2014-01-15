#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Servo.h>
#include "Controller.h"
#include "IOpins.h"
#include "Constants.h"

int Controller::Charged;

Controller::Controller()
{
  LeftPWMStartFlag = 0;
  RightPWMStartFlag = 0;
    
  Volts = 0;
  LeftAmps = 0;
  RightAmps = 0;
  chargeTimer = 0.0;
  leftoverload = 0.0;
  rightoverload = 0.0;
      
  highVolts = 0;
  startVolts = 0;	
  Charged = 1;                                               // 0=Flat battery  1=Charged battery
      	
  Leftmode = 1;                                               // 0=reverse, 1=brake, 2=forward
  Rightmode = 1;                                              // 0=reverse, 1=brake, 2=forward
      
  LeftPWM = 0;                                                  // PWM value for left  motor speed / brake
  RightPWM = 0;                                                 // PWM value for right motor speed / brake
  LeftPWM_Prev = 0;
  RightPWM_Prev = 0;
      
  Leftspeed = 0;
  Rightspeed = 0;
  Speed = 0;
  Steer = 0;
      	
  data = 0;	
}

int Controller::ReturnChargedFlagStatus()
{ 
  return Charged;
}
void Controller::InitServos_IO_Pin()
{
  Servo0.attach(S0);                                          // attach servo to I/O pin
  Servo1.attach(S1);                                          // attach servo to I/O pin
  Servo2.attach(S2);                                          // attach servo to I/O pin
  Servo3.attach(S3);                                          // attach servo to I/O pin
  Servo4.attach(S4);                                          // attach servo to I/O pin
  Servo5.attach(S5);                                          // attach servo to I/O pin
  Servo6.attach(S6);                                          // attach servo to I/O pin
}

void Controller::InitServosDefaultPos()
{
  Servo0.attach(S0);                                          // attach servo to I/O pin
  Servo1.attach(S1);                                          // attach servo to I/O pin
  Servo2.attach(S2);                                          // attach servo to I/O pin
  Servo3.attach(S3);                                          // attach servo to I/O pin
  Servo4.attach(S4);                                          // attach servo to I/O pin
  Servo5.attach(S5);                                          // attach servo to I/O pin
  Servo6.attach(S6);                                          // attach servo to I/O pin  
}

void Controller::Serialread() 
{//---------------------------------------------------------- Read serial port until data has been received -----------------------------------
  do{
      data = Serial.read();  
  }while (data < 0);
}

void Controller::ProcessRightMotorCommands()
{
  if ((millis() - rightoverload) > overloadtime)
  {
    switch (Rightmode)                                    // if right motor has not overloaded recently
    {
      case 2:                 // right motor forward
      RightMorotForward();
      break;

      case 1:                                               // right motor brake
      RightMotorBrake();
      break;

      case 0:                                               // right motor reverse
      RightMotorBackward();
      break;
    }
  } 
}


void Controller::ProcessLeftMotorCommands()
{
  if ((millis() - leftoverload) > overloadtime)             
  {
    switch (Leftmode)                                     // if left motor has not overloaded recently
    {
      case 2:        // left motor forward
      LeftMorotForward();
      break;

      case 1:                     // left motor brake
      LeftMotorBrake();
      break;

      case 0:                                               // left motor reverse
      LeftMotorBackward();
      break;
      
      default:
      break;
    }
  }  
}

void Controller::SetLeft_PWM()
{
  Serialread();
  Leftmode = data;
  Serialread();
  LeftPWM_Prev = LeftPWM;
  LeftPWM = data * SpeedScale;
  
  CheckLeftPWM_Received();
}

void Controller::SetRight_PWM()
{
  Serialread();
  Rightmode = data;
  Serialread();
  RightPWM_Prev = RightPWM;
  RightPWM = data * SpeedScale;  
  
  CheckRightPWM_Received();  
}

void Controller::CheckLeftPWM()
{
  if(LeftPWM == 0)
  {
    LeftMotorBrake();
  }  
  else if(LeftPWM < InitStablePWM)
  {
    LeftPWM = MaxInitStraightMove;
    LeftPWMStartFlag = 1;
  }
}

void Controller::CheckRightPWM()
{
  if(RightPWM == 0)
  {
    RightMotorBrake();
  }
  else if(RightPWM < InitStablePWM)
  {
    RightPWM = MaxInitStraightMove;
    RightPWMStartFlag = 1;
  }
}

void Controller::RightMotorBrake()
{
  if(RightPWM != 0)
  {
    RightPWM = 0;
  }
  RightPWM_Prev = 0;
  
  analogWrite(RmotorA,RightPWM);
  analogWrite(RmotorB,RightPWM);
}

void Controller::LeftMotorBrake()
{
  if(LeftPWM != 0)
  {
    LeftPWM = 0;
  }
  LeftPWM_Prev = 0;
  
  analogWrite(LmotorA,LeftPWM);
  analogWrite(LmotorB,LeftPWM);  
}

void Controller::RightMotorBackward()
{
  CheckRightPWM();
  if(LeftPWMStartFlag == 0)
  {
    analogWrite(RmotorA,RightPWM);
    analogWrite(RmotorB,0);
  }
  else
  {
    AdjustRightPWM2Normal();
    analogWrite(RmotorA,RightPWM);
    analogWrite(RmotorB,0);
  }  
}

void Controller::LeftMotorBackward()
{

  CheckLeftPWM();
  if(LeftPWMStartFlag == 0)
  {
    analogWrite(LmotorA,LeftPWM);
    analogWrite(LmotorB,0);
  }
  else
  {
    AdjustLeftPWM2Normal();
    analogWrite(LmotorA,LeftPWM);
    analogWrite(LmotorB,0);
  }  
}

void Controller::AdjustLeftPWM2Normal()
{
  if(LeftPWM > InitStablePWM)
  {
    LeftPWM = LeftPWM - InitMoveAdjustValue;
  }  
  if(LeftPWM <= InitStablePWM && LeftPWMStartFlag == 1)
  {
    LeftPWM_Prev = LeftPWM;
    LeftPWMStartFlag = 0;
  }    
}

void Controller::AdjustRightPWM2Normal()
{
  if(RightPWM > InitStablePWM)
  {
    RightPWM = RightPWM - InitMoveAdjustValue;
  }
  if(RightPWM <= InitStablePWM && RightPWMStartFlag == 1)
  {
    RightPWM_Prev = RightPWM;    
    RightPWMStartFlag = 0;
  }    
}

void Controller::LeftMorotForward()
{
  CheckLeftPWM();
  if(LeftPWMStartFlag == 0)
  {
    analogWrite(LmotorA,0);
    analogWrite(LmotorB,LeftPWM);
  }
  else
  {
    AdjustLeftPWM2Normal();
    analogWrite(LmotorA,0);
    analogWrite(LmotorB,LeftPWM);    
  }  
}

void Controller::RightMorotForward()
{
  CheckRightPWM();
  if(RightPWMStartFlag == 0)
  {
    analogWrite(RmotorA,0);
    analogWrite(RmotorB,RightPWM);
  }
  else
  {
    AdjustRightPWM2Normal();
    analogWrite(RmotorA,0);
    analogWrite(RmotorB,RightPWM);    
  }  
}

void Controller::CheckLeftPWM_Received()
{
  if(LeftPWM_Prev != 0 && LeftPWM < MinLimit)
  {
    LeftPWM = 0;
    LeftPWM_Prev = 0;
  }  
}

void Controller::CheckRightPWM_Received()
{
  if(RightPWM_Prev != 0 && RightPWM < MinLimit)
  {
    RightPWM = 0;
    RightPWM_Prev = 0;
  }
}

void Controller::MonitorRightPWM_HBridge()  // This code is incorporated into the controller by default
{
  if (RightPWM < 0)
  {
    analogWrite (RmotorA,(-1 * RightPWM));                                // turn off motors
    analogWrite (RmotorB,0);                                // turn off motors
  }
  else
  {
    analogWrite (RmotorA,0);                                // turn off motors
    analogWrite (RmotorB,RightPWM);                                // turn off motors
  }
}

void Controller::MonitorLeftPWM_HBridge()  // This code is incorporated into the controller by default
{
  if (LeftPWM < 0)
  {
    analogWrite (LmotorA,(-1 * LeftPWM)); 
    analogWrite (LmotorB,0);  
  }
  else
  {
    analogWrite (LmotorA,0);                                // turn off motors
    analogWrite (LmotorB,LeftPWM);                                // turn off motors
  }  
}

void Controller::SelectOperationMode()
{
   if(Cmode == 0)
   {             
     RCmode();     // RC mode via D0 and D1
   }
   else if(Cmode == 1)
   {         
     SCmode();    // Serial mode via D0(RX) and D1(TX)
   }
   else if(Cmode == 2)
   {                                             
     I2Cmode();   // I2C mode via A4(SDA) and A5(SCL)
   }  
}

void Controller::CheckVoltageLevel()
{
  if (Volts < lowvolt)
  {
    TurnOffMotors();                                          // temporarily used to carry the robot back to the charger  
  }  
}

void Controller::RechargeBattery()
{
  CheckVoltageLevel();
  
  if (Volts > highVolts)                                      // has battery voltage increased?
  {
    highVolts = Volts;                                        // record the highest voltage. Used to detect peak charging.
    chargeTimer = millis();                                   // when voltage increases record the time
  }

  if (Volts > batvolt)                                        // battery voltage must be higher than this before peak charging can occur.
  {
    Serial.print(    "Timer: ");
    Serial.println(millis() - chargeTimer);
    if ((highVolts - Volts) > 5 || (millis() - chargeTimer) > chargetimeout || Volts > maxvolt) // has voltage begun to drop or levelled out?
    {
      Charged = 1;                                            // battery voltage has peaked
      digitalWrite(Charger,0);                             // turn off current regulator
      //digitalWrite (Charger,1);                             // turn off current regulator
    }
  }   
}

void Controller::TurnOffMotors()
{
  analogWrite (LmotorA,0);                                // turn off motors
  analogWrite (LmotorB,0);                                // turn off motors
  analogWrite (RmotorA,0);                                // turn off motors
  analogWrite (RmotorB,0);                                // turn off motors  
}

void Controller::SendPowerLevel()
{
  double Power = (double)Volts;
  Power = Power / VoltageScale;
  Serial.write('P'); //start bit for a power command 
  Serial.print(Power);  
}

void Controller::MonitorBatteryVoltage()
{
  Volts = analogRead(Battery);                                  // read the battery voltage
  LeftAmps = analogRead(LmotorC);                               // read left motor current draw
  RightAmps = analogRead(RmotorC);                              // read right motor current draw
  SendPowerLevel();
  Serial.write('C'); //start bit for charge status
  Serial.print(Charged);  
  if ((Volts < lowvolt) && (Charged == 1))                       // check condition of the battery  
  {                                                           // change battery status from charged to flat
    //---------------------------------------------------------- FLAT BATTERY speed controller shuts down until battery is recharged ----
    //---------------------------------------------------------- This is a safety feature to prevent malfunction at low voltages!! ------
    Charged = 0;                                                // battery is flat
    highVolts = Volts;                                          // record the voltage
    startVolts = Volts;
    chargeTimer = millis();                                     // record the time
    digitalWrite(Charger, 0);                                 // enable current regulator to charge battery
  }  
}

void Controller::LeftMotor_MonitorCurrent()
{
  if (LeftAmps > Leftmaxamps)                                   // is motor current draw exceeding safe limit
  {
    analogWrite(LmotorA, 0);                                  // turn off motors
    analogWrite(LmotorB, 0);                                  // turn off motors
    leftoverload = millis();                                    // record time of overload
  }  
}

void Controller::RightMotor_MonitorCurrent()
{
  if(RightAmps > Rightmaxamps)                                 // is motor current draw exceeding safe limit
  {
    analogWrite(RmotorA, 0);                                  // turn off motors
    analogWrite(RmotorB, 0);                                  // turn off motors
    rightoverload = millis();                                   // record time of overload
  }
}

void Controller::SetServosPos()
{
  for (int i=0;i<15;i++)                               // read 14 bytes of data
  {
    Serialread();                                      
    servo[i] = data;
  }

  Servo0.writeMicroseconds(servo[0] * 256 + servo[1]);     // set servo position
  Servo1.writeMicroseconds(servo[2] * 256 + servo[3]);     // set servo position
  Servo2.writeMicroseconds(servo[4] * 256 + servo[5]);     // set servo position
  Servo3.writeMicroseconds(servo[6] * 256 + servo[7]);     // set servo position
  Servo4.writeMicroseconds(servo[8] * 256 + servo[9]);     // set servo position
  Servo5.writeMicroseconds(servo[10] * 256 + servo[11]);   // set servo position
  Servo6.writeMicroseconds(servo[12] * 256 + servo[13]);   // set servo position
}

void Controller::SCmode()
{// ------------------------------------------------------------ Code for Serial Communications --------------------------------------

  // FL = flush serial buffer

  // AN = report Analog inputs 1-5

  // SV = next 7 integers will be position information for servos 0-6

  // HB = "H" bridge data - next 4 bytes will be:
  //      left  motor mode 0-2
  //      left  motor PWM  0-255
  //      right motor mode 0-2
  //      right motor PWM  0-255
  
  if (Serial.available() > 1)                                   // command available
  {
    int A = Serial.read();
    int B = Serial.read();
    int command = A * 256 + B;

    switch(command)
    {
      case 17996:                                             // FL
        Serial.flush();                                       // flush buffer
        break;

      case 16718:                                             // AN - return values of analog inputs 1-5
        for (int i=1;i<6;i++)                                 // index analog inputs 1-5
        {
          data = analogRead(i);                                 // read 10bit analog input 
          Serial.write(highByte(data));                       // transmit high byte
          Serial.write(lowByte(data));                        // transmit low byte
        }
        break;

      case 21334:                                            // SV - receive postion information for servos 0-6
        SetServosPos();
        break;

      case 18498:                                            // HB - mode and PWM data for left and right motors
        SetLeft_PWM();
        SetRight_PWM();
        break;
         
      default:                                                // invalid command
       Serial.flush();                                       // flush buffer
    }
  }
}

void Controller::I2Cmode()
{//----------------------------------------------------------- Your code goes here ------------------------------------------------------------

}

void Controller::RCmode()
{
  //------------------------------------------------------------ Code for RC inputs ---------------------------------------------------------

  Speed = pulseIn(RCleft, HIGH, 25000);                           // read throttle/left stick
  Steer=pulseIn(RCright, HIGH, 25000);                          // read steering/right stick


  if (Speed == 0)
  {    
    Speed = 1500;                                   // if pulseIn times out (25mS) then set speed to stop
  } 

  if (Steer == 0) 
  {
    Steer = 1500;                                   // if pulseIn times out (25mS) then set steer to centre
  }

  if (abs(Speed - 1500) < RCdeadband)
  {
    Speed = 1500;                 // if Speed input is within deadband set to 1500 (1500uS=center position for most servos)
  }

  if (abs(Steer - 1500) < RCdeadband) 
  {
    Steer = 1500;                 // if Steer input is within deadband set to 1500 (1500uS=center position for most servos)
  }

  if (Mix == 1)                                                 // Mixes speed and steering signals
  {
    Steer = Steer - 1500;
    Leftspeed = Speed - Steer;
    Rightspeed = Speed + Steer;
  }
  else                                                        // Individual stick control
  {
    Leftspeed = Speed;
    Rightspeed = Steer;
  }
  
  Leftmode = 2;
  Rightmode = 2;

  if(Leftspeed > (Leftcenter + RCdeadband)) 
  {
    Leftmode = 0;          // if left input is forward then set left mode to forward
  }

  if(Rightspeed > (Rightcenter + RCdeadband)) 
  {
    Rightmode = 0;       // if right input is forward then set right mode to forward
  }

  LeftPWM = abs(Leftspeed - Leftcenter) * 10 / scale;                 // scale 1000-2000uS to 0-255
  LeftPWM=min(LeftPWM,255);                                   // set maximum limit 255

  RightPWM = abs(Rightspeed - Rightcenter) * 10 / scale;              // scale 1000-2000uS to 0-255
  RightPWM = min(RightPWM, 255);                                 // set maximum limit 255
}
