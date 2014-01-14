#include <Servo.h>
#include "IOpins.h"
#include "Constants.h"


//-------------------------------------------------------------- define global variables --------------------------------------------

unsigned int Volts;
unsigned int VoltsAtStop = 0;
unsigned int LeftAmps;
unsigned int RightAmps;
unsigned long chargeTimer;
unsigned long leftoverload;
unsigned long rightoverload;
int StartFlag = 0;
int StartTurnFlag = 0;

int AmpsAdjust = 0;
int highVolts;
int startVolts;
int Leftspeed=0;
int Rightspeed=0;
int Speed;
int Steer;
int CmdReceived;
int CmdPrev;
byte Charged=1;                                               // 0=Flat battery  1=Charged battery
int Leftmode=1;                                               // 0=reverse, 1=brake, 2=forward
int Rightmode=1;                                              // 0=reverse, 1=brake, 2=forward
byte Leftmodechange=0;                                        // Left input must be 1500 before brake or reverse can occur
byte Rightmodechange=0;                                       // Right input must be 1500 before brake or reverse can occur
double LeftPWM;                                                  // PWM value for left  motor speed / brake
double RightPWM;                                                 // PWM value for right motor speed / brake
double LeftPWM_Prev;
double RightPWM_Prev;
int data;
int servo[7];

//-------------------------------------------------------------- define servos ------------------------------------------------------


Servo Servo0;                                                 // define servos
Servo Servo1;                                                 // define servos
Servo Servo2;                                                 // define servos
Servo Servo3;                                                 // define servos
Servo Servo4;                                                 // define servos
Servo Servo5;                                                 // define servos
Servo Servo6;                                                 // define servos

void setup()
{  
  //------------------------------------------------------------ Initialize Servos ----------------------------------------------------

  Servo0.attach(S0);                                          // attach servo to I/O pin
  Servo1.attach(S1);                                          // attach servo to I/O pin
  Servo2.attach(S2);                                          // attach servo to I/O pin
  Servo3.attach(S3);                                          // attach servo to I/O pin
  Servo4.attach(S4);                                          // attach servo to I/O pin
  Servo5.attach(S5);                                          // attach servo to I/O pin
  Servo6.attach(S6);                                          // attach servo to I/O pin

  //------------------------------------------------------------ Set servos to default position ---------------------------------------

  Servo0.writeMicroseconds(DServo0);                          // set servo to default position
  Servo1.writeMicroseconds(DServo1);                          // set servo to default position
  Servo2.writeMicroseconds(DServo2);                          // set servo to default position
  Servo3.writeMicroseconds(DServo3);                          // set servo to default position
  Servo4.writeMicroseconds(DServo4);                          // set servo to default position
  Servo5.writeMicroseconds(DServo5);                          // set servo to default position
  Servo6.writeMicroseconds(DServo6);                          // set servo to default position

  //------------------------------------------------------------ Initialize I/O pins --------------------------------------------------

  pinMode (Charger,OUTPUT);                                   // change Charger pin to output
  digitalWrite (Charger,1);                                   // disable current regulator to charge battery

  if (Cmode==1) 
  {
    Serial.begin(Brate);                                      // enable serial communications if Cmode=1
    Serial.flush();                                           // flush buffer
  } 
  //Serial.begin(57600);
}


