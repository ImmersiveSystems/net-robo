#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "UnoRobotEncoder.h"

#define WhichEncoder(Mode) (Mode == 0 ? "Right Encoder:  " : "Left Encoder:  ")

UnoRobotEncoder::UnoRobotEncoder()
{	
  encoderPinA[0] = 2;
  encoderPinA[1] = 3;
  encoderPinB[0] = 8;
  encoderPinB[1] = 9;
  encoderPos[0] = 0;
  encoderPos[1] = 0;
  encoderPinA_Set[0] = 0;
  encoderPinA_Set[1] = 0;
  encoderPinB_Set[0] = 0;
  encoderPinB_Set[1] = 0;
}

void UnoRobotEncoder::InitEncoderPins()
{
  pinMode(encoderPinA[0], INPUT);
  digitalWrite(encoderPinA[0], HIGH); // enables pull-up resistor
  pinMode(encoderPinB[0], INPUT);
  digitalWrite(encoderPinB[0], HIGH); // enables pull-up resistor  

  encoderPinA_Set[0] = digitalRead(encoderPinA[0]);
  encoderPinB_Set[0] = digitalRead(encoderPinB[0]);

  pinMode(encoderPinA[1], INPUT);
  digitalWrite(encoderPinA[1], HIGH); // enables pull-up resistor
  pinMode(encoderPinB[1], INPUT);
  digitalWrite(encoderPinB[1], HIGH); // enables pull-up resistor  

  encoderPinA_Set[1] = digitalRead(encoderPinA[1]);
  encoderPinB_Set[1] = digitalRead(encoderPinB[1]);
  Serial.begin(9600);      
}

// void UnoRobotEncoder::ReadEncoder(int Mode)
// {
//   encoderPinA_Set[Mode] = digitalRead(encoderPinA[Mode]) == HIGH;
//   encoderPinB_Set[Mode] = digitalRead(encoderPinB[Mode]) == HIGH;
//   switch(Mode)
//   {
//     case 0:
//       encoderPos[Mode] += (encoderPinA_Set[Mode] != encoderPinB_Set[Mode]) ? +1 : -1;      
//       encoderPos[Mode] += (encoderPinA_Set[Mode] == encoderPinB_Set[Mode]) ? +1 : -1;    
//       break;
//     case 1:
//       encoderPos[Mode] += (encoderPinA_Set[Mode] != encoderPinB_Set[Mode]) ? -1 : +1;
//       encoderPos[Mode] += (encoderPinA_Set[Mode] == encoderPinB_Set[Mode]) ? -1 : +1;    
//       break;
//   }
// }
void UnoRobotEncoder::encoderPinChange_Left()
{
  encoderPinA_Set[0] = digitalRead(encoderPinA[0]) == HIGH;
  encoderPos[0] += (encoderPinA_Set[0] != encoderPinB_Set[0]) ? +1 : -1;
  encoderPinB_Set[0] = digitalRead(encoderPinB[0]) == HIGH;
  encoderPos[0] += (encoderPinA_Set[0] == encoderPinB_Set[0]) ? +1 : -1;
}

void UnoRobotEncoder::encoderPinChange_Right()
{
  encoderPinA_Set[1] = digitalRead(encoderPinA[1]) == HIGH;
  encoderPos[1] += (encoderPinA_Set[1] != encoderPinB_Set[1]) ? -1 : +1;
  encoderPinB_Set[1] = digitalRead(encoderPinB[1]) == HIGH;
  encoderPos[1] += (encoderPinA_Set[1] == encoderPinB_Set[1]) ? -1 : +1;
}

void UnoRobotEncoder::ShowEncoderValue()
{
  Serial.print("Left:   ");
  Serial.println(encoderPos[0]);
  Serial.print("Right:   ");
  Serial.println(encoderPos[1]);    
}