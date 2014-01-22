void setup()  
{
  Serial.begin(9600);  
}

int speed1 = 127;
char speed2 = (char) speed1; 

int speed3 = 255;
char speed4 = (char) speed3; 

void loop() // run over and over
{
 // Serial.write((char) speed1);
  Serial.write(speed2);
  Serial.write(speed4);
  //delay(2000);
}
