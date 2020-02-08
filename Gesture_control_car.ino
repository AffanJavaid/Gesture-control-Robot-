#include <Wire.h>

int lm1=10,lm2=11,rm1=12,rm2=13; //Output pins to be connected to 10, 11, 12, 13 of encoder IC

long accelX, accelY, accelZ;
float gForceX, gForceY, gForceZ;


void setup() {
  Serial.begin(9600);
  Wire.begin();// for MPU
  setupMPU();

  pinMode(lm1,OUTPUT); // for motor 
  pinMode(lm2,OUTPUT);
  pinMode(rm1,OUTPUT);
  pinMode(rm2,OUTPUT);
  
}


void loop() {
  recordAccelRegisters();  // get values of accelerometer
  printData();
  
  if(gForceY>67)      // Change the value for adjusting sensitivity  
     forward();
  else if(gForceY<-50) // Change the value for adjusting sensitivity
     backward();
  else if(gForceX>66) // Change the value for adjusting sensitivity
     right();
  else if(gForceX<-47) // Change the value for adjusting sensitivity
     left();

  else
  {
     stop_();
  }
  delay(500);
}

////functions///////////////////////functions//////////////////////////////////functions//////////////////////////////////functions///////////////////////functions/////////////////////////

///////////////////////////////////////////////////////////////////function 1

void setupMPU(){    // used to set up the MPU for the first time 
  Wire.beginTransmission(0x68); //This is the I2C address of the MPU (b1101000/b1101001 for AC0 low/high datasheet sec. 9.2)
  Wire.write(0x6B); //Accessing the register 6B - Power Management (Sec. 4.28)
  Wire.write(0b00000000); //Setting SLEEP register to 0. (Required; see Note on p. 9)
  Wire.endTransmission();  
  
   
  Wire.beginTransmission(0x68); //I2C address of the MPU
  Wire.write(0x1C); //Accessing the register 1C - Acccelerometer Configuration (Sec. 4.5) 
  Wire.write(0b00000000); //Setting the accel to +/- 2g
  Wire.endTransmission(); 
}
//////////////////////////////////////////////////////////////////////////////function 2
void recordAccelRegisters() {
  Wire.beginTransmission(0x68); //I2C address of the MPU
  Wire.write(0x3B); //Starting register for Accel Readings
  Wire.endTransmission();
  Wire.requestFrom(0x68,6); //Request Accel Registers (3B - 40)
  while(Wire.available() < 6);
  accelX = Wire.read()<<8|Wire.read(); //Store first two bytes into accelX
  accelY = Wire.read()<<8|Wire.read(); //Store middle two bytes into accelY
  accelZ = Wire.read()<<8|Wire.read(); //Store last two bytes into accelZ
  processAccelData();
}
/////////////////////////////////////////////////////////////////////function 3

void processAccelData(){
  gForceX = (accelX / 16384.0)*100;  // later used for left and right
  gForceY = (accelY / 16384.0)*100;   // later used for forward and reverse 
  gForceZ = (accelZ / 16384.0)*100;   // later used for move round 
}

////////////////////////////////////////////////////////////////////////function 4

void printData() {

  Serial.print(" Accel (g)");
  Serial.print(" X=");
  Serial.print(gForceX);
  Serial.print(" Y=");
  Serial.print(gForceY);
  Serial.print(" Z=");
  Serial.println(gForceZ);
 delay(50);
}

////////////////////////////////////////////////////////////////////////////function 5

void stop_()
{
  Serial.println("");
  Serial.println("STOP");
  digitalWrite(lm1,LOW);
  digitalWrite(lm2,LOW);
  digitalWrite(rm1,LOW);
  digitalWrite(rm2,LOW);
}
///////////////////////////////////////////////////////////////////////////function 6

void forward()
{
  Serial.println("");
  Serial.println("Forward");
  digitalWrite(lm1,HIGH);
  digitalWrite(lm2,LOW);
  digitalWrite(rm1,HIGH);
  digitalWrite(rm2,LOW);
}
/////////////////////////////////////////////////////////////////////function 7

void backward()
{
  Serial.println("");
  Serial.println("Backward");
  digitalWrite(lm1,LOW);
  digitalWrite(lm2,HIGH);
  digitalWrite(rm1,LOW);
  digitalWrite(rm2,HIGH);
}
//////////////////////////////////////////////////////////////////////////function 8
void left()
{
  Serial.println("");
  Serial.println("Left");
  digitalWrite(lm1,LOW);
  digitalWrite(lm2,HIGH);
  digitalWrite(rm1,HIGH);
  digitalWrite(rm2,LOW);
}
///////////////////////////////////////////////////////////////////////function 9
void right()
{
  Serial.println("");
  Serial.println("Right");
  digitalWrite(lm1,HIGH);
  digitalWrite(lm2,LOW);
  digitalWrite(rm1,LOW);
  digitalWrite(rm2,HIGH);
}
void CircleLeft()
{
  Serial.println("");
  Serial.println("CircleLeft");
  digitalWrite(lm1,LOW);
  digitalWrite(lm2,LOW);
  digitalWrite(rm1,HIGH);
  digitalWrite(rm2,LOW);
}
void CircleRight()
{
  Serial.println("");
  Serial.println("CircleRight");
  digitalWrite(lm1,HIGH);
  digitalWrite(lm2,LOW);
  digitalWrite(rm1,LOW);
  digitalWrite(rm2,LOW);
}
