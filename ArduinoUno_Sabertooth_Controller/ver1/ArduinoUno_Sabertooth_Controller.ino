#include <Sabertooth.h>

#define WhichEncoder(Mode) (Mode == 0 ? "Right Enocder:  " : "Left Encoder:  ")

#define SpeedScale 1.27
#define RightMotor 1
#define LeftMotor 2
#define StopSignal 0

int data = 0;
int Leftmode = 0;
int Rightmode = 0;
int LeftPWM = 0;                                       
int RightPWM = 0;

const int encoderPinA[2] = {3, 6};
const int encoderPinB[2] = {4, 5};
int encoderPos[2] = {0, 0};
int encoderPinA_Cur[2] = {LOW, LOW};
int encoderPinA_Last[2] = {LOW, LOW};

Sabertooth ST(128);

void setup()
{
  printf("We are here");
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
  ReadEncoder(RightMotor - 1);
  ReadEncoder(LeftMotor - 1);
  // ReadEncoder(RightMotor - 1);
  // ReadEncoder(LeftMotor - 1);

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
  for(int i=0; i<2; i++)
  {
    pinMode(encoderPinA[i], INPUT);
    pinMode(encoderPinB[i] , INPUT);
  }  
  Serial.begin(9600);
}

void ReadEncoder(int Mode)
{
  encoderPinA_Cur[Mode] = digitalRead(encoderPinA[Mode]);
  if((encoderPinA_Last[Mode] == LOW) && (encoderPinA_Cur[Mode] == HIGH))
  {
    if(digitalRead(encoderPinB[Mode]) == LOW)
    {
      encoderPos[Mode]--;
    }
    else
    {
      encoderPos[Mode]++;
    }
    Serial.println(WhichEncoder(Mode));
    Serial.println(encoderPos[Mode]);
  }
  encoderPinA_Last[Mode] = encoderPinA_Cur[Mode];
}