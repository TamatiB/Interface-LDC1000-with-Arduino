/*

Author: Pelonomi Moiloa
Date:26 June 2014

This code sets up an external clock (square wave)at 8MHz for connection to the 
LDCLK pin of the LDC1000

This code works both with an Arduino Mega and Arduino Uno

The clock is set up using Timer2

Refer to data sheet...

  REGISTERS:
  OC2A := wave output pin
  OCR2A and OCR2B 
      Timer2 output compare registers
  TCCR2A := FOC2A    FOC2B    -        -        WGM22    CS22    CS21     CS20
  TCCR2B := COM2A1   COM2A0   COM2B1   COM2B0   -        -       WGM21    WGM20
      WGM -> control timer mode
      CS  -> control clock prescaler (divide 16Hz by prescaler to get desired clock frequency)
      COM2A/B -> enable/disable/invert output A and B respectively
    
We need WGM := 010 to select CTC mode (clear timer on comparator match)
        CS  := 001 technically we want clk/2 but by trial and error clk/1 gives us 8MHz
        COM2A/B := 10 non-inverted PWM input 
 
 */
 
// OC2A ouput for arduino Uno = pin 11/ Mega = pin 10
const int pwm8 = 10;

void setup() {                
  // initialize the digital pin as an output.
  pinMode(pwm8, OUTPUT);   
  TCCR2A = ((1<<WGM21) | (1<<COM2A0)); //set WGM to CTC mode and enable output A
  TCCR2B = (1 << CS20);// set prescaler to 1
  TIMSK2 = 0;// disable timer interrupt just in case
}

void loop() {
  Serial.begin(9600);
}
