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
  encoderPinA[1] = 8;
  encoderPinB[0] = 3;
  encoderPinB[1] = 9;
  encoderPos[0] = 0;
  encoderPos[1] = 0;
  encoderPinA_Set[0] = 0;
  encoderPinA_Set[1] = 0;
  encoderPinB_Set[0] = 0;
  encoderPinB_Set[1] = 0;

  // encoderPinA_Cur[0] = LOW;
  // encoderPinA_Cur[1] = LOW;
  // encoderPinA_Last[0] = LOW;
  // encoderPinA_Last[1] = LOW;
}

void UnoRobotEncoder::InitEncoderPins()
{
  for(int i=0; i<2; i++)
  {
    pinMode(encoderPinA[i], INPUT);
    digitalWrite(encoderPinA[i], HIGH); // enables pull-up resistor
    pinMode(encoderPinB[i] , INPUT);
    digitalWrite(encoderPinB[i], HIGH); // enables pull-up resistor

    encoderPinA_Set[i] = digitalRead(encoderPinA[i]);
    encoderPinB_Set[i] = digitalRead(encoderPinB[i]);
  }  
  Serial.begin(9600);	
}

void UnoRobotEncoder::ShowEncoderValue(int Mode)
{
  Serial.println(WhichEncoder(Mode));
  Serial.println(encoderPos[Mode]);
}

// void UnoRobotEncoder::ReadEncoder(int Mode)
// {
//   encoderPinA_Cur[Mode] = digitalRead(encoderPinA[Mode]);
//   if((encoderPinA_Last[Mode] == LOW) && (encoderPinA_Cur[Mode] == HIGH))
//   {
//     if(digitalRead(encoderPinB[Mode]) == LOW)
//     {
//       encoderPos[Mode]--;
//     }
//     else
//     {
//       encoderPos[Mode]++;
//     }
//     Serial.println(WhichEncoder(Mode));
//     Serial.println(encoderPos[Mode]);
//   }
//   encoderPinA_Last[Mode] = encoderPinA_Cur[Mode];
// }

void UnoRobotEncoder::EncoderPinChange_Left()
{
  encoderPinA_Set[0] = digitalRead(encoderPinA[0]) == HIGH;
  encoderPos[0] += (encoderPinA_Set[0] != encoderPinB_Set[0]) ? +1 : -1;
  encoderPinB_Set[0] = digitalRead(encoderPinB[0]) == HIGH;
  encoderPos[0] += (encoderPinA_Set[0] == encoderPinB_Set[0]) ? +1 : -1;
}

void UnoRobotEncoder::EncoderPinChange_Right()
{
  encoderPinA_Set[1] = digitalRead(encoderPinA[1]) == HIGH;
  encoderPos[1] += (encoderPinA_Set[1] != encoderPinB_Set[1]) ? +1 : -1;  
  encoderPinB_Set[1] = digitalRead(encoderPinB[1]) == HIGH;
  encoderPos[1] += (encoderPinA_Set[1] == encoderPinB_Set[1]) ? +1 : -1;
}


// void doEncoder() {
//   /* If pinA and pinB are both high or both low, it is spinning
//    * forward. If they're different, it's going backward.
//    *
//    * For more information on speeding up this process, see
//    * [Reference/PortManipulation], specifically the PIND register.
//    */
//   if (digitalRead(encoder0PinA) == digitalRead(encoder0PinB)) {
//     encoder0Pos++;
//   } else {
//     encoder0Pos--;
//   }

//   Serial.println (encoder0Pos, DEC);
// }

// // Using both pins high/low to track the encoder changes
// void doEncoderA(){

//   // look for a low-to-high on channel A
//   if (digitalRead(encoder0PinA) == HIGH) { 
//     // check channel B to see which way encoder is turning
//     if (digitalRead(encoder0PinB) == LOW) {  
//       encoder0Pos = encoder0Pos + 1;         // CW
//     } 
//     else {
//       encoder0Pos = encoder0Pos - 1;         // CCW
//     }
//   }
//   else   // must be a high-to-low edge on channel A                                       
//   { 
//     // check channel B to see which way encoder is turning  
//     if (digitalRead(encoder0PinB) == HIGH) {   
//       encoder0Pos = encoder0Pos + 1;          // CW
//     } 
//     else {
//       encoder0Pos = encoder0Pos - 1;          // CCW
//     }
//   }
//   Serial.println (encoder0Pos, DEC);          
//   // use for debugging - remember to comment out
// }

// void doEncoderB(){

//   // look for a low-to-high on channel B
//   if (digitalRead(encoder0PinB) == HIGH) {   
//    // check channel A to see which way encoder is turning
//     if (digitalRead(encoder0PinA) == HIGH) {  
//       encoder0Pos = encoder0Pos + 1;         // CW
//     } 
//     else {
//       encoder0Pos = encoder0Pos - 1;         // CCW
//     }
//   }
//   // Look for a high-to-low on channel B
//   else { 
//     // check channel B to see which way encoder is turning  
//     if (digitalRead(encoder0PinA) == LOW) {   
//       encoder0Pos = encoder0Pos + 1;          // CW
//     } 
//     else {
//       encoder0Pos = encoder0Pos - 1;          // CCW
//     }
//   }
// }