void loop()
{
  //------------------------------------------------------------ Check battery voltage and current draw of motors ---------------------
  LeftMotor_MonitorCurrent();
  RightMotor_MonitorCurrent();

  MonitorBatteryVoltage();

  //------------------------------------------------------------ CHARGE BATTERY -------------------------------------------------------
  if ((Charged == 0)) //&& (Volts-startVolts>65))                  // if battery is flat and charger has been connected (voltage has increased by at least 1V)
  {
    RechargeBattery();
  }
  else
  {
    //----------------------------------------------------------- GOOD BATTERY speed controller opperates normally ---------------------- 
    AdjustKickOffPWM();     
    AdjustOnSpotTurn();
    CheckMinimumPWMs();
    SelectOperationMode();
    // --------------------------------------------------------- Code to drive dual "H" bridges --------------------------------------
    MonitorLeftPWM_HBridge();
    MonitorRightPWM_HBridge();
           
 //   DeccelerateIfNoKeyReceived();
     
    /*if (Charged==1)                                           // Only power motors if battery voltage is good
    {
      if ((millis()-leftoverload)>overloadtime)             
      {
        switch (Leftmode)                                     // if left motor has not overloaded recently
        {
        case 2:                                               // left motor forward
          analogWrite(LmotorA,0);
          analogWrite(LmotorB,LeftPWM);
          break;

        case 1:                                               // left motor brake
          analogWrite(LmotorA,LeftPWM);
          analogWrite(LmotorB,LeftPWM);
          break;

        case 0:                                               // left motor reverse
          analogWrite(LmotorA,LeftPWM);
          analogWrite(LmotorB,0);
          break;
        }
      }
      if ((millis()-rightoverload)>overloadtime)
      {
        switch (Rightmode)                                    // if right motor has not overloaded recently
        {
        case 2:                                               // right motor forward
          analogWrite(RmotorA,0);
          analogWrite(RmotorB,RightPWM);
          break;

        case 1:                                               // right motor brake
          analogWrite(RmotorA,RightPWM);
          analogWrite(RmotorB,RightPWM);
          break;

        case 0:                                               // right motor reverse
          analogWrite(RmotorA,RightPWM);
          analogWrite(RmotorB,0);
          break;
        }
      } 
    }
    else                                                      // Battery is flat
    {
      analogWrite (LmotorA,0);                                // turn off motors
      analogWrite (LmotorB,0);                                // turn off motors
      analogWrite (RmotorA,0);                                // turn off motors
      analogWrite (RmotorB,0);                                // turn off motors
    }*/
  }
}

void DeccelerateIfNoKeyReceived()
{
  if (LeftPWM > 0)
  {
    LeftPWM = LeftPWM - DeccelRate;
  }
  else
  {     
    LeftPWM = LeftPWM + DeccelRate;
  }
      
  if (RightPWM > 0)
  {
    RightPWM = RightPWM - DeccelRate;
  }
  else
  {
    RightPWM = RightPWM + DeccelRate;
  }
/*      
  if ((RightPWM - LeftPWM) > 1)
    RightPWM = RightPWM - 0.05;
      
  if ((LeftPWM > RightPWM) > 1)
    LeftPWM = LeftPWM - 0.05;  
    */
}
  
void TurnOffMotors()
{
  analogWrite (LmotorA,0);                                // turn off motors
  analogWrite (LmotorB,0);                                // turn off motors
  analogWrite (RmotorA,0);                                // turn off motors
  analogWrite (RmotorB,0);                                // turn off motors  
}

void RechargeBattery()
{
  if (Volts < lowvolt)
  {
    TurnOffMotors();                                          // temporarily used to carry the robot back to the charger  
  }
    
  if (Volts > highVolts)                                      // has battery voltage increased?
  {
    highVolts = Volts;                                        // record the highest voltage. Used to detect peak charging.
    chargeTimer = millis();                                   // when voltage increases record the time
  }

  if (Volts > batvolt)                                        // battery voltage must be higher than this before peak charging can occur.
  {
    Serial.print(    "Timer: ");
    Serial.println(millis() - chargeTimer);
    if ((highVolts - Volts) > 5 || (millis() - chargeTimer) > chargetimeout || Volts > maxvolt ) // has voltage begun to drop or levelled out?
    {
      Charged = 1;                                            // battery voltage has peaked
      digitalWrite(Charger,0);                             // turn off current regulator
      //digitalWrite (Charger,1);                             // turn off current regulator
    }
  }   
}

void MonitorBatteryVoltage()
{
  Volts = analogRead(Battery);                                  // read the battery voltage
  LeftAmps = analogRead(LmotorC);                               // read left motor current draw
  RightAmps = analogRead(RmotorC);                              // read right motor current draw
/*  
  Serial.print("Volts: ");
  Serial.print(Volts);
  Serial.print("     Charging: ");
  Serial.println(Charged);
  */
  if ((Volts < lowvolt) && (Charged == 1))                       // check condition of the battery  
  {                                                           // change battery status from charged to flat

    //---------------------------------------------------------- FLAT BATTERY speed controller shuts down until battery is recharged ----
    //---------------------------------------------------------- This is a safety feature to prevent malfunction at low voltages!! ------
    Charged = 0;                                                // battery is flat
    highVolts = Volts;                                          // record the voltage
    startVolts = Volts;
    chargeTimer = millis();                                     // record the time

    digitalWrite(Charger,0);                                 // enable current regulator to charge battery
  }  
}

