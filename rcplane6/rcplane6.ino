//SDA (data line) is on analog input pin 4, and SCL (clock line) is on analog input pin 5
#include <Wire.h>
#define I2C_ADDRESS 0x4C
#define TILT_STATUS 0x03
#define SRST 0x04
#define SPCNT 0x05
#define INTSU 0x06
#define MODE 0x07
#define SR 0x08
#define PDET 0x09
#define PD 0x0A
#define POWER_UP_DELAY 10
#define X_OUT 0x00
#define Y_OUT 0x01
#define Z_OUT 0x02
unsigned char REG_ADDRESS[3];
int mode = 0;
int pin = 3;
int leftMotor = 5;
int rightMotor = 6;
void setup()
{
  Wire.begin();
  Serial.begin(9600);  // start serial for output
  Serial.println("Setting up I2C protocol..."); 
  Serial.println("Powering up SLAVE interface...");
  SLAVE_I2C_INIT(); 
  Serial.println("here");
  pinMode(pin, OUTPUT);
  pinMode(leftMotor, OUTPUT);
  pinMode(rightMotor, OUTPUT);
  
  for(int i=9;i<13;i++)
  pinMode(i,OUTPUT);
  for(int j=0;j<3;j++)
  pinMode(j,OUTPUT);

}

void loop()
{
  for(int i=9;i<13;i++)digitalWrite(i,HIGH);
  for(int j=0;j<3;j++)
  digitalWrite(j,LOW);
  switch(mode)
  {
    case 0:mode0();break;
    case 1:mode1();break;
    case 2:mode2();break;
  }
}

void mode0()
{
        int i;
        int j,t2;
        long t1=millis();
      
  	for(i=0;i<562;i++)
	{
		Serial.println("high");
                digitalWrite(leftMotor,HIGH);
                digitalWrite(rightMotor,HIGH);
                delayMicro(1500);
		digitalWrite(leftMotor,LOW);
                digitalWrite(rightMotor,LOW);
		delayMicro(12638);
	}
      t2=millis();
        
  	for(j=0;j<288;j++)
	{
		Serial.println("LOW");
                digitalWrite(leftMotor,HIGH);
                digitalWrite(rightMotor,HIGH);
                digitalWrite(pin,HIGH);
		delayMicro(869);
		digitalWrite(leftMotor,LOW);
                digitalWrite(rightMotor,LOW);
                digitalWrite(pin,LOW);
		delayMicro(13270);
	}
      long t3=millis();
      Serial.println(t3-t2);
      Serial.println(t2-t1);
      
  mode = 2;
}

void mode1()
{
         int i,i1,i2;
         i1=micros();
  		digitalWrite(leftMotor,HIGH);
                digitalWrite(rightMotor,HIGH);
		delayMicro(5000);
		digitalWrite(leftMotor,LOW);
                digitalWrite(rightMotor,LOW);
		delayMicro(5000);
	i2=micros();
        Serial.println(i2-i1);
        for(;;);
mode = 2;
}
  
void mode2()
{

  int motor = 0;
  SLAVE_I2C_READ();
  int yaxis = REG_ADDRESS[1];
  if(yaxis>=43)yaxis-=64;
  if(yaxis-1>=4)yaxis=(yaxis-4)*3.8;
  else yaxis=4;
  int xaxis = REG_ADDRESS[0];
  if(xaxis>=45)xaxis-=64;
  xaxis-=3;
  if(xaxis>3){motor = 0;xaxis =3+(xaxis-3)*4.25; }
  else if(xaxis<-3){motor=1;xaxis =(-1*xaxis-3)*3.8;}
  else xaxis = 4;
  motordrive(motor,xaxis,yaxis,3);
}

