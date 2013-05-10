#include <Wire.h>
#define CTRL_REG1 0x20
#define CTRL_REG2 0x21
#define CTRL_REG3 0x22
#define CTRL_REG4 0x23
#define CTRL_REG5 0x24
int mode = 0;
int pin = 3;
int L3G4200D_Address = 105; //I2C address of the L3G4200D
int x;
int y;
int z;
const int numReadings = 10;
int xreadings[numReadings];
int yreadings[numReadings];
int zreadings[numReadings];
int index = 0;
void setup()
{
  pinMode(pin,OUTPUT);
  Wire.begin();
  Serial.begin(9600);
  Serial.println("starting up L3G4200D");
  delay(1500); //wait for the sensor to be ready 

        }

void loop()
{
	switch(mode)
	{
	case 0: mode0();break;
	case 1: mode1();break;
	case 2: mode2();break;
	default: break;
	}
}

void mode0()
{
	int i;
	long  t1=millis();
        for(i=0;i<557;i++)
	{  
                Serial.println("high");
		digitalWrite(pin,HIGH);
		delayMicroseconds(1900);
		digitalWrite(pin,LOW);
		delayMicroseconds(16000);
	}
        long t2=millis();
        i=0;
	for(i=0;i<980;i++)
	{
                Serial.println("low");
		digitalWrite(pin,HIGH);
		delayMicroseconds(1100);
		digitalWrite(pin,LOW);
		delayMicroseconds(16800);
	}

        long t3=millis();
        Serial.println(t2-t1);
        Serial.println(t3-t2);
  
for(;;)
{
                
                Serial.print(t3-t2);
                Serial.println(t2-t1);

    		digitalWrite(pin,HIGH);
		delayMicroseconds(1300);
		digitalWrite(pin,LOW);
		delayMicroseconds(16600);

}
  mode = 0;
}

void mode1()
{
  int i;
  for(index=0;index<numReadings;index++)
  {
    getGyroValues();  // This will update x, y, and z with new values
   // smooth();
  }
  
  Serial.print("X:");
  Serial.print(x);

  Serial.print(" Y:");
  Serial.print(y);

  Serial.print(" Z:");
  Serial.println(z);

  delay(100); //Just here to slow down the serial to make it more readable
}


  
void getGyroValues(){

  byte xMSB = readRegister(L3G4200D_Address, 0x29);
  byte xLSB = readRegister(L3G4200D_Address, 0x28);
  x = ((xMSB << 8) | xLSB);
  xreadings[index] = x;

  byte yMSB = readRegister(L3G4200D_Address, 0x2B);
  byte yLSB = readRegister(L3G4200D_Address, 0x2A);
  y = ((yMSB << 8) | yLSB);
  yreadings[index]=y;
  byte zMSB = readRegister(L3G4200D_Address, 0x2D);
  byte zLSB = readRegister(L3G4200D_Address, 0x2C);
  z = ((zMSB << 8) | zLSB);
  zreadings[index]=z;
}

int setupL3G4200D(int scale){
  //From  Jim Lindblom of Sparkfun's code

  // Enable x, y, z and turn off power down:
  writeRegister(L3G4200D_Address, CTRL_REG1, 0b00001111);

  // If you'd like to adjust/use the HPF, you can edit the line below to configure CTRL_REG2:
  writeRegister(L3G4200D_Address, CTRL_REG2, 0b00000000);

  // Configure CTRL_REG3 to generate data ready interrupt on INT2
  // No interrupts used on INT1, if you'd like to configure INT1
  // or INT2 otherwise, consult the datasheet:
  writeRegister(L3G4200D_Address, CTRL_REG3, 0b00001000);

  // CTRL_REG4 controls the full-scale range, among other things:

  if(scale == 250){
    writeRegister(L3G4200D_Address, CTRL_REG4, 0b00000000);
  }else if(scale == 500){
    writeRegister(L3G4200D_Address, CTRL_REG4, 0b00010000);
  }else{
    writeRegister(L3G4200D_Address, CTRL_REG4, 0b00110000);
  }

  // CTRL_REG5 controls high-pass filtering of outputs, use it
  // if you'd like:
  writeRegister(L3G4200D_Address, CTRL_REG5, 0b00000000);
}

void writeRegister(int deviceAddress, byte address, byte val) {
    Wire.beginTransmission(deviceAddress); // start transmission to device 
    Wire.write(address);       // send register address
    Wire.write(val);         // send value to write
    Wire.endTransmission();     // end transmission
}

int readRegister(int deviceAddress, byte address){

    int v;
    Wire.beginTransmission(deviceAddress);
    Wire.write(address); // register to read
    Wire.endTransmission();

    Wire.requestFrom(deviceAddress, 1); // read a byte

    while(!Wire.available()) {
        // waiting
    }

    v = Wire.read();
    return v;
}
	


void mode2()
{
}


