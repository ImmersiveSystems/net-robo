#include <Sabertooth.h>

#define SpeedScale 1.27
#define encoderPinA_Right 3
#define encoderPinB_Right 4
#define encoderPinA_Left 6
#define encoderPinB_Left 5

#define RightMotor 1
#define LeftMotor 2
#define StopSignal 0

int data = 0;
int Leftmode = 0;
int Rightmode = 0;
int LeftPWM = 0;                                       
int RightPWM = 0;

int encoderPos_Right = 0;
int encoderPinA_Rgiht_Last = LOW;
int encoderPinA_Right_Cur = LOW;
int encoderPos_Left = 0;
int encoderPinA_Left_Last = LOW;
int encoderPinA_Left_Cur = LOW;


Sabertooth ST(128); // The Sabertooth is on address 128.
                    // If you've set up your Sabertooth on a different address, of course change
                    // that here. For how to configure address, etc. see the DIP Switch Wizard for
                    //   Sabertooth - http://www.dimensionengineering.com/datasheets/SabertoothDIPWizard/start.htm
                    //   SyRen      - http://www.dimensionengineering.com/datasheets/SyrenDIPWizard/start.htm
                    // Be sure to select Packetized Serial Mode for use with this library.
                    // On that note, you can use this library for SyRen just as easily.
                    // The diff-drive commands (drive, turn) do not work on a SyRen, of course, but it will respond correctly
                    // if you command motor 1 to do something (ST.motor(1, ...)), just like a Sabertooth.
                    // In this sample, hardware serial TX connects to S1.
                    // See the SoftwareSerial example in 3.Advanced for how to use other pins.

void setup()
{
  SabertoothTXPinSerial.begin(9600); // 9600 is the default baud rate for Sabertooth packet serial.
  ST.autobaud(); // Send the autobaud command to the Sabertooth controller(s).
                 // NOTE: *Not all* Sabertooth controllers need this command.
                 //       It doesn't hurt anything, but V2 controllers use an
                 //       EEPROM setting (changeable with the function setBaudRate) to set
                 //       the baud rate instead of detecting with autobaud.
                 //       If you have a 2x12, 2x25 V2, 2x60 or SyRen 50, you can remove
                 //       the autobaud line and save yourself two seconds of startup delay.  
  ST.drive(StopSignal);
  ST.turn(StopSignal);

  InitEncoderPins();
}

void loop()
{
  // int power;
  
  // Ramp motor 1 from -127 to 127 (full reverse to full forward),
  // waiting 20 ms (1/50th of a second) per value.
  // for (power = -127; power <= 127; power ++)
  // {
  //   ST.motor(1, power);
  //   delay(20);
  // }
  
  // // Now go back the way we came.
  // for (power = 127; power >= -127; power --)
  // {
  //   ST.motor(1, power); // Tip for SyRen users: Typing ST.motor(power) does the same thing as ST.motor(1, power).
  //   delay(20);          //                      Since SyRen doesn't have a motor 2, this alternative can save you typing.
  // }
  SerialCommunicate();
  ReadEncoder(1);
  ReadEncoder(2);

  // for (power = 127; power >= -127; power --)
  // {
  //   ST.drive(power); // Tip for SyRen users: Typing ST.motor(power) does the same thing as ST.motor(1, power).
  //   delay(20);          //                      Since SyRen doesn't have a motor 2, this alternative can save you typing.
  // }
  

  // for (power = -127; power <= 127; power ++)
  // {
  //   ST.turn(power);
  //   delay(50);
  // }
  // Now stop turning, and stop driving.
  // ST.turn(0);
  // ST.drive(0);
  
  // Wait a bit. This is so you can catch your robot if you want to. :-)
  // delay(3000);
}

void Serialread() 
{
  do
  {
      data = Serial.read();  
  }
  while (data < 0);
}

void SerialCommunicate()
{
  if (Serial.available() > 0)                                   // command available
  {
    if(Serial.read() == 'H')
    {
      Set_PWM(RightMotor);
      Set_PWM(LeftMotor);
    }
    else
    {
      Serial.flush();
    }
  }
}

void Set_PWM(int MotorNum)
{
  switch(MotorNum)
  {
    case 1:
      Serialread();
      Rightmode = data;
      Serialread();
      RightPWM = data * SpeedScale;  
      ProcessMotorCommand(Rightmode, RightMotor, RightPWM);
      break;    
   
    case 2:
      Serialread();
      Leftmode = data;
      Serialread();
      LeftPWM = data * SpeedScale;
      ProcessMotorCommand(Leftmode, LeftMotor, LeftPWM);
      break;
   
    default:
      break;
  }
}

void ProcessMotorCommand(int Mode, int MotorNum, int PWMVal)
{
  switch(Mode)
  {
    case 2: //Move Forward
      ST.motor(MotorNum, PWMVal);
      break;
   
    case 1: //Stop
      ST.motor(MotorNum, 0);
      break;
   
    case 0: //Move Backward
      ST.motor(MotorNum, -1 * PWMVal);  
      break;
  }
}

void InitEncoderPins()
{  
  pinMode(encoderPinA_Right, INPUT);
  pinMode(encoderPinB_Right, INPUT);
  pinMode(encoderPinA_Left, INPUT);
  pinMode(encoderPinB_Left, INPUT);

  Serial.begin(9600);
}

void ReadEncoder(int Mode)
{
  switch(Mode)
  {
   case 1:
     encoderPinA_Right_Cur = digitalRead(encoderPinA_Right);
     if ((encoderPinA_Rgiht_Last == LOW) && (encoderPinA_Right_Cur == HIGH)) 
     {
       if (digitalRead(encoderPinB_Right) == LOW) 
       {
         encoderPos_Right--;
       } 
       else 
       {
         encoderPos_Right++;
       }
       Serial.print("Right Ecoder:  ");
       Serial.println(encoderPos_Right);
     } 
     encoderPinA_Rgiht_Last = encoderPinA_Right_Cur;  
     break;

   case 2:
     encoderPinA_Left_Cur = digitalRead(encoderPinA_Left);
     if ((encoderPinA_Left_Last == LOW) && (encoderPinA_Left_Cur == HIGH)) 
     {
       if (digitalRead(encoderPinB_Left) == LOW) 
       {
         encoderPos_Left--;
       } 
       else 
       {
         encoderPos_Left++;
       }
       Serial.print("Left Ecoder:  ");
       Serial.println(encoderPos_Left);
     } 
     encoderPinA_Left_Last = encoderPinA_Left_Cur;  
     break;   
  }
}