#include <stdio.h>
#define MATRIX_SIZE 10

//Pin connected to latch pin (ST_CP) of 74HC595
const int latchPin = 8;
//Pin connected to clock pin (SH_CP) of 74HC595
const int clockPin = 13;
////Pin connected to Data in (DS) of 74HC595
const int dataPin = 12;

int matrix[MATRIX_SIZE][MATRIX_SIZE];

void setup() {
  int i,j;
  //set pins to output because they are addressed in the main loop
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);
  Serial.begin(9600);
  Serial.println("reset");
  // Initialize matrix
  for(j = 0; j < MATRIX_SIZE; j++){
    for(i = 0; i < MATRIX_SIZE; i++){
      matrix[j][i] = (i == j || i == MATRIX_SIZE - j - 1);
    }
  }
}

void loop() {
  show();
  checkSerial();
}

void checkSerial(){
  // Check in the serial buffer for 100 bytes to describe the matrix
  int i, j;
  int column = 0;
  int line = 0;
  int readSerial = 1;
  byte dataByte;
  // 13 bytes to store 100 bits
  if(Serial.available() >= 13){
    while(readSerial){
      dataByte = Serial.read();
      Serial.write(dataByte);
      for(i = 0; i < 8; i++){
        matrix[line][column++] = dataByte & (1 << i);
        if(column >= MATRIX_SIZE){
          column = 0;
          line++;
          if(line >= MATRIX_SIZE){
            readSerial = 0;
            break; 
          }
        }
      }
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
  int i, j;
  // An int with MATRIX_SIZE bits equal 1
  int anodesMask = 1 << (MATRIX_SIZE) - 1;
  long data;
  for(j = 0; j < MATRIX_SIZE; j++){
    digitalWrite(latchPin, 0);
    shiftOut(dataPin, clockPin, j);
    digitalWrite(latchPin, 1);
  }
}

void shiftBit(int myDataPin, int myClockPin, byte b){
  digitalWrite(myClockPin, 0);
  //Sets the pin to HIGH or LOW depending on bit
  digitalWrite(myDataPin, b); 
  //register shifts bits on upstroke of clock pin  
  digitalWrite(myClockPin, 1);
  //zero the data pin after shift to prevent bleed through
  digitalWrite(myDataPin, 0);
}

void shiftOut(int myDataPin, int myClockPin, int line) {
  // This shifts 8 bits out MSB first,
  //on the rising edge of the clock,
  //clock idles low

  //internal function setup
  int i=0;
  int pinState;
  pinMode(myClockPin, OUTPUT);
  pinMode(myDataPin, OUTPUT);
  
  // Set the 10th line state - because of a mistake in the
  // circuit board the first bit is always sent to define
  // the 10th line.
  shiftBit(myDataPin, myClockPin, line != 9);
  
  // Set the columns states
  for (i= MATRIX_SIZE - 1; i >= 0; i--)  {
    shiftBit(myDataPin, myClockPin, matrix[line][i]); 
  }
  
  //Set the remaining lines
  for(i= MATRIX_SIZE - 2; i >= 0; i--){
    shiftBit(myDataPin, myClockPin, i != line);
  }

  //stop shifting
  digitalWrite(myClockPin, 0);
}
