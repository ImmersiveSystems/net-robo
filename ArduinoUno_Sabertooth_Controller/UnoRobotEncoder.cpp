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

void UnoRobotEncoder::SetLeftEncoder()
{
  pinMode(encoderPinA[0], INPUT);
  digitalWrite(encoderPinA[0], HIGH); // enables pull-up resistor
  pinMode(encoderPinB[0], INPUT);
  digitalWrite(encoderPinB[0], HIGH); // enables pull-up resistor  

  encoderPinA_Set[0] = digitalRead(encoderPinA[0]);
  encoderPinB_Set[0] = digitalRead(encoderPinB[0]);  
}

void UnoRobotEncoder::SetRightEncoder()
{
  pinMode(encoderPinA[1], INPUT);
  digitalWrite(encoderPinA[1], HIGH); // enables pull-up resistor
  pinMode(encoderPinB[1], INPUT);
  digitalWrite(encoderPinB[1], HIGH); // enables pull-up resistor  

  encoderPinA_Set[1] = digitalRead(encoderPinA[1]);
  encoderPinB_Set[1] = digitalRead(encoderPinB[1]);  
}
void UnoRobotEncoder::InitEncoderPins()
{
  SetLeftEncoder();
  SetRightEncoder();
  Serial.begin(9600);      
}

void UnoRobotEncoder::MonitorLeftEncoder()
{
  encoderPinA_Set[0] = digitalRead(encoderPinA[0]) == HIGH;
  encoderPos[0] += (encoderPinA_Set[0] != encoderPinB_Set[0]) ? +1 : -1;
  encoderPinB_Set[0] = digitalRead(encoderPinB[0]) == HIGH;
  encoderPos[0] += (encoderPinA_Set[0] == encoderPinB_Set[0]) ? +1 : -1;
}

void UnoRobotEncoder::MonitorRightEncoder()
{
  encoderPinA_Set[1] = digitalRead(encoderPinA[1]) == HIGH;
  encoderPos[1] += (encoderPinA_Set[1] != encoderPinB_Set[1]) ? -1 : +1;
  encoderPinB_Set[1] = digitalRead(encoderPinB[1]) == HIGH;
  encoderPos[1] += (encoderPinA_Set[1] == encoderPinB_Set[1]) ? -1 : +1;
}

void UnoRobotEncoder::SendLeftEncoderValue()
{
  Serial.println(encoderPos[0]);
}
void UnoRobotEncoder::SendRightEncoderValue()
{
  Serial.println(encoderPos[1]);
}

void UnoRobotEncoder::ShowEncoderValue()
{  
  // Serial.print("Left:   ");
  // Serial.println(encoderPos[0]);
  // Serial.print("Right:   ");
  // Serial.println(encoderPos[1]);
  String left = String(encoderPos[0], DEC);    
  String right = String(encoderPos[1], DEC);
  String msg = left + ' ' + right;
  Serial.println(msg);
}