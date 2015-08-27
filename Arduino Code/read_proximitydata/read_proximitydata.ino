/*

Author: Pelonomi Moiloa
        24 June 2014

Code to read proximity data from proximity registers 

Note: Sensor frequency, LDC configuration and CLK configuration register setup is
      only neccessary when needing to read frequency data

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
  
  //Start initialisation of LDC1000
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
  // end of Initiailsation
  
//Check to see if values were written to registers correctly
      /*
    // Read Rpmax
      digitalWrite(CSB, LOW);
      SPI.transfer(0x81); 
      data = SPI.transfer(0x00);
      Serial.println(data);
      digitalWrite(CSB, HIGH);
      delay(500);
    // Read Rpmin
      digitalWrite(CSB, LOW);
      SPI.transfer(0x82); 
      data = SPI.transfer(0x00);
      Serial.println(data);
      digitalWrite(CSB, HIGH);
    // Read thresh HiLSB value
      digitalWrite(CSB, LOW);
      SPI.transfer(0x86); 
      data = SPI.transfer(0x00);
      Serial.println(data);
      digitalWrite(CSB, HIGH);
      delay(500);
    // Read thresh HiMSB value
      digitalWrite(CSB, LOW);
      SPI.transfer(0x87); 
      data = SPI.transfer(0x00);
      Serial.println(data);
      digitalWrite(CSB, HIGH);
      delay(500); 
    // Read thresh LoLSB value
      digitalWrite(CSB, LOW);
      SPI.transfer(0x88); 
      data = SPI.transfer(0x00);
      Serial.println(data);
      digitalWrite(CSB, HIGH);
      delay(500); 
    // Read thresh LoMSB value
      digitalWrite(CSB, LOW);
      SPI.transfer(0x89); 
      data = SPI.transfer(0x00);
      Serial.println(data);
      digitalWrite(CSB, HIGH);
      delay(500); 
      */  
}

void loop() 
{
  unsigned int val = 0;
  unsigned int dataLSB = 0;
  unsigned int dataMSB = 0;
  unsigned int proximitydata = 0;
  
  // Read proximity data LSB register
  digitalWrite(CSB, LOW);
  SPI.transfer(0xA1); // 0x80 + 0x21
  dataLSB = SPI.transfer(0x00);
  digitalWrite(CSB, HIGH);
  delay(100);
  
  // Read proximity data MSB register
  digitalWrite(CSB, LOW);
  SPI.transfer(0xA2); // 0x80 + 0x22
  dataMSB = SPI.transfer(0x00);
  digitalWrite(CSB, HIGH);
  delay(100);
  
  proximitydata = ((unsigned int)dataMSB << 8) | (dataLSB);// combine two registers to form 16bit resolution proximity data
  Serial.println(proximitydata);
}

