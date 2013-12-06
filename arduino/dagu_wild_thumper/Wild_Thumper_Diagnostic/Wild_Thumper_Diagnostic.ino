//george's code

#include <Servo.h>
#include "IOpins.h"
#include "Constants.h"


//-------------------------------------------------------------- define global variables --------------------------------------------

unsigned int Volts;
unsigned int LeftAmps;
unsigned int RightAmps;
unsigned long chargeTimer=millis();
unsigned long leftoverload;
unsigned long rightoverload;
int highVolts;
int startVolts;
int Leftspeed=0;
int Rightspeed=0;
int Speed;
int Steer;
byte Charged=1;                                               // 0=Flat battery  1=Charged battery
int Leftmode=0;                                               // 0=reverse, 1=brake, 2=forward
int Rightmode=0;                                              // 0=reverse, 1=brake, 2=forward
byte Leftmodechange=0;                                        // Left input must be 1500 before brake or reverse can occur
byte Rightmodechange=0;                                       // Right input must be 1500 before brake or reverse can occur
int LeftPWM=0;                                                  // PWM value for left  motor speed / brake
int RightPWM=0;                                                 // PWM value for right motor speed / brake
int data=1100;
int servo[7];

//-------------------------------------------------------------- define servos ------------------------------------------------------


Servo Servo0;                                                 // define servos
Servo Servo1;                                                 // define servos
Servo Servo2;                                                 // define servos
Servo Servo3;                                                 // define servos
Servo Servo4;                                                 // define servos
Servo Servo5;                                                 // define servos
Servo Servo6;                                                 // define servos

void setup()
{
  //------------------------------------------------------------ Initialize Servos ----------------------------------------------------

  Servo0.attach(S0);                                          // attach servo to I/O pin
  Servo1.attach(S1);                                          // attach servo to I/O pin
  Servo2.attach(S2);                                          // attach servo to I/O pin
  Servo3.attach(S3);                                          // attach servo to I/O pin
  Servo4.attach(S4);                                          // attach servo to I/O pin
  Servo5.attach(S5);                                          // attach servo to I/O pin
  Servo6.attach(S6);                                          // attach servo to I/O pin


  //------------------------------------------------------------ Initialize I/O pins --------------------------------------------------

  pinMode (Charger,OUTPUT);                                   // change Charger pin to output
  digitalWrite (Charger,0);                                   // disable current regulator to charge battery

  Serial.begin(9600);
}

void stop(void)                         //Stop
{
  analogWrite(LmotorA,0);
  analogWrite(LmotorB,0); 
  analogWrite(RmotorA,0);
  analogWrite(RmotorB,0);     
  Serial.println("Stopped");
}   
        
void advance(char a,char b)             //Move forward
{
  analogWrite(LmotorA,0);
  analogWrite(LmotorB,a); 
  analogWrite(RmotorA,0);
  analogWrite(RmotorB,b);
  Serial.println("Went Forward");
}  
        
void back_off (char a,char b)           //Move backward
{
  analogWrite(LmotorA,a);
  analogWrite(LmotorB,0); 
  analogWrite(RmotorA,b);
  analogWrite(RmotorB,0);
  Serial.println("Went Backwards");
}

void turn_L (char a,char b)             //Turn Left
{
  Serial.println("Turned Left");
}
void turn_R (char a,char b)             //Turn Right
{
  Serial.println("Turned Right");
}


void print_I_V()
{
  Volts=analogRead(Battery);                                  // read the battery voltage
  LeftAmps=analogRead(LmotorC);                               // read left motor current draw
  RightAmps=analogRead(RmotorC);                              // read right motor current draw

  Serial.print("Battery Voltage:");
  Serial.print(Volts);
  Serial.print("   Left motor current:");
  Serial.print(LeftAmps);
  Serial.print("   Right motor current:");
  Serial.println(RightAmps);
}

void loop()
{
    char ch = Serial.read();
    if(ch != 1)
    {
      switch(ch)
      {
        case 'w':
          advance(0,127);
          Serial.println("Forward L");
          break;
        case 's':
          advance(127,0);
          Serial.println("Forward R");
          break;
        case 'd':
          turn_L(127,100);
          Serial.println("Left");
          break;
        case 'a':
          turn_R(100,127);
          Serial.println("Right");
          break;   
        case 'q':
          stop();
          break; 
      }
      delay(40); 
      print_I_V();
    }
    else stop();
  
  //------------------------------------------------------------ Check battery voltage and current draw of motors ---------------------


}
 /*
  Servo0.writeMicroseconds(data);                             // set servo to default position
  Servo1.writeMicroseconds(data);                             // set servo to default position
  Servo2.writeMicroseconds(data);                             // set servo to default position
  Servo3.writeMicroseconds(data);                             // set servo to default position
  Servo4.writeMicroseconds(data);                             // set servo to default position
  Servo5.writeMicroseconds(data);                             // set servo to default position
  Servo6.writeMicroseconds(data);                             // set servo to default position

  if (data>1900) 
  {
    Leftmode+=2;
    Rightmode+=2;
    Leftmode*=(Leftmode<3);
    Rightmode*=(Rightmode<3);
  }
  data+=10-800*(data>1900);


  // --------------------------------------------------------- Code to drive dual "H" bridges --------------------------------------

  switch (Leftmode)                                     // if left motor has not overloaded recently
  {
  case 2:                                               // left motor forward
    analogWrite(LmotorA,0);
    analogWrite(LmotorB,LeftPWM);
    break;

  case 1:                                               // left motor brake
    analogWrite(LmotorA,LeftPWM);
    analogWrite(LmotorB,LeftPWM);
    break;

  case 0:                                               // left motor reverse
    analogWrite(LmotorA,LeftPWM);
    analogWrite(LmotorB,0);
    break;
  }

  switch (Rightmode)                                    
  {
  case 2:                                               // right motor forward
    analogWrite(RmotorA,0);
    analogWrite(RmotorB,RightPWM);
    break;

  case 1:                                               // right motor brake
    analogWrite(RmotorA,RightPWM);
    analogWrite(RmotorB,RightPWM);
    break;

  case 0:                                               // right motor reverse
    analogWrite(RmotorA,RightPWM);
    analogWrite(RmotorB,0);
    break;

} 

*/







