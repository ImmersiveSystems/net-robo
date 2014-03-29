//Trex serial communication code
//listens for a ser.write('H' + chr(2) + chr(100) + chr(2) + chr(100)) type of a command on serial and than drives motors



#define lmbrkpin     4  //  D4 - left  motor brake        control    pin    HIGH = Brake 
#define lmdirpin     2  //  D2 - left  motor direction    control    pin    HIGH = Forward   Low = Reverse
#define lmpwmpin     3  //  D3 - left  motor pulse width  modulation pin    0 - 255          Speed and Brake 

#define rmbrkpin     9  //  D9 - right motor brake        control    pin    HIGH = Brake 
#define rmdirpin    10  // D10 - right motor direction    control    pin    HIGH = Forward   Low = Reverse
#define rmpwmpin    11  // D11 - right motor pulse width  modulation pin    0 - 255          Speed and Brake 

#define voltspin     3  //  A3 - battery voltage          1V = 33.57        30V = 1007
#define axisxpin     0  //  A0 - accelerometer X-axis
#define axisypin     1  //  A1 - accelerometer Y-axis
#define axiszpin     2  //  A2 - accelerometer Z-axis

#define voltspin     3  //  A3 - battery voltage          1V = 33.57        30V = 1007

int data = 0;
int Leftmode = 0;
int Rightmode = 0;

byte lmbrake, rmbrake;
int lmspeed, rmspeed;

int xaxis,yaxis,zaxis;                                 // X, Y, Z accelerometer readings
int deltx,delty,deltz;                                 // X, Y, Z impact readings 
int magnitude;                                         // impact magnitude
byte devibrate=50;                                     // number of 2mS intervals to wait after an impact has occured before a new impact can be recognized
int sensitivity=50; 

int volts; 
unsigned long time;                                    // timer used to monitor accelerometer and encoders

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
  ReadSensors();
  Serial.println(magnitude);
  Serial.println(volts);
  
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


void ReadSensors()
{
  
  
  static byte alternate;                               // variable used to alternate between reading accelerometer and power analog inputs
  
  
  
  //----------------------------------------------------- Perform these functions every 1mS ---------------------------------------------- 
  if(micros()-time>999)                       
  {
    time=micros();                                     // reset timer
    alternate=alternate^1;                             // toggle alternate between 0 and 1

    //--------------------------------------------------- These functions must alternate as they both take in excess of 780uS ------------    
    if(alternate)
    {
      Accelerometer();                                 // monitor accelerometer every second millisecond                            
    }
    else 
    {
      volts=analogRead(voltspin)*10/3.357;             // read battery level and convert to volts with 2 decimal places (eg. 1007 = 10.07 V)
    }
  }
}