void LeftMotor_MonitorCurrent()
{
  if (LeftAmps > Leftmaxamps)                                   // is motor current draw exceeding safe limit
  {
    analogWrite (LmotorA,0);                                  // turn off motors
    analogWrite (LmotorB,0);                                  // turn off motors
    leftoverload = millis();                                    // record time of overload
  }  
}

void RightMotor_MonitorCurrent()
{
  if (RightAmps > Rightmaxamps)                                 // is motor current draw exceeding safe limit
  {
    analogWrite (RmotorA,0);                                  // turn off motors
    analogWrite (RmotorB,0);                                  // turn off motors
    rightoverload=millis();                                   // record time of overload
  }
}




void RCmode()
{
  //------------------------------------------------------------ Code for RC inputs ---------------------------------------------------------

  Speed=pulseIn(RCleft,HIGH,25000);                           // read throttle/left stick
  Steer=pulseIn(RCright,HIGH,25000);                          // read steering/right stick


  if (Speed==0) Speed=1500;                                   // if pulseIn times out (25mS) then set speed to stop
  if (Steer==0) Steer=1500;                                   // if pulseIn times out (25mS) then set steer to centre

  if (abs(Speed-1500)<RCdeadband) Speed=1500;                 // if Speed input is within deadband set to 1500 (1500uS=center position for most servos)
  if (abs(Steer-1500)<RCdeadband) Steer=1500;                 // if Steer input is within deadband set to 1500 (1500uS=center position for most servos)

  if (Mix==1)                                                 // Mixes speed and steering signals
  {
    Steer=Steer-1500;
    Leftspeed=Speed-Steer;
    Rightspeed=Speed+Steer;
  }
  else                                                        // Individual stick control
  {
    Leftspeed=Speed;
    Rightspeed=Steer;
  }
  /*
  Serial.print("Left:");
  Serial.print(Leftspeed);
  Serial.print(" -- Right:");
  Serial.println(Rightspeed);
  */
  Leftmode=2;
  Rightmode=2;
  if (Leftspeed>(Leftcenter+RCdeadband)) Leftmode=0;          // if left input is forward then set left mode to forward
  if (Rightspeed>(Rightcenter+RCdeadband)) Rightmode=0;       // if right input is forward then set right mode to forward

  LeftPWM=abs(Leftspeed-Leftcenter)*10/scale;                 // scale 1000-2000uS to 0-255
  LeftPWM=min(LeftPWM,255);                                   // set maximum limit 255

  RightPWM=abs(Rightspeed-Rightcenter)*10/scale;              // scale 1000-2000uS to 0-255
  RightPWM=min(RightPWM,255);                                 // set maximum limit 255
}







