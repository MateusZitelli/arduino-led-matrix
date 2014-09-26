#include <stdio.h>
#define MATRIX_SIZE 2

//Pin connected to latch pin (ST_CP) of 74HC595
const int latchPin = 8;
//Pin connected to clock pin (SH_CP) of 74HC595
const int clockPin = 13;
////Pin connected to Data in (DS) of 74HC595
const int dataPin = 12;

int matrix[MATRIX_SIZE];

void setup() {
  int i;
  //set pins to output because they are addressed in the main loop
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);
  Serial.begin(9600);
  Serial.println("reset");
  // Initialize matrix
  for(i = 0; i < MATRIX_SIZE; i++){
    matrix[i] = 0; 
  }
}

void loop() {
  checkSerial();
  show();
}

void checkSerial(){
  // Check the serial buffer for arrays instructions
  int i, j, line;
  // Check if there is enouth bytes in the buffer
  if(Serial.available() >= MATRIX_SIZE * MATRIX_SIZE){
    for(j = 0; j < MATRIX_SIZE; j++){
      line = 0;
      // Create an int with the first MATIX_SIZE bits representing each led in a
      // line, with states between ON (input != 0) and off (input = 0)
      for(i = 0; i < MATRIX_SIZE; i++){
        line = line << 1;
        // Subtract 48 from the byte to convert ASCII decimals representation
        // to int.
        line += (Serial.read() - 48) || 0;
      }
      // Add the loaded line to the matrix
      matrix[j] = line;
    }
  } 
}

void show(){
  // Turn on the led matrix based on the internal matrix representation.
  // The MATRIX_SIZE LSB sended to the shift register represents the lines
  // states and the next MATRIX_SIZE bits reflect the columns states.
  // To show the matrix, it's multiplexed line by line, seing that all the leds
  // in the same line are connected by their anodes and the columns by the
  // cathodes, each line is activated setting it to an off state while the
  // other lines are on. With one line activated a specific led from it could be
  // turned on just aplying current on their cathod column.
  int j;
  // An int with MATRIX_SIZE bits equal 1
  int anodesMask = 1 << (MATRIX_SIZE) - 1;
  int data;
  for(j = 0; j < MATRIX_SIZE; j++){
    // Iterate line by line
    data = 0;
    // Turn off the bit related w/ the line that will be turned on
    data = anodesMask - (1 << (j - 1));
    // Put the columns bits after the anodes ones
    data += matrix[j] << MATRIX_SIZE;
    // Turn on the line
    digitalWrite(latchPin, 0);
    shiftOut(dataPin, clockPin, data);
    digitalWrite(latchPin, 1);
  }
}

void shiftOut(int myDataPin, int myClockPin, byte myDataOut) {
  // This shifts 8 bits out MSB first,
  //on the rising edge of the clock,
  //clock idles low

  //internal function setup
  int i=0;
  int pinState;
  pinMode(myClockPin, OUTPUT);
  pinMode(myDataPin, OUTPUT);

  //clear everything out just in case to
  //prepare shift register for bit shifting
  digitalWrite(myDataPin, 0);
  digitalWrite(myClockPin, 0);

  //for each bit in the byte myDataOut�
  //NOTICE THAT WE ARE COUNTING DOWN in our for loop
  //This means that %00000001 or "1" will go through such
  //that it will be pin Q0 that lights.
  for (i=7; i>=0; i--)  {
    digitalWrite(myClockPin, 0);

    //if the value passed to myDataOut and a bitmask result
    // true then... so if we are at i=6 and our value is
    // %11010100 it would the code compares it to %01000000
    // and proceeds to set pinState to 1.
    if ( myDataOut & (1<<i) ) {
      pinState= 1;
    }
    else { 
      pinState= 0;
    }

    //Sets the pin to HIGH or LOW depending on pinState
    digitalWrite(myDataPin, pinState);
    //register shifts bits on upstroke of clock pin  
    digitalWrite(myClockPin, 1);
    //zero the data pin after shift to prevent bleed through
    digitalWrite(myDataPin, 0);
  }

  //stop shifting
  digitalWrite(myClockPin, 0);
}
