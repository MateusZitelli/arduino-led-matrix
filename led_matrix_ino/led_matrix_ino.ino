#define N_BYTES 3
#define MATRIX_SIZE 10
#define TOTAL_BYTES MATRIX_SIZE * N_BYTES

const int latchPin = 8;
const int clockPin = 13;
const int dataPin = 12;


int matrix[MATRIX_SIZE][MATRIX_SIZE] = {
  {1,0,0,0,0,0,0,0,0,1},
  {0,1,0,0,0,0,0,0,1,0},
  {0,0,1,0,0,0,0,1,0,0},
  {0,0,0,1,0,0,1,0,0,0},
  {0,0,0,0,1,1,0,0,0,0},
  {0,0,0,0,1,1,0,0,0,0},
  {0,0,0,1,0,0,1,0,0,0},
  {0,0,1,0,0,0,0,1,0,0},
  {0,1,0,0,0,0,0,0,1,0},
  {1,0,0,0,0,0,0,0,0,1},
};

void setup() {
  int i,j;

  // setting pins to output because they are
  // addressed in the main loop
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);
  Serial.begin(9600);
  Serial.println("reset");
}

void loop () {
  show();
  checkSerial();
}

String intToBin(int value) {
  String str;
  String initialBinStr = String(value, BIN);
  int zeros = MATRIX_SIZE - initialBinStr.length();
  
  for (int i = 0; i < zeros; i++)
    str = str + '0'; 
  
  return str + initialBinStr;
}

int hexToInt(char * str) {
  int number = (int) strtol(str, NULL, 16);
  
  return number;
}

/**
 * Checks the Serial then mutates the matrix
 * state.
 */
void checkSerial() {
  char buffer[N_BYTES];
  String bin;
  int counter;
  int i;
  int j;
  
  if (Serial.available() >= TOTAL_BYTES) {
    j = 0;
    counter = 0;
    
    while (Serial.available()) {
      buffer[counter++] = Serial.read();
       
      if (counter >= N_BYTES) {
        bin = intToBin(hexToInt(buffer));
         
        for(i = 0; i < MATRIX_SIZE; i++) {
          matrix[j][i] = bin.charAt(i) - 48; 
          Serial.print(matrix[i][j]);
        }
        
        Serial.println();

        counter = 0;
        j++;
      }
    }
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

 /**
  * This shifts 8 bits out MSB first, on the
  * rising edge of the clock, clock idles low
  */
void shiftOut(int myDataPin, int myClockPin, int line) {
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
  for (i= MATRIX_SIZE - 1; i >= 0; i--)
    shiftBit(myDataPin, myClockPin, matrix[line][i]); 
  
  //Set the remaining lines
  for(i= MATRIX_SIZE - 2; i >= 0; i--)
    shiftBit(myDataPin, myClockPin, i != line);

  //stop shifting
  digitalWrite(myClockPin, 0);
}

/** 
 * Turns on the led matrix based on the internal
 * matrix representation (global var). The
 * MATRIX_SIZE LSB sended to the shift register
 * represents the lines states and the next
 * MATRIX_SIZE bits reflect the columns states.
 * To show the matrix, it's multiplexed line by
 * line, seing that all the leds in the same
 * line are connected by their anodes and the
 * columns by the cathodes, each line is
 * activated setting it to an off state while
 * the other lines are on. With one line
 * activated a specific led from it could be
 * turned on just aplying current on their
 * cathod column.
 */
void show(){
  // An int with MATRIX_SIZE bits equal 1
  int anodesMask = 1 << (MATRIX_SIZE) - 1;
  int i, j;
  long data;

  for(j = 0; j < MATRIX_SIZE; j++){
    digitalWrite(latchPin, 0);
    shiftOut(dataPin, clockPin, j);
    digitalWrite(latchPin, 1);
  }
}
