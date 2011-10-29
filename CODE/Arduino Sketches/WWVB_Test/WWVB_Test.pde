#include <LiquidCrystal.h>

/**********************************************************************
 * Clock WWV Edge Triggered  by capt.tagon
 *
 * WWVB receiver input on digital pin 2
 **********************************************************************/

#define wwvbIn      2  // WWVB receiver data input digital pin
#define ledRxPin    40  // WWVB receiver state indicator pin
#define ledFramePin 41  // Data received frame indicator pin
#define ledBitPin   42  // LED data decoded indicator pin
#define ledMarkPin  43  // Data received mark inicator pin

LiquidCrystal LCD(30,31,32,33,34,35);

// variable changed by interrupt service routine - volatile
volatile byte wwvbInState;            // store receiver signal level

byte prevWwvbInState;                 // store previous signal level
unsigned int prevEdgeMillis;          // store time signal was read
byte bitVal;                          // bit decoded 0, 1 or Mark
byte badBit;                          // bad bit, noise detected
byte prevMark;                        // store previous mark bit

void setup() {
      LCD.begin(16,2);
      pinMode(wwvbIn, INPUT);
      pinMode(ledRxPin, OUTPUT);
      pinMode(ledFramePin, OUTPUT);
      pinMode(ledBitPin, OUTPUT);
      pinMode(ledMarkPin, OUTPUT);
      attachInterrupt(0, readLevel, CHANGE); // fire interrupt on edge detected
      LCD.clear();
      LCD.home();
      LCD.print("hello");
      Serial.begin(9600);
}

void loop() {
    
      if (wwvbInState != prevWwvbInState) {
            pulseValue();
            prevWwvbInState = wwvbInState;
      }
}

/******************************************************************
 * pulseValue()
 *
 * determine pulse width 200ms = 0, 500ms = 1, 800ms = mark
 ******************************************************************/

void pulseValue() {
  unsigned int edgeMillis = millis();         // save current time
  badBit = 0;                                 // set noise counter to zero
  if (wwvbInState == 1) {                     // rising edge
    prevEdgeMillis = edgeMillis;              // set previous time to current
  } 
  else {                                    // falling edge
    int pulseLength = edgeMillis - prevEdgeMillis; // calculate pulse length millis
    if (pulseLength < 100) {                  // less than 100ms, noise pulses
      badBit = 1;
    } 
    else if (pulseLength < 400) {           // 800ms carrier drop mark
      bitVal = 2;
    } 
    else if (pulseLength < 700) {           // 500ms carrier drop one
      bitVal = 1;
    } 
    else {                                  // 200ms carrier drop zero
      bitVal = 0;
    }
    if (badBit == 0) {
      printBitVal();
    }
  }
}

/******************************************************************************
 * readLevel() {
 *
 * Pin 2 INT0 Interrupt Handler Reads pin state - flashes signal indicator LED
 ******************************************************************************/

void readLevel() {
  wwvbInState = digitalRead(wwvbIn);   // read signal level
  digitalWrite(ledRxPin, wwvbInState); // flash WWVB receiver indicator pin
}

/******************************************************************************
 * printBitVal()
 *
 * Display bit values to terminal screen, output delimited data stream with 
 * colons at mark and new line at frame start.
 ******************************************************************************/

void printBitVal() {
  if ((bitVal == 2) && (prevMark == 0)) {
    Serial.print(" : ");
    digitalWrite(ledMarkPin, HIGH);
    prevMark = 1;
  } 
  else if ((bitVal == 2) && (prevMark == 1)) {
    Serial.print("\nBit Value: ");
    Serial.print("| ");
    digitalWrite(ledFramePin, HIGH);
    prevMark = 0;
  } 
  else {
    Serial.print(bitVal, DEC);
    digitalWrite(ledMarkPin, LOW);
    digitalWrite(ledFramePin, LOW);
    if (bitVal == 0) { 
      digitalWrite(ledBitPin, LOW); 
    }
    if (bitVal == 1) { 
      digitalWrite(ledBitPin, HIGH); 
    }
    prevMark = 0; 
  }
}

/*****************************************************************************
 * Time display functions
 *****************************************************************************/

void printTime() {
  Serial.print("?x00?y0?f"); // movie cursor to line 1 char 1, clear screen
}