void SCmode()
{// ------------------------------------------------------------ Code for Serial Communications --------------------------------------

                                                              // FL = flush serial buffer
 
                                                              // AN = report Analog inputs 1-5
                                                              
                                                              // SV = next 7 integers will be position information for servos 0-6
 
                                                              // HB = "H" bridge data - next 4 bytes will be:
                                                              //      left  motor mode 0-2
                                                              //      left  motor PWM  0-255
                                                              //      right motor mode 0-2
                                                              //      right motor PWM  0-255
   
 

  if (Serial.available() >= 1)                                   // command available
  {
    Volts=analogRead(Battery);                                  // read the battery voltage
    Serial.print("Volts: ");
    Serial.println(Volts);
    
    CmdReceived = Serial.read();
        
    switch(CmdReceived)
    {
      case 'w':
//        CheckIfTurning();
        CheckIfTurning_New();
        LeftPWM_Prev = LeftPWM;
        RightPWM_Prev = RightPWM;
        MoveForward();
        CmdPrev = CmdReceived;
        Serial.println("Forward L");
        break;
      case 's':
//        CheckIfTurning();
        CheckIfTurning_New();
        LeftPWM_Prev = LeftPWM;
        RightPWM_Prev = RightPWM;        
        MoveBackward();
        CmdPrev = CmdReceived;   
        Serial.println("Backward R");
        break;
      case 'd': //turn on spot
        TurnLeft();
        CmdPrev = CmdReceived;        
        Serial.println("Left");
        break;
      case 'a': //turn on spot
        TurnRight();
        CmdPrev = CmdReceived;        
        Serial.println("Right");
        break;   
      case 'x':
        LeftPWM = 0;
        RightPWM = 0;
        CmdPrev = CmdReceived;
                
        break; 
      case 'q':        
        ShiftLeft();
        CmdPrev = CmdReceived;
        Serial.println("Shift L");
        break;   
      case 'e':
        ShiftRight();
        CmdPrev = CmdReceived;
        Serial.println("Shift R");
        break; 
    }

    Serial.println(LeftPWM);
    Serial.println(RightPWM);        
   
    /*int B=Serial.read();
    int command=A*256+B;
    switch (command)
    {
      case 17996:                                             // FL
        Serial.flush();                                       // flush buffer
        break;
        
      case 16718:                                             // AN - return values of analog inputs 1-5
        for (int i=1;i<6;i++)                                 // index analog inputs 1-5
        {
          data=analogRead(i);                                 // read 10bit analog input 
          Serial.write(highByte(data));                       // transmit high byte
          Serial.write(lowByte(data));                        // transmit low byte
        }
        break;
              
       case 21334:                                            // SV - receive postion information for servos 0-6
         for (int i=0;i<15;i++)                               // read 14 bytes of data
         {
           Serialread();                                      
           servo[i]=data;
         }
         Servo0.writeMicroseconds(servo[0]*256+servo[1]);     // set servo position
         Servo1.writeMicroseconds(servo[2]*256+servo[3]);     // set servo position
         Servo2.writeMicroseconds(servo[4]*256+servo[5]);     // set servo position
         Servo3.writeMicroseconds(servo[6]*256+servo[7]);     // set servo position
         Servo4.writeMicroseconds(servo[8]*256+servo[9]);     // set servo position
         Servo5.writeMicroseconds(servo[10]*256+servo[11]);   // set servo position
         Servo6.writeMicroseconds(servo[12]*256+servo[13]);   // set servo position
         break;
       
       case 18498:                                            // HB - mode and PWM data for left and right motors
         Serialread();
         Leftmode=data;
         Serialread();
         LeftPWM=data;
         Serialread();
         Rightmode=data;
         Serialread();
         RightPWM=data;
         break;
         
       default:                                                // invalid command
         Serial.flush();                                       // flush buffer
    }*/
  }
}

void SelectOperationMode()
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

void MonitorRightPWM_HBridge()  // This code is incorporated into the controller by default
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

void MonitorLeftPWM_HBridge()  // This code is incorporated into the controller by default
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

void AdjustKickOffPWM()
{
  if(StartFlag != 0)
  {
    if(CmdReceived == 'w')
    {
      AdjustForwardToNormal();
    }    
    if(CmdReceived == 's')
    {
      AdjustBackwardToNormal();
    }
  }    
}

void AdjustBackwardToNormal()
{
  if(LeftPWM < (-1 * InitStablePWM))
  {
    LeftPWM = LeftPWM + InitMoveAdjustValue;
  }
  if(RightPWM < (-1 * InitStablePWM))
  {
    RightPWM = RightPWM + InitMoveAdjustValue;
  }
  if(LeftPWM >= (-1 * InitStablePWM) && RightPWM >= (-1 * InitStablePWM) && StartFlag == 1)
  {
    StartFlag = 0;
    LeftPWM_Prev = LeftPWM;
    RightPWM_Prev = RightPWM;

  }  
}

