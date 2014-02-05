// ArduinoUno_Sabertooth_Controller.ino
#include <Servo.h>
#include <Sabertooth.h>

Sabertooth SaberTooth(128);

#define MaxTiltAngle 150
#define MaxElbowAngle 140
#define MaxOtherServoAngle 180
#define MinServoAngle 30
#define MinPanAngle 0


#define PWMRate 1.27
#define FORWARD 2
#define STOP 1

#define BACKWARD 0

#define ServoAdjustRate 10
#define ClawMax 180
#define ClawMin 0

int RightMotor = 1;
int LeftMotor = 2;

int data = 0;
int Leftmode = 0;
int Rightmode = 0;
int LeftPWM = 0;                                       
int RightPWM = 0;

int pinTilt = 5;
int pinPan = 6;
int pinElbow = 8;
int pinClaw = 9;
int pinWrist = 10;


int tilt = 120;
int pan = 90;
int elbow = 95;
int claw = 180;
int wrist = 95;


Servo servoTilt;
Servo servoPan;
Servo servoElbow;
Servo servoClaw;
Servo servoWrist;


void setup()
{
  SabertoothTXPinSerial.begin(9600);
  SaberTooth.autobaud(); 
  SaberTooth.drive(0);
  SaberTooth.turn(0);

  servoTilt.attach(pinTilt);
  servoPan.attach(pinPan);
  servoElbow.attach(pinElbow);
  servoClaw.attach(pinClaw);
  servoWrist.attach(pinWrist);

  servoTilt.write(tilt);
  servoPan.write(pan);
  servoElbow.write(elbow);
  servoClaw.write(claw);
  servoWrist.write(wrist);
  // delay(15);
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
        pan = ValidateServoCurPos(pinPan, pan);
        servoPan.write(pan);
        break;
      case 'T':
        Serialread();
        tilt = data;          
        tilt = ValidateServoCurPos(pinTilt, tilt);
        servoTilt.write(tilt);
        break;
      case 'E':
        Serialread();
        if(data == 1)
        {
          elbow = elbow - ServoAdjustRate;
        }
        else if(data == 0)
        {
          elbow = elbow + ServoAdjustRate;
        }
        elbow = ValidateServoCurPos(pinElbow, elbow);
        servoElbow.write(elbow);
        break;
      case 'C':
        if(claw == 0)
        {
          claw = ClawMax;
        }
        else if(claw == 180)
        {
          claw = ClawMin;
        }
        servoClaw.write(claw);
        break;
      case 'W':
        Serialread();
        if(data == 1)
        {
          wrist = wrist - ServoAdjustRate;
        }
        else if(data == 0)
        {
          wrist = wrist + ServoAdjustRate;
        }
        wrist = ValidateServoCurPos(pinWrist, wrist);
        servoWrist.write(wrist);
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
      RightPWM = data * PWMRate; //1.27;  
      ProcessMotorCommand(Rightmode, RightMotor, RightPWM);
      break;    
   
    case 2:
      Serialread();
      Leftmode = data;
      Serialread();
      LeftPWM = data * PWMRate;//1.27;
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
    case FORWARD: //2: //Move Forward
      SaberTooth.motor(MotorNum, PWMVal);
      break;
   
    case STOP: //1: //Stop
      SaberTooth.stop();
      break;
   
    case BACKWARD: //0: //Move Backward
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

int ValidateServoCurPos(int PinNum, int POS)
{
  if(PinNum == pinTilt && POS > MaxTiltAngle)
  {
    return MaxTiltAngle;
  }
  else if(PinNum == pinElbow && POS > MaxElbowAngle)
  {
    return MaxElbowAngle;
  }
  else if((PinNum != pinTilt && PinNum != pinElbow) && POS > MaxOtherServoAngle)
  {
    return MaxOtherServoAngle;
  }
  else if(PinNum == pinPan && POS < MinPanAngle)
  {
    return MinPanAngle;
  }
  else if(PinNum != pinPan && POS < MinServoAngle)
  {
    return MinServoAngle;
  }
  else
  {
    return POS;
  }
}
