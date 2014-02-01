// ArduinoUno_Sabertooth_Controller.ino
#include <Servo.h>
#include <Sabertooth.h>

Sabertooth SaberTooth(128);

int RightMotor = 1;
int LeftMotor = 2;

int data = 0;
int Leftmode = 0;
int Rightmode = 0;
int LeftPWM = 0;                                       
int RightPWM = 0;

int pinTilt = 5;
int pinPan = 6;

int tilt = 90;
int pan = 90;

Servo servoTilt;
Servo servoPan;

void setup()
{
  SabertoothTXPinSerial.begin(9600);
  SaberTooth.autobaud(); 
  SaberTooth.drive(0);
  SaberTooth.turn(0);

  servoTilt.attach(pinTilt);
  servoPan.attach(pinPan);
}

void loop()
{
  if (Serial.available() > 0)                                   // command available
  {
    int command = Serial.read();

    switch(command)
    {
      case 'H':
        Set_PWM(RightMotor);
        Set_PWM(LeftMotor);
        break;
      case 'P':
        Serialread();
        pan = data;
        servoPan.write(pan);
        break;
      case 'T':
        Serialread();
        tilt = data;
        servoTilt.write(tilt);
        break;
      default:
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
      RightPWM = data * 1.27;  
      ProcessMotorCommand(Rightmode, RightMotor, RightPWM);
      break;    
   
    case 2:
      Serialread();
      Leftmode = data;
      Serialread();
      LeftPWM = data * 1.27;
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
      SaberTooth.motor(MotorNum, PWMVal);
      break;
   
    case 1: //Stop
      SaberTooth.stop();
      break;
   
    case 0: //Move Backward
      SaberTooth.motor(MotorNum, -1 * PWMVal);  
      break;
  }

}

void Serialread() 
{
  do
  {
    data = Serial.read();  
  }
  while (data < 0);

}