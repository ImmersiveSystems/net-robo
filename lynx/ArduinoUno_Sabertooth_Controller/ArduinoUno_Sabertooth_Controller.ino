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
#define PanAdjustRate 2
#define TiltAdjustRate 2
#define ClawMax 180
#define ClawMin 0

const byte pin_0A =  2; // connect white wire here
const byte pin_0B =  8; // connect black wire here
const byte pin_1A =  3; // connect white wire here
const byte pin_1B =  9; // connect black wire here

int oldDistance = 1023; // distance measured by ir sensor 
int newDistance = 1023;


int A0_set =         0;
int B0_set =         0;
long pulses0 =       0;

int A1_set =         0;
int B1_set =         0;
long pulses1 =       0;

int RightMotor = 1;
int LeftMotor = 2;

int data = 0;
int Leftmode = 0;
int Rightmode = 0;
int LeftPWM = 0;                                       
int RightPWM = 0;

int pinTilt = 12;
int pinPan = 11;
int pinElbow = 4;
int pinClaw = 6;
int pinWrist = 5;
int pinDistSensor = 7;

int tilt = 120;
int pan = 90;
int elbow = 95;
int claw = ClawMax;
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
  
  pinMode(pin_0A, INPUT);
  digitalWrite(pin_0A, HIGH); // enables pull-up resistor
  pinMode(pin_0B, INPUT);
  digitalWrite(pin_0B, HIGH); // enables pull-up resistor  

  A0_set = digitalRead(pin_0A);
  B0_set = digitalRead(pin_0B);

  pinMode(pin_1A, INPUT);
  digitalWrite(pin_1A, HIGH); // enables pull-up resistor
  pinMode(pin_1B, INPUT);
  digitalWrite(pin_1B, HIGH); // enables pull-up resistor  

  A1_set = digitalRead(pin_1A);
  B1_set = digitalRead(pin_1B);
  attachInterrupt(0, encoderPinChange_A, CHANGE); // pin 2
  attachInterrupt(1, encoderPinChange_B, CHANGE); // pin 3
  Serial.begin(9600);      
}

void loop()
{
  
  newDistance = analogRead(pinDistSensor); 
 // if (abs(newDistance - oldDistance) > 20){
  Serial.write('D');
  Serial.println(newDistance); 
 // oldDistance = newDistance;
//  } 
  
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
        // pan = data;
        if(data == 1)
        {
          pan = pan - PanAdjustRate;
        }
        else if(data == 0)
        {
          pan = pan + PanAdjustRate;
        }                
        pan = ValidateServoCurPos(pinPan, pan);
        servoPan.write(pan);
        break;
      case 'T':
        Serialread();
        // tilt = data;
        if(data == 1)
        {
          tilt = tilt - TiltAdjustRate;
        }
        else if(data == 0)
        {
          tilt = tilt + TiltAdjustRate;
        }          
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
        if(claw == ClawMin)
        {
          claw = ClawMax;
        }
        else if(claw == ClawMax)
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
  // Serial.print(RightPWM);
  // Serial.print('  ');
  // Serial.println(LeftPWM);

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

void encoderPinChange_A()
{
  A0_set = digitalRead(pin_0A) == HIGH;
  pulses0 += (A0_set != B0_set) ? +1 : -1;
  B0_set = digitalRead(pin_0B) == HIGH;
  pulses0 += (A0_set == B0_set) ? +1 : -1;
}

void encoderPinChange_B()
{
  A1_set = digitalRead(pin_1A) == HIGH;
  pulses1 += (A1_set != B1_set) ? -1 : +1;  
  B1_set = digitalRead(pin_1B) == HIGH;
  pulses1 += (A1_set == B1_set) ? -1 : +1;
}
