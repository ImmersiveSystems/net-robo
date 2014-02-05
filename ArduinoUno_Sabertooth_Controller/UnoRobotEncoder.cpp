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