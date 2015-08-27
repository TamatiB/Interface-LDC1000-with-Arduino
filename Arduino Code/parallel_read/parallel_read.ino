/*

Author: Pelonomi Moiloa
        24 June 2014

Code to read proximity data from proximity registers 

Note: Sensor frequency, LDC configuration and CLK configuration register setup is
      only neccessary when needing to read frequency data

*/

#include "SPI.h" // include arduino SPI library

const int CSB = 10; // chip select bit for Arduino Uno
const int CSB2 = 4;
const int sensor_1 = 8;
const int sensor_2 = 2;
unsigned int sensor1_max = 0;
unsigned int sensor2_max = 0;
unsigned int val1 = 0;
unsigned int val2 = 0;


void setup() 
{
  unsigned int data = 0;
  Serial.begin(9600);
  // start SPI library/ activate BUS
  SPI.begin();
 
  pinMode(CSB, OUTPUT);
  pinMode(CSB2, OUTPUT);
  pinMode(sensor_1, OUTPUT);
  pinMode(sensor_2, OUTPUT);
    
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
    
    //Calibrate for 5 seconds
    //sensor 1 calibration
  while (millis()<5000)
  { 
    // Read proximity data LSB register
      digitalWrite(CSB, LOW);
      SPI.transfer(0xA1); // 0x80 + 0x21
      unsigned int dataLSB = SPI.transfer(0x00);
      digitalWrite(CSB, HIGH);
      //delay(5);
      
      // Read proximity data MSB register
      digitalWrite(CSB, LOW);
      SPI.transfer(0xA2); // 0x80 + 0x22
      unsigned int dataMSB = SPI.transfer(0x00);
      digitalWrite(CSB, HIGH);
      delay(5);
      
      val1 = (((unsigned int)dataMSB << 8) | (dataLSB));// combine two registers to form 16bit resolution proximity data
        
        if (val1 > sensor1_max)
        {
          sensor1_max = val1;
        }
    //sensor 2 calibration    
   // Read proximity data LSB register
      digitalWrite(CSB2, LOW);
      SPI.transfer(0xA1); // 0x80 + 0x21
      dataLSB = SPI.transfer(0x00);
      digitalWrite(CSB2, HIGH);
      //delay(5);
      
      // Read proximity data MSB register
      digitalWrite(CSB2, LOW);
      SPI.transfer(0xA2); // 0x80 + 0x22
      dataMSB = SPI.transfer(0x00);
      digitalWrite(CSB2, HIGH);
      delay(5);
      
      val2 = (((unsigned int)dataMSB << 8) | (dataLSB));// combine two registers to form 16bit resolution proximity data    
      
        if (val2 > sensor2_max)
      {
        sensor2_max = val2;
      }  
  }
  //print calibrated values
  Serial.print("Sensor 1 Calibrated to: ");
  Serial.println(sensor1_max);
  Serial.print("Sensor 2 Calibrated to: ");
  Serial.println(sensor2_max);
    
}


void loop() 
{
  digitalWrite(sensor_1, LOW);
  digitalWrite(sensor_2, LOW);
  unsigned long temp1 = 0;
  unsigned long temp2 = 0;
 
 for (int i = 0; i < 10; i ++)
 { 
      // Read proximity data LSB register
      digitalWrite(CSB, LOW);
      SPI.transfer(0xA1); // 0x80 + 0x21
      unsigned int dataLSB = SPI.transfer(0x00);
      digitalWrite(CSB, HIGH);
      //delay(5);
      
      // Read proximity data MSB register
      digitalWrite(CSB, LOW);
      SPI.transfer(0xA2); // 0x80 + 0x22
      unsigned int dataMSB = SPI.transfer(0x00);
      digitalWrite(CSB, HIGH);
      delay(5);
      
      val1 = (((unsigned int)dataMSB << 8) | (dataLSB));// combine two registers to form 16bit resolution proximity data
      temp1 = temp1 + val1;
      //Serial.println(val1);
      
      // Read proximity data LSB register
      digitalWrite(CSB2, LOW);
      SPI.transfer(0xA1); // 0x80 + 0x21
      dataLSB = SPI.transfer(0x00);
      digitalWrite(CSB2, HIGH);
      //delay(5);
      
      // Read proximity data MSB register
      digitalWrite(CSB2, LOW);
      SPI.transfer(0xA2); // 0x80 + 0x22
      dataMSB = SPI.transfer(0x00);
      digitalWrite(CSB2, HIGH);
      delay(5);
      
      val2 = (((unsigned int)dataMSB << 8) | (dataLSB));// combine two registers to form 16bit resolution proximity data    
      temp2 = temp2 + val2;
      //Serial.println(val2);
      
      delay(5);
 }
 
 unsigned int proximitydata1 = temp1/10;
 unsigned int proximitydata2 = temp2/10;
 
 if (proximitydata1 > sensor1_max)
      {
      digitalWrite(sensor_1, HIGH);
      Serial.println("Sensor 1 obstruction");
      }
      Serial.println(proximitydata1);
 if (proximitydata2 > sensor2_max)
      {
      digitalWrite(sensor_2, HIGH);
      Serial.println("Sensor 2 obstruction");
      }
      delay(50);
      Serial.println(proximitydata2);
}