void AdjustForwardToNormal()
{
  if(LeftPWM > InitStablePWM)
  {
    LeftPWM = LeftPWM - InitMoveAdjustValue;
  }
  if(RightPWM > InitStablePWM)
  {
    RightPWM = RightPWM - InitMoveAdjustValue;
  }
  if(LeftPWM <= InitStablePWM && RightPWM <= InitStablePWM && StartFlag == 1)
  {
    LeftPWM_Prev = LeftPWM;
    RightPWM_Prev = RightPWM;    
    StartFlag = 0;
  }  
}

void TurnLeft()
{
  if(CmdPrev == CmdReceived)
  {
    if(StartTurnFlag == 1)
    {
      StartTurnFlag = 1;
    }
    if(LeftPWM > (-1 * MaxStraightLimit) && RightPWM < MaxStraightLimit)
    {
      RightPWM = RightPWM + TurnRate;
      LeftPWM = LeftPWM - TurnRate;
    }
  }
  else
  {
    RightPWM = InitStableTurnPWM;
    LeftPWM = -1 * InitStableTurnPWM;
    StartTurnFlag = 1;
  }  
}

void AdjustOnSpotTurn()
{
  if(StartTurnFlag == 1)
  {
    if(LeftPWM > 0 && RightPWM < 0)
    {
      if(LeftPWM > InitStableTurn && RightPWM < (-1 * InitStableTurn))
      {
        LeftPWM = LeftPWM - InitMoveAdjustValue;
        RightPWM = RightPWM + InitMoveAdjustValue;
      }
      else if(LeftPWM <= InitStableTurn && RightPWM >= (-1 * InitStableTurn))
      {
        StartTurnFlag = 0;
      }    
    }
    else if(LeftPWM < 0 && RightPWM > 0)
    {
      if(LeftPWM < (-1 * InitStableTurn) && RightPWM > InitStableTurn)
      {
        LeftPWM = LeftPWM + InitMoveAdjustValue;
        RightPWM = RightPWM - InitMoveAdjustValue;
      }
      else if(LeftPWM >= (-1 * InitStableTurn) && RightPWM <= InitStableTurn)
      {
        StartTurnFlag = 0;
      }
    }
  }
}

void TurnLeft_New()
{
  if(LeftPWM == 0 && RightPWM == 0)
  {
    RightPWM = InitStableTurnPWM;
    LeftPWM = -1 * InitStableTurnPWM;
  }  
  else if(CmdPrev == CmdReceived && LeftPWM < 0 && RightPWM > 0)
  {
    if(LeftPWM > (-1 * MaxStraightLimit) && RightPWM < MaxStraightLimit)
    {
      RightPWM = RightPWM + TurnRate;
      LeftPWM = LeftPWM - TurnRate;
    }
  }  
  else if(LeftPWM > 0 && RightPWM > 0)
  {
    RightPWM = RightPWM + TurnRate;
  }
  else if(LeftPWM < 0 && RightPWM < 0)
  {
    RightPWM = RightPWM - TurnRate;
  } 
}

void TurnRight()
{
  if(CmdPrev == CmdReceived)
  {
    if(StartTurnFlag == 1)
    {
      StartTurnFlag = 1;
    }    
    if(RightPWM > (-1 * MaxStraightLimit) && LeftPWM < MaxStraightLimit)
    {
      RightPWM = RightPWM - TurnRate;
      LeftPWM = LeftPWM + TurnRate;
    }
  }
  else
  {
    LeftPWM = InitStableTurnPWM;
    RightPWM = -1 * InitStableTurnPWM;
    StartTurnFlag = 1;
  }  
}

void ShiftLeft()
{
  if(RightPWM > 0 && LeftPWM > 0)
  {
    if (RightPWM > LeftPWM)
      RightPWM = RightPWM - ShiftRate;
    else
      LeftPWM = LeftPWM + ShiftRate;
    
    if(LeftPWM >= MaxStraightLimit)
      LeftPWM = MaxStraightLimit;
  }
  else if (RightPWM < 0 && LeftPWM < 0)
  {
    if (LeftPWM > RightPWM)
      RightPWM = RightPWM + ShiftRate;
    else
      LeftPWM = LeftPWM - ShiftRate;
    
    if(LeftPWM <= (-1 * MaxStraightLimit))
      LeftPWM = (-1 * MaxStraightLimit);
  }    
}

