//Trex serial communication code
//listens for a ser.write('H' + chr(2) + chr(100) + chr(2) + chr(100)) type of a command on serial and than drives motors



#define lmbrkpin     4  //  D4 - left  motor brake        control    pin    HIGH = Brake 
#define lmdirpin     2  //  D2 - left  motor direction    control    pin    HIGH = Forward   Low = Reverse
#define lmpwmpin     3  //  D3 - left  motor pulse width  modulation pin    0 - 255          Speed and Brake 

#define rmbrkpin     9  //  D9 - right motor brake        control    pin    HIGH = Brake 
#define rmdirpin    10  // D10 - right motor direction    control    pin    HIGH = Forward   Low = Reverse
#define rmpwmpin    11  // D11 - right motor pulse width  modulation pin    0 - 255          Speed and Brake 

int data = 0;
int Leftmode = 0;
int Rightmode = 0;

byte lmbrake, rmbrake;
int lmspeed, rmspeed;



void setup()
{
  
  pinMode(lmpwmpin,OUTPUT);                            // configure left  motor PWM       pin for output
  pinMode(lmdirpin,OUTPUT);                            // configure left  motor direction pin for output
  pinMode(lmbrkpin,OUTPUT);                            // configure left  motor brake     pin for output
  
  pinMode(rmpwmpin,OUTPUT);                            // configure right motor PWM       pin for output
  pinMode(rmdirpin,OUTPUT);                            // configure right motor direction pin for output
  pinMode(rmbrkpin,OUTPUT);                            // configure right motor brake     pin for output
  Serial.begin(9600);      
}

void loop()
{
  
  if (Serial.available() > 0)                                   // command available
  {
    int command = Serial.read();   
    switch(command)
    {
      case 'H':
      Serialread();
      Leftmode = data;
      
      Serialread();
      lmspeed = data;
      
      Serialread();
      Rightmode = data;
      
      Serialread();
      rmspeed = data;
      
      lmbrake = 0;
      rmbrake = 0;
  
      Motors();

      default:
        Serial.flush();
    }
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


void Motors()
{
  digitalWrite(lmbrkpin,lmbrake>0);                     // if left brake>0 then engage electronic braking for left motor
  digitalWrite(lmdirpin,Leftmode==2);                     // if left speed>0 then left motor direction is forward else reverse
  analogWrite (lmpwmpin,abs(lmspeed));                  // set left PWM to absolute value of left speed - if brake is engaged then PWM controls braking

  digitalWrite(rmbrkpin,rmbrake>0);                     // if right brake>0 then engage electronic braking for right motor
  digitalWrite(rmdirpin,Rightmode==2);                     // if right speed>0 then right motor direction is forward else reverse
  analogWrite (rmpwmpin,abs(rmspeed));                  // set right PWM to absolute value of right speed - if brake is engaged then PWM controls braking
 }






