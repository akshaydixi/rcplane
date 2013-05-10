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



void setup()
{
  Wire.begin();
  Serial.begin(9600);  // start serial for output
  Serial.println("Setting up I2C protocol..."); 
  Serial.println("Powering up SLAVE interface...");
  SLAVE_I2C_INIT(); 
  pinMode(13, OUTPUT);
pinMode(0,OUTPUT);
pinMode(1,OUTPUT);
pinMode(9,OUTPUT);
pinMode(10,OUTPUT);}

void loop()
{digitalWrite(0,LOW);
digitalWrite(1,LOW);
digitalWrite(9,HIGH);
digitalWrite(10,HIGH);

SLAVE_I2C_READ();

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
  unsigned char REG_ADDRESS[3];
  int i=0;
  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(0x00);  // register to read
  Wire.endTransmission();
  Wire.requestFrom(I2C_ADDRESS, 3); // read a byte
  
  for(i=0; i<3; i++){
   if(Wire.available())REG_ADDRESS[i]=Wire.read();
  }
  for(i=0; i<3; i++){
    Serial.print("  || 0x0");
    Serial.print(i);
    Serial.print(":");
    Serial.print(REG_ADDRESS[i], DEC);
    Serial.print("  ||");
    
  }
  Serial.println("");
  delay(100);
}


