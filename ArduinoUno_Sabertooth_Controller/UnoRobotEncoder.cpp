#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "UnoRobotEncoder.h"

#define WhichEncoder(Mode) (Mode == 0 ? "Right Encoder:  " : "Left Encoder:  ")

long UnoRobotEncoder::encoderPos_Prev[2];

UnoRobotEncoder::UnoRobotEncoder()
{	
  encoderPinA[0] = 2;
  encoderPinA[1] = 3;
  encoderPinB[0] = 8;
  encoderPinB[1] = 9;

  encoderPinA_Set[0] = 0;
  encoderPinA_Set[1] = 0;
  encoderPinB_Set[0] = 0;
  encoderPinB_Set[1] = 0;

  encoderPos[0] = 0;
  encoderPos[1] = 0;
  encoderPos_Prev[0] = 0;
  encoderPos_Prev[1] = 0;
  encoderPos_TrackPrev[0] = 0;
  encoderPos_TrackPrev[1] = 0;

  EncoderValuesMsg = "";
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
  encoderPos_TrackPrev[0] = encoderPos[0];
  encoderPos[0] += (encoderPinA_Set[0] == encoderPinB_Set[0]) ? +1 : -1;
}

void UnoRobotEncoder::MonitorRightEncoder()
{
  encoderPinA_Set[1] = digitalRead(encoderPinA[1]) == HIGH;
  encoderPos[1] += (encoderPinA_Set[1] != encoderPinB_Set[1]) ? -1 : +1;
  encoderPinB_Set[1] = digitalRead(encoderPinB[1]) == HIGH;
  encoderPos_TrackPrev[1] = encoderPos[1];
  encoderPos[1] += (encoderPinA_Set[1] == encoderPinB_Set[1]) ? -1 : +1;
}

void UnoRobotEncoder::UpdateEncoderValues()
{  
  String left = String(encoderPos[0], DEC);    
  String right = String(encoderPos[1], DEC);
  EncoderValuesMsg = left + ' ' + right;
  // Serial.println(msg);
}