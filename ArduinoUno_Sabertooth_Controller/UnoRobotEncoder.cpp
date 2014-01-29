#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "UnoRobotEncoder.h"


#define WhichEncoder(Mode) (Mode == 0 ? "Right Encoder:  " : "Left Encoder:  ")

UnoRobotEncoder::UnoRobotEncoder()
{	
  encoderPinA[0] = 12;
  encoderPinA[1] = 11;
  encoderPinB[0] = 13;
  encoderPinB[1] = 10;
  encoderPos[0] = 0;
  encoderPos[1] = 0;
  encoderPinA_Cur[0] = LOW;
  encoderPinA_Cur[1] = LOW;
  encoderPinA_Last[0] = LOW;
  encoderPinA_Last[1] = LOW;
}

void UnoRobotEncoder::InitEncoderPins()
{
  for(int i=0; i<2; i++)
  {
    pinMode(encoderPinA[i], INPUT);
    pinMode(encoderPinB[i] , INPUT);
  }  
  Serial.begin(9600);	
}

void UnoRobotEncoder::ReadEncoder(int Mode)
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