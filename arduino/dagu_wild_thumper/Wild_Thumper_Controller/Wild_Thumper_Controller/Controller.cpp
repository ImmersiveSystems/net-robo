#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Servo.h>
#include "Controller.h"
#include "IOpins.h"
#include "Constants.h"

#include <avr/power.h>
#include "avr/sleep.h"

int Controller::Charged;

Controller::Controller()
{
  data = 0; 
  LeftPWMStartFlag = 0;
  RightPWMStartFlag = 0;
    
  Volts = 0;
  LeftAmps = 0;
  RightAmps = 0;
  chargeTimer = 0.0;
  leftoverload = 0.0;
  rightoverload = 0.0;
  ConvertedVolts = 0;
   
  // 0=Flat battery  1=Charged battery
  Charged = 1;      
  highVolts = 0;
  startVolts = 0;	
      	
  // 0=reverse, 1=brake, 2=forward
  Leftmode = 1;
  Rightmode = 1;

  // PWM value for left  motor speed / brake
  LeftPWM = 0;
  RightPWM = 0;
  LeftPWM_Prev = 0;
  RightPWM_Prev = 0;

  prevTime = 0;
}

int Controller::ReturnChargedFlagStatus()
{ 
  return Charged;
}

void Controller::InitServos_IO_Pin()
{
  // attach servo to I/O pin
  Servo0.attach(S0);                                          
  Servo1.attach(S1);
  Servo2.attach(S2);
  Servo3.attach(S3);
  Servo4.attach(S4);
  Servo5.attach(S5);
  Servo6.attach(S6);
}

void Controller::InitServosDefaultPos()
{
  // set servo to default position
  Servo0.writeMicroseconds(DServo0);                          
  Servo1.writeMicroseconds(DServo1);
  Servo2.writeMicroseconds(DServo2);
  Servo3.writeMicroseconds(DServo3);
  Servo4.writeMicroseconds(DServo4);
  Servo5.writeMicroseconds(DServo5);
  Servo6.writeMicroseconds(DServo6);
}

void Controller::Serialread() 
{
  do
  {
      data = Serial.read();  
  }
  while (data < 0);
}

void Controller::ProcessRightMotorCommands()
{
  if ((millis() - rightoverload) > overloadtime)
  {
    // if right motor has not overloaded recently
    switch (Rightmode)
    {
      // right motor forward
      case 2:
        RightMotorForward();
        break;
      // right motor brake
      case 1:                                               
        RightMotorBrake();
        break;
      // right motor reverse
      case 0:                                               
        RightMotorBackward();
        break;
      default:
        break;
    }
  } 
}


