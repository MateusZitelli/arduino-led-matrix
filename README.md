# arduino-led-matrix

> Simple Arduino program to control led matrices based on serial data.

There's a video where we are using it to display the [snake game](http://github.com/cirocosta/matrix-snake) that is running on a [node-webkit application](http://github.com/cirocosta/ledmatrix). Check it out!

[![Video](http://i.imgur.com/AMxTHSA.png)](https://www.youtube.com/watch?v=m-w2wwzHoVA)

## Using it

1. Download Arduino software

2. Open the `led_matrix_ino/led_matrix_ino.ino` file in the IDE, click `verify` then `upload`. This will flash our program to arduino so that it will be able to interpret any Hexadecimal representation of a 10x10 matrix.


## How it works

Its functionality is pretty straightforward: there's a loop and in this loop the program will do two things, firstly it will check the serial to see if there's new information comming, then, if there is, it will decode the hexadecimal representation and update the internal matrix representation. After this step occurs (remembering, only if there's new info comming from the serial), it will run the `show()` procedure, which os what will actually draw the matrix representation with the LEDs.

### .checkSerial

As arduino imposes a limit regarding the number of bytes that it can receive in a given step of the main loop we can't "just" send the entire matrix as a full string of 0s and 1s (that would be a total of 100 chars). We decided, then, to take another approach: rely on the fact that the sender is going to send a 10*(3byte) string encoded in hexadecimal that represents the matrix and then in the arduino we just decode it and update the internal representation (an array of arrays).


### .show()

For showing (i.e, lightining the leds that correspond to a 1 in the internal matrix representation) we decided to use the technique of multiplexing.

The idea of multiplexing is to send voltage to only a row of LEDs at any given time so that we are able to keep with a great degree of light without needing a power providing. By flashing more than one row faster than what our eyes are able to interpret we are able then to draw any shape, which is what we want.

## Resources

The resources for recreating the ledmatrix is very similar to what is presented in [Multiplexing with Arduino and the 74HC595](http://www.instructables.com/id/Multiplexing-with-Arduino-and-the-74HC595/step2/How-does-the-74HC595-work/) which covers in a step-by-step basis how to get a 4x4 led matrix to work.