void motordrive(int motor,int xx, int yy,int tt)
{
  
            xx = 5.52*xx+1113;
            yy =5.52*yy+1047;
            tt = 5.52*tt+1113;
            long t1,t2,t3,t4,t5;
            if(motor == 0)
            {
              if(yy<tt){
            t1=micros();
            digitalWrite(pin,HIGH);
            digitalWrite(leftMotor,HIGH);
            digitalWrite(rightMotor,HIGH);
            delayMicro(yy);
            t2=micros();
            digitalWrite(pin,LOW);
            delayMicro(tt-yy);
            t3=micros();
            digitalWrite(rightMotor,LOW);
            delayMicro(xx-tt);
            t4=micros();
            digitalWrite(leftMotor,LOW);
            delayMicro(13025-xx);
            t5=micros();
            }
            else
            {
            t1=micros();
            digitalWrite(pin,HIGH);
            digitalWrite(leftMotor,HIGH);
            digitalWrite(rightMotor,HIGH);
            delayMicro(tt);
            t2=micros();
            digitalWrite(rightMotor,LOW);
            delayMicro(yy-tt);
            t3=micros();
            digitalWrite(pin,LOW);
            delayMicro(xx-yy);
            t4=micros();
            digitalWrite(leftMotor,LOW);
            delayMicro(13025-xx);
            t5=micros();
          
            }
          } 
          else
          {
            if(yy<tt)
            {
          t1=micros();
            digitalWrite(pin,HIGH);
            digitalWrite(leftMotor,HIGH);
            digitalWrite(rightMotor,HIGH);
            delayMicro(yy);
            t2=micros();
            digitalWrite(pin,LOW);
            delayMicro(tt-yy);
            t3=micros();
            digitalWrite(leftMotor,LOW);
            delayMicro(xx-tt);
            t4=micros();
            digitalWrite(rightMotor,LOW);
            delayMicro(13025-xx);
            t5=micros();
          }  
          else{
          
          t1=micros();
            digitalWrite(pin,HIGH);
            digitalWrite(leftMotor,HIGH);
            digitalWrite(rightMotor,HIGH);
            delayMicro(tt);
            t2=micros();
            digitalWrite(leftMotor,LOW);
            delayMicro(yy-tt);
            t3=micros();
            digitalWrite(pin,LOW);
            delayMicro(xx-yy);
            t4=micros();
            digitalWrite(rightMotor,LOW);
            delayMicro(13025-xx);
            t5=micros();
          
          }
          }
          
        
        /*if(motor == 0)
        {
          if(xx < yy)
          {
            t1=micros();
            digitalWrite(pin,HIGH);
            digitalWrite(leftMotor,HIGH);
            digitalWrite(rightMotor,HIGH);
            delayMicro(1113+tt);
            t2=micros();
            digitalWrite(rightMotor,LOW);
            delayMicro(xx-tt);
            t3=micros();
            digitalWrite(leftMotor,LOW);
            delayMicro(yy-xx);
            t4=micros();
            digitalWrite(pin,LOW);
            delayMicro(13025-yy);
            t5=micros();
          }
          else
          {
            digitalWrite(pin,HIGH);
            digitalWrite(leftMotor,HIGH);
            digitalWrite(rightMotor,HIGH);
            t1=micros();
            delayMicro(1113+tt);
            digitalWrite(rightMotor,LOW);
            t2=micros();
            delayMicro(yy-tt);
            digitalWrite(pin,LOW);
            t3=micros();
            delayMicro(xx-yy);
            digitalWrite(leftMotor,LOW);
            t4=micros();
            delayMicro(13025-xx);
            t5=micros();
          }
        }
        else
        {
          if(xx < yy)
          {
            t1=micros();
            digitalWrite(pin,HIGH);
            digitalWrite(leftMotor,HIGH);
            digitalWrite(rightMotor,HIGH);
            delayMicro(1113+tt);
            t2=micros();
            digitalWrite(leftMotor,LOW);
            delayMicro(xx-tt);
            t3=micros();
            digitalWrite(rightMotor,LOW);
            delayMicro(yy-xx);
            t4=micros();
            digitalWrite(pin,LOW);
            delayMicro(13025-yy);
            t5=micros();

          }
          else
          {
            t1=micros();
            digitalWrite(pin,HIGH);
            digitalWrite(leftMotor,HIGH);
            digitalWrite(rightMotor,HIGH);
            delayMicro(1113+tt);
            digitalWrite(leftMotor,LOW);
            t2=micros();

            delayMicro(yy-tt);
            digitalWrite(pin,LOW);
            t3=micros();

            delayMicro(xx-yy);
            digitalWrite(rightMotor,LOW);
            t4=micros();

            delayMicro(13025-xx);
            t5=micros();

          }
        }
        */
        Serial.print(t2-t1);
        Serial.print(" 2:");
        Serial.print(t3-t2);
        Serial.print(" 3:");
        Serial.print(t4-t3);
        Serial.print(" 4:");
        Serial.print(t5-t4);
        Serial.print(" x:");Serial.print(xx);
        Serial.print(" y:");Serial.print(yy);
        Serial.print(" t:");Serial.println(tt);
        
       
        
         
}
void SLAVE_I2C_INIT()
{

 

    SLAVE_I2C_SEND(0x07,0x00); // Setting up MODE to Stand by to set SR
    delay(2);
    
    SLAVE_I2C_SEND(0x06,0x10);
    delay(2);
    
    SLAVE_I2C_SEND(0x08,0x00);  // Setting up SR register to 120 samples active and auto sleep mode
    delay(2); 
 
    SLAVE_I2C_SEND(0x07,0x01); //Setting up MODE Active to START measures 

 
}

void SLAVE_I2C_SEND(unsigned char REG_ADDRESS, unsigned  char DATA)  //SEND data to MMA7660
{
  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(REG_ADDRESS);
  Wire.write(DATA);
  Wire.endTransmission();
}

void SLAVE_I2C_READ() //READ MMA7660 data
{
  int i=0;
  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(0x00);  // register to read
  Wire.endTransmission();
  Wire.requestFrom(I2C_ADDRESS, 3); // read a byte
  
  for(i=0; i<3; i++){
   if(Wire.available())REG_ADDRESS[i]=Wire.read();
  }
}

void delayMicro(int x)
{
  int i;
  for(i=0;i<x;i++)
  {
    delayMicroseconds(1);
  }
}
    
