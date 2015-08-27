/*

Author: Pelonomi Moiloa
        24 June 2014


*/

#include "SPI.h" // include arduino SPI library

const int CSB = 10; // chip select bit for Arduino Uno


void setup() 
{
  unsigned int data = 0;
  Serial.begin(9600);
  // start SPI library/ activate BUS
  SPI.begin();
 
  pinMode(CSB, OUTPUT); 
 SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0); // CPOL = 0 and CPH = 0 mode 3 also works
  SPI.setClockDivider(SPI_CLOCK_DIV4); // set SCLK @ 4MHz, LDC1000 max is 4MHz DIV2 also works
  
    // set power mode to idle to configure stuff
  digitalWrite(CSB, LOW);
  SPI.transfer(0x0B);
  SPI.transfer(0x00);
  digitalWrite(CSB, HIGH);
  delay(100);

 // Set RpMax
  digitalWrite(CSB, LOW);
  SPI.transfer(0x01);
  SPI.transfer(0x0E);
  digitalWrite(CSB, HIGH);
  delay(100);
  // Set RpMin
  digitalWrite(CSB, LOW);
  SPI.transfer(0x02);
  SPI.transfer(0x3B);
  digitalWrite(CSB, HIGH);
  delay(100);
  
   // Set Sensor frequency
  digitalWrite(CSB, LOW);
  SPI.transfer(0x03);
  SPI.transfer(0x94);
  digitalWrite(CSB, HIGH);
  delay(100);
  
   // Set LDC configurationn
  digitalWrite(CSB, LOW);
  SPI.transfer(0x04);
  SPI.transfer(0x17);
  digitalWrite(CSB, HIGH);
  delay(100);
  
   // Set clock configuration
  digitalWrite(CSB, LOW);
  SPI.transfer(0x05);
  SPI.transfer(0x00);
  digitalWrite(CSB, HIGH);
  delay(100);
  
  
  // disable all interrupt modes
  digitalWrite(CSB, LOW);
  SPI.transfer(0x0A);
  SPI.transfer(0x00);
  digitalWrite(CSB, HIGH);
  // set thresh HiLSB value
  digitalWrite(CSB, LOW);
  SPI.transfer(0x06);
  SPI.transfer(0x50);
  digitalWrite(CSB, HIGH);
  delay(100);
  // set thresh HiMSB value
  digitalWrite(CSB, LOW);
  SPI.transfer(0x07);
  SPI.transfer(0x14);
  digitalWrite(CSB, HIGH);
  delay(100);
  // set thresh LoLSB value
  digitalWrite(CSB, LOW);
  SPI.transfer(0x08);
  SPI.transfer(0xC0);
  digitalWrite(CSB, HIGH);
  delay(100);
  // set thresh LoMSB value
  digitalWrite(CSB, LOW);
  SPI.transfer(0x09);
  SPI.transfer(0x12);
  digitalWrite(CSB, HIGH);
  delay(100);
  
  // set power mode to active mode
  digitalWrite(CSB, LOW);
  SPI.transfer(0x0B);
  SPI.transfer(0x01);
  digitalWrite(CSB, HIGH);
  delay(100);
  
  /*
// Read Rpmax
  digitalWrite(CSB, LOW);
  SPI.transfer(0x81); 
  data = SPI.transfer(0x00);
  Serial.println(data);
  digitalWrite(CSB, HIGH);
  delay(200);
// Read Rpmin
  digitalWrite(CSB, LOW);
  SPI.transfer(0x82); 
  data = SPI.transfer(0x00);
  Serial.println(data);
  digitalWrite(CSB, HIGH);
  delay(200); 
// Read thresh HiLSB value
  digitalWrite(CSB, LOW);
  SPI.transfer(0x86); 
  data = SPI.transfer(0x00);
  Serial.println(data);
  digitalWrite(CSB, HIGH);
  delay(200);
// Read thresh HiMSB value
  digitalWrite(CSB, LOW);
  SPI.transfer(0x87); 
  data = SPI.transfer(0x00);
  Serial.println(data);
  digitalWrite(CSB, HIGH);
  delay(200); 
// Read thresh LoLSB value
  digitalWrite(CSB, LOW);
  SPI.transfer(0x88); 
  data = SPI.transfer(0x00);
  Serial.println(data);
  digitalWrite(CSB, HIGH);
  delay(200); 
// Read thresh LoMSB value
  digitalWrite(CSB, LOW);
  SPI.transfer(0x89); 
  data = SPI.transfer(0x00);
  Serial.println(data);
  digitalWrite(CSB, HIGH);
  delay(200); 
  */ 
}

void loop() 
{
  unsigned int val = 0;
  unsigned int dataLSB = 0;
  unsigned int dataMSB = 0;
  
  unsigned int datafLSB = 0;
  unsigned int dataMidB = 0;
  unsigned int datafMSB = 0;
  unsigned int proximityData = 0;
  unsigned long frequencyData = 0; //long to store 24bit long number
  byte zeros = 0b00000000;
  
  // Read proximity data LSB register
  digitalWrite(CSB, LOW);
  SPI.transfer(0xA1); 
  dataLSB = SPI.transfer(0x00);
  //Serial.println(dataLSB);
  digitalWrite(CSB, HIGH);
  delay(100);
  
  // Read proximity data MSB register
  digitalWrite(CSB, LOW);
  SPI.transfer(0xA2); 
  dataMSB = SPI.transfer(0x00);
  //Serial.println(dataMSB);
  digitalWrite(CSB, HIGH);
  delay(100);
  
  // Read frequency data LSB register
  digitalWrite(CSB, LOW);
  SPI.transfer(0xa3); 
  datafLSB = SPI.transfer(0x00);
  digitalWrite(CSB, HIGH);
  delay(100);
  
  // Read frequency data MidB register
  digitalWrite(CSB, LOW);
  SPI.transfer(0xa4); 
  dataMidB = SPI.transfer(0x00);
  digitalWrite(CSB, HIGH);
  delay(100);
  
  // Read frequency data MSB register
  digitalWrite(CSB, LOW);
  SPI.transfer(0xa5); 
  datafMSB = SPI.transfer(0x00);
  digitalWrite(CSB, HIGH);
  delay(100);
  
  proximityData = ((unsigned int)dataMSB << 8) | (dataLSB);
  Serial.println(proximityData);
  frequencyData = (zeros << 8)|(datafMSB);
  frequencyData = (frequencyData << 8) | (dataMidB);
  frequencyData = (frequencyData << 8)| (datafLSB);
  Serial.println(frequencyData);
  //Serial.println(datafLSB);
  //Serial.println(dataMidB);
  //Serial.println(datafMSB);
}

