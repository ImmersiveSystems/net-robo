// ArduinoUno_Sabertooth_Controller.ino

#include "UnoConstants.h"
#include "UnoRobotEncoder.h"
#include "UnoRobotController.h"

#include <Servo.h>

const byte pin_0A =  2; // connect white wire here
const byte pin_0B =  8; // connect black wire here
const byte pin_1A =  3; // connect white wire here
const byte pin_1B =  9; // connect black wire here

int A0_set =         0;
int B0_set =         0;
long pulses0 =       0;

int A1_set =         0;
int B1_set =         0;
long pulses1 =       0;

UnoRobotController robot;

void setup()
{
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

  robot.InitServos();
  robot.InitSaber();
  // robot.InitEncoderPins();
  
  attachInterrupt(0, encoderPinChange_A, CHANGE); // pin 2
  attachInterrupt(1, encoderPinChange_B, CHANGE); // pin 3
  Serial.begin(9600);      
}

void loop()
{
  Serial.print("Left:   ");
  Serial.println(pulses0);
  Serial.print("   Right:   ");
  Serial.println(pulses1);  

  // robot.ShowEncoderValue(RightMotor - 1);
  // robot.ShowEncoderValue(LeftMotor - 1);

  for(int pos = 0; pos < 180; pos++)
  {
  	robot.TestServos(1);
	  robot.UpdateServosPos();
	  delay(ServoDelayValue);
  }
  for(int pos = 180; pos >= 0; pos--)
  {
  	robot.TestServos(-1);
  	robot.UpdateServosPos();
	  delay(ServoDelayValue);
  }
}

// void encoderPinChange_A()
// {
//   robot.EncoderPinChange_Left();
// }

// void encoderPinChange_B()
// {
//   robot.EncoderPinChange_Right();
// }

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