void ShiftRight()
{
  if(RightPWM > 0 && LeftPWM > 0)
  {
    if (LeftPWM > RightPWM)
      LeftPWM = LeftPWM - ShiftRate;
    else
      RightPWM = RightPWM + ShiftRate;
    
    if(RightPWM >= MaxStraightLimit)
      RightPWM = MaxStraightLimit;
  }
  else if (RightPWM < 0 && LeftPWM < 0)
  {
    if (RightPWM > LeftPWM)
      LeftPWM = LeftPWM + ShiftRate;
    else
      RightPWM = RightPWM - ShiftRate;
    
    if(LeftPWM <= (-1 * MaxStraightLimit))
      RightPWM = (-1 * MaxStraightLimit);
  }
 
}

void CheckIfTurning()
{
  if(CmdPrev == 'd' || CmdPrev == 'a')
  {
    LeftPWM = 0.0;
    RightPWM = 0.0;
  }          
}

void CheckIfTurning_New()
{
  /*
  if(CmdPrev == 'q' || CmdPrev == 'e')
  {
    LeftPWM = LeftPWM_Prev;
    RightPWM = RightPWM_Prev;
    Serial.println("--------------");
    Serial.println(LeftPWM_Prev);
    Serial.println(RightPWM_Prev);        
    Serial.println("--------------");      
  }  
  else*/ if(CmdPrev == 'd' || CmdPrev == 'a')
  {
    if(CmdReceived == 'w')
    {
      if(LeftPWM_Prev < InitStablePWM || RightPWM_Prev < InitStablePWM)
      {
        LeftPWM = MaxInitStraightMove;
        RightPWM = MaxInitStraightMove;    
        StartFlag = 1;        
      }
      else
      {
        LeftPWM = LeftPWM_Prev;
        RightPWM = RightPWM_Prev;
      }
    }
    else if(CmdReceived == 's')
    {
      if(LeftPWM_Prev > (-1 * InitStablePWM) || RightPWM_Prev > (-1 * InitStablePWM))
      {
        LeftPWM = -1 * MaxInitStraightMove;
        RightPWM = -1 * MaxInitStraightMove;    
        StartFlag = 1;                
      }
      else
      {
        LeftPWM = LeftPWM_Prev;
        RightPWM = RightPWM_Prev;
      }      
    }
  }
  
}

void MoveForward()
{
  if(LeftPWM == 0 && RightPWM == 0)
  {
    LeftPWM = MaxInitStraightMove;
    RightPWM = MaxInitStraightMove;
    StartFlag = 1;
  }
  else
  {
    if(LeftPWM < MaxStraightLimit && RightPWM < MaxStraightLimit)
    {
      LeftPWM = LeftPWM + AccelRate;
      RightPWM = RightPWM + AccelRate;
    }
  }          
}

void MoveBackward()
{
  if(LeftPWM == 0 && RightPWM == 0)
  {
    LeftPWM = -1 * MaxInitStraightMove;
    RightPWM = -1 * MaxInitStraightMove;
    StartFlag = 1;
  }
  else
  {
    if(LeftPWM > (-1 * MaxStraightLimit) && RightPWM > (-1 * MaxStraightLimit))
    {
      LeftPWM = LeftPWM - AccelRate;
      RightPWM = RightPWM - AccelRate;
    }
  }      
}

void CheckMinimumPWMs()
{
  if(LeftPWM < 0.0 && RightPWM < 0.0)
  {
    if(LeftPWM > (-1 * MinLimit) || RightPWM > (-1 *MinLimit))
    {
      LeftPWM = 0.0;
      RightPWM = 0.0;
    }    
  }
  else if(LeftPWM > 0.0 && RightPWM > 0.0)
  {
    if(LeftPWM < MinLimit || RightPWM < MinLimit)
    {
      LeftPWM = 0.0;
      RightPWM = 0.0;
    }        
  }
}

void Serialread() 
{//---------------------------------------------------------- Read serial port until data has been received -----------------------------------
  do 
  {
    data=Serial.read();
  } while (data<0);
}
    






void I2Cmode()
{//----------------------------------------------------------- Your code goes here ------------------------------------------------------------

}












