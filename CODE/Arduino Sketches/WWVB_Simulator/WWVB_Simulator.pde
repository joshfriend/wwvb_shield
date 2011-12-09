#include <Wire.h>

/**************************************************************************************
 * WWVB Receiver Simulator  by Capt.Tagon
 *
 * For those days when you want to work on your code, but the signal is bad, somebody
 * fired something off that spurs on 60kHz or you think you just fried your WWVB receiver.
 *
 * C-Max CMMR-6P-60 TCO Positive Output Signal Emulation
 **************************************************************************************/

#define clockOutPin 13                // data out, send this to your second
// Arduino's WWVB Rx input pin

int bitCount = 60;                    // 60 bits, one per second


// Clock output data, fiddle the zeros and ones to your heart's content,
// but the twos must remain! All bits noted as blank are ALWAYS ZERO.

// December 2 at 4:21AM
byte clockData[60] = {                // transmitted data 2 Mark, 1 or 0
  2, //  0 frame reference Pr -> *** MUST BE 2 ***
  0, //  1 minute 40                        
  1, //  2 minute 20
  0, //  3 minute 10
  0, //  4 blank
  0, //  5 minute 8
  0, //  6 minute 4
  0, //  7 minute 2
  1, //  8 minute 1
  2, //  9 mark P1 -> *** MUST BE 2 ***
  0, // 10 blank
  0, // 11 blank
  0, // 12 hours 20
  0, // 13 hours 10
  0, // 14 blank
  0, // 15 hours 8
  1, // 16 hours 4
  0, // 17 hours 2
  0, // 18 hours 1
  2, // 19 mark P2 -> *** MUST BE 2 ***
  0, // 20 blank
  0, // 21 blank
  1, // 22 day of year 200
  1, // 23 day of year 100
  0, // 24 blank
  0, // 25 day of year 80
  0, // 26 day of year 40
  1, // 27 day of year 20
  1, // 28 day of year 10
  2, // 29 mark P3 -> *** MUST BE 2 ***
  0, // 30 day of year 8
  1, // 31 day of year 4
  1, // 32 day of year 2
  0, // 33 day of year 1
  0, // 34 blank
  0, // 35 blank
  0, // 36 UTI sign +
  0, // 37 UTI sign -
  0, // 38 UTI sign +
  2, // 39 mark P4 -> *** MUST BE 2 ***
  0, // 40 UTI correction 0.8
  0, // 41 UTI correction 0.4
  0, // 42 UTI correction 0.2
  0, // 43 UTI correction 0.1
  0, // 44 blank
  0, // 45 year 80
  0, // 46 year 40
  0, // 47 year 20
  1, // 48 year 10
  2, // 49 mark P5 -> *** MUST BE 2 ***
  0, // 50 year 8
  0, // 51 year 4
  0, // 52 year 2
  1, // 53 year 1
  0, // 54 blank
  0, // 55 leapyear
  0, // 56 leapsecond
  1, // 57 dst bit 1  set 11 dst, 00 standard
  1, // 58 dst bit 2
  2, // 59 frame reference P0 -> *** MUST BE 2 ***
};


void setup() {
  Serial.begin(9600);
  
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
    
  Serial.println("\nRESET");
  Wire.begin(4);                // join i2c bus with address #4
  Wire.onReceive(receiveEvent); // register event
  pinMode(clockOutPin, OUTPUT);      // set pin digital output
}


void  loop(){
  for(int i = 0; i < bitCount; i++) {    
    if (clockData[i] == 2) { 
      genMark(); 
    } 
    else if (clockData[i] == 1) { 
      genOne(); 
    } 
    else { 
      genZero(); 
    }
    /*
    Serial.println();
    Serial.print(i);
    Serial.print("   ");
    Serial.print(clockData[i],DEC);
    */
  }
}

void receiveEvent(int howMany)
{
  //Serial.println("Recieved from PIC:");
    while(Wire.available()) // loop through all but the last
    {
        unsigned char c = Wire.read(); // receive byte as a character
        if(c == 0xFF) {
            Serial.println();
        }
        else if(c == 0xFE) {
            Serial.println("FRAME: ");
        }
        else if(c == 0xFC) {
            Serial.println("DOUBLE SYNC MARKER");
        }
        else if(c == 0xFB) {
            Serial.print("PASSED VALIDATION @ ");
            
            digitalWrite(7,HIGH);
            digitalWrite(8,HIGH);
            delay(200);
            digitalWrite(7,LOW);
            digitalWrite(8,LOW);
        }
        else if(c == 0xFA) {
            Serial.print("FAILED VALIDATION @ ");
            
            digitalWrite(9,HIGH);
            digitalWrite(10,HIGH);
            delay(200);
            digitalWrite(9,LOW);
            digitalWrite(10,LOW);
        }
        else {
            Serial.print(c,HEX);
            Serial.print(" ");
            switch(c) {
                case 0:
                    digitalWrite(7, HIGH);
                    delay(200);
                    digitalWrite(7,LOW);
                    break;
                case 1:
                    digitalWrite(8, HIGH);
                    delay(200);
                    digitalWrite(8,LOW);
                    break;
                case 2:
                    digitalWrite(9, HIGH);
                    delay(200);
                    digitalWrite(9,LOW);
                    break;
                case 3:
                    digitalWrite(10, HIGH);
                    delay(200);
                    digitalWrite(10,LOW);
                    break;
            }
        }
    }
}

void genMark() {                      //generate 800ms Mark
  digitalWrite(clockOutPin, LOW);
  delay(800);
  digitalWrite(clockOutPin, HIGH);
  delay(200);
}

void genZero() {                      // generate 200ms Zero
  digitalWrite(clockOutPin, LOW);
  delay(200);
  digitalWrite(clockOutPin, HIGH);
  delay(800);
}

void genOne() {                       // generate 500ms One
  digitalWrite(clockOutPin, LOW);
  delay(500);
  digitalWrite(clockOutPin, HIGH);
  delay(500);
}