void Controller::ProcessLeftMotorCommands()
{
  if ((millis() - leftoverload) > overloadtime)             
  {
    // if left motor has not overloaded recently
    switch (Leftmode)
    {
      // left motor forward
      case 2:
        LeftMotorForward();
        break;
      // left motor brake
      case 1:
        LeftMotorBrake();
        break;
      // left motor reverse
      case 0:
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

void Controller::TurnOffMotors()
{
  analogWrite (LmotorA,0);
  analogWrite (LmotorB,0);
  analogWrite (RmotorA,0);
  analogWrite (RmotorB,0);
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

void Controller::LeftMotorForward()
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

void Controller::RightMotorForward()
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

void Controller::CheckVoltageLevel()
{
  if (Volts < lowvolt)
  {
    LeftPWM = 0;
    RightPWM = 0;
  }  
}

void Controller::RechargeBattery()
{
  if ((Charged==0) && (Volts-startVolts>10))  
  {
    // Serial.println('C');
    // one means it's  charging!  
    // Serial.println(1);  
    
    // has battery voltage increased?
    if (Volts > highVolts)                                      
    {
      // record the highest voltage. Used to detect peak charging.
      highVolts = Volts;
      // when voltage increases record the time
      chargeTimer = millis();                                   
    }

    // battery voltage must be higher than this before peak charging can occur.
    if (Volts >= peakvolt)
    {
      // has voltage begun to drop or levelled out?
      if ((highVolts - Volts) > 5 || (millis() - chargeTimer) > chargetimeout)
      {
        Serial.print("Battery has stopped charging at charge of ");
        Serial.print(ConvertedVolts);
        Serial.print(" at ");
        Serial.println(millis());

        // battery voltage has peaked
        Charged = 1;                    
         // turn off current regulator                       
        digitalWrite (Charger,1);          
      }
    }   
  }
}

void Controller::SendPowerLevel()
{
  //start bit for a power command 
  Serial.println('P');
  Serial.println(ConvertedVolts);  
}

void Controller::DebugInfo()
{
  if (abs(millis() - prevTime) >=  1000)
  {
    prevTime = millis();
    Serial.print(Volts);
    Serial.print(", ");
    Serial.print(ConvertedVolts);
    Serial.print(", ");
    Serial.print(Charged);
    Serial.print(", ");
    Serial.println(millis());
  }
}

void Controller::MonitorBatteryVoltage()
{
  // read the battery voltage
  Volts = analogRead(Battery);    
  // read left motor current draw                              
  LeftAmps = analogRead(LmotorC);    
  // read right motor current draw                           
  RightAmps = analogRead(RmotorC);   

  ConvertedVolts = (double) Volts / VoltageScale;

  // SendPowerLevel();  

  // check condition of the battery  
  if ((Volts < batvolt) && (Charged == 1))                       
  {      
    Serial.print("Battery went flat with charge of ");
    Serial.print(ConvertedVolts);
    Serial.print(" at ");
    Serial.println(millis());

    // change battery status from charged to flat
    Charged = 0;
    // record the voltage
    highVolts = Volts; 
    startVolts = Volts;
    // record the time
    chargeTimer = millis();
    // enable current regulator to charge battery
    digitalWrite(Charger, 0);     
    // 0 means it's charging  
    // Serial.println('C');
    // Serial.println(0);
  }  
}

void Controller::LeftMotor_MonitorCurrent()
{
  // is motor current draw exceeding safe limit
  if (LeftAmps > Leftmaxamps)
  {
    // turn off motors
    analogWrite(LmotorA, 0);
    analogWrite(LmotorB, 0);
    // record time of overload
    leftoverload = millis();
  }  
}

void Controller::RightMotor_MonitorCurrent()
{
  // is motor current draw exceeding safe limit
  if(RightAmps > Rightmaxamps)                                 
  {
    // turn off motors
    analogWrite(RmotorA, 0);                                  
    analogWrite(RmotorB, 0); 
    // record time of overload
    rightoverload = millis();                                   
  }
}

void Controller::SetServosPos()
{
  // read 14 bytes of data
  for (int i=0;i<15;i++)                               
  {
    Serialread();                                      
    servo[i] = data;
  }

  // set servo position
  Servo0.writeMicroseconds(servo[0] * 256 + servo[1]);     
  Servo1.writeMicroseconds(servo[2] * 256 + servo[3]);
  Servo2.writeMicroseconds(servo[4] * 256 + servo[5]);
  Servo3.writeMicroseconds(servo[6] * 256 + servo[7]);
  Servo4.writeMicroseconds(servo[8] * 256 + servo[9]);
  Servo5.writeMicroseconds(servo[10] * 256 + servo[11]);
  Servo6.writeMicroseconds(servo[12] * 256 + servo[13]);
}

void Controller::SerialCommunicate()
{
  // FL = flush serial buffer
  // AN = report Analog inputs 1-5
  // SV = next 7 integers will be position information for servos 0-6
  // HB = "H" bridge data - next 4 bytes will be:
  //      left  motor mode 0-2
  //      left  motor PWM  0-255
  //      right motor mode 0-2
  //      right motor PWM  0-255
  
  if (Serial.available() > 0)                                   // command available
  {
    int command = Serial.read();

    switch(command)
    {
      // FL
      case 'F':                                             
        Serial.flush(); 
        break;
      // AN - return values of analog inputs 1-5
      case 'A':                                             
        for (int i=1;i<6;i++)
        {
          // read 10bit analog input 
          data = analogRead(i);               
          // transmit high byte                  
          Serial.write(highByte(data));
          // transmit low byte
          Serial.write(lowByte(data));
        }
        break;
      // SV - receive postion information for servos 0-6
      case 'S':
        SetServosPos();
        break;
      // HB - mode and PWM data for left and right motors
      case 'H':
        SetLeft_PWM();
        SetRight_PWM();
        break;
      case 'W':
        Serial.println("Go");
        Leftmode = 2;
        Rightmode = 2;
        LeftPWM = 127;
        RightPWM = 127;
        break;
      case 'X':
        Serial.println("Stop");
        Leftmode = 2;
        Rightmode = 2;
        LeftPWM = 0;
        RightPWM = 0;
        break;
      case 'T':
        Serial.print("Volts: ");
        Serial.print(Volts);
        Serial.print(" ");
        Serial.print(ConvertedVolts);
        Serial.print("    Timer:");
        Serial.println((millis()-chargeTimer));
        break;
      case 'Z':
        Serial.println("GO TO SLEEP!");
        LeftPWM = 0;
        RightPWM = 0;
        TurnOffMotors();
        delay(100);
        SleepNow();
        break;
      // invalid command
      default:                                                
       Serial.flush();
    }
  }
}

void Controller::SleepNow()
{
  /* Now is the time to set the sleep mode. In the Atmega8 datasheet
   * http://www.atmel.com/dyn/resources/prod_documents/doc2486.pdf on page 35
   * there is a list of sleep modes which explains which clocks and 
   * wake up sources are available in which sleep modus.
   *
   * In the avr/sleep.h file, the call names of these sleep modus are to be found:
   *
   * The 5 different modes are:
   *     SLEEP_MODE_IDLE         -the least power savings 
   *     SLEEP_MODE_ADC
   *     SLEEP_MODE_PWR_SAVE
   *     SLEEP_MODE_STANDBY
   *     SLEEP_MODE_PWR_DOWN     -the most power savings
   *
   *  the power reduction management <avr/power.h>  is described in 
   *  http://www.nongnu.org/avr-libc/user-manual/group__avr__power.html
   */  
  
  // sleep mode is set here
  set_sleep_mode(SLEEP_MODE_IDLE);

  // enables the sleep bit in the mcucr register so sleep is possible. just a safety pin 
  sleep_enable();          
  
  power_adc_disable();
  power_spi_disable();
  power_timer0_disable();
  power_timer1_disable();
  power_timer2_disable();
  power_twi_disable();

  // here the device is actually put to sleep!!
  sleep_mode();            
 
  // THE PROGRAM CONTINUES FROM HERE AFTER WAKING UP
  // first thing after waking from sleep: disable sleep...
  sleep_disable();          

  power_all_enable();
                        
  Serial.println("WOKE UP");
}