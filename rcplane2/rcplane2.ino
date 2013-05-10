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

void setup()
{
  Wire.begin();
  Serial.begin(9600);  // start serial for output
  Serial.println("Setting up I2C protocol..."); 
  Serial.println("Powering up SLAVE interface...");
  SLAVE_I2C_INIT(); 
  pinMode(pin, OUTPUT);

}

void loop()
{
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
        int j;
        long t1=millis();
      
  	for(i=0;i<557;i++)
	{
		Serial.println("high");
                digitalWrite(pin,HIGH);
		delayMicroseconds(1900);
		digitalWrite(pin,LOW);
		delayMicroseconds(16000);
	}
      long t2=millis();
        
  	for(j=0;j<977;j++)
	{
		Serial.println("LOW");
                digitalWrite(pin,HIGH);
		delayMicroseconds(1100);
		digitalWrite(pin,LOW);
		delayMicroseconds(16800);
	}
      long t3=millis();
      Serial.println(t3-t2);
      Serial.println(t2-t1);
      
for(i=0;;i++)
	{
		digitalWrite(pin,HIGH);
		delayMicroseconds(1300);
		digitalWrite(pin,LOW);
		delayMicroseconds(16600);
	}
  mode = 1;
}

void mode1()
{
         int i;
  	for(i=0;i<278;i++)
	{
		digitalWrite(pin,HIGH);
		delayMicroseconds(1300);
		digitalWrite(pin,LOW);
		delayMicroseconds(16600);
	}
mode = 2;
}
  
void mode2()
{

  int i;
  SLAVE_I2C_READ();
  motordrive(30);
}

void motordrive(int x)
{
                x=7*x;
                digitalWrite(pin,HIGH);
		delayMicroseconds(1200+x);
		digitalWrite(pin,LOW);
		delayMicroseconds(17900-x);
  
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


