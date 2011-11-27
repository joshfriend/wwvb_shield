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

// November 30 at 4:21AM
byte clockData[60] = {                // transmitted data 2 Mark, 1 or 0
  2, //  0 frame reference Pr -> *** MUST BE 2 ***
  0, //  1 minute 40                        
  0, //  2 minute 20
  1, //  3 minute 10
  0, //  4 blank
  0, //  5 minute 8
  0, //  6 minute 4
  1, //  7 minute 2
  0, //  8 minute 1
  2, //  9 mark P1 -> *** MUST BE 2 ***
  0, // 10 blank
  0, // 11 blank
  0, // 12 hours 20
  0, // 13 hours 10
  0, // 14 blank
  0, // 15 hours 8
  1, // 16 hours 4
  0, // 17 hours 2
  1, // 18 hours 1
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
  0, // 32 day of year 2
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
  Serial.println("RESET");
  Wire.begin(4);                // join i2c bus with address #4
  Wire.onReceive(receiveEvent); // register event
  pinMode(clockOutPin, OUTPUT);      // set pin digital output
  pinMode(22,INPUT);
}


void  loop(){
  Serial.println("\nSTART OF FRAME");
  for(int i = 0; i < bitCount; i++) {
    if(digitalRead(22) == HIGH){
        while(digitalRead(22) == HIGH);
        i = 56;
    }
        
    if (clockData[i] == 2) { 
      genMark(); 
    } 
    else if (clockData[i] == 1) { 
      genOne(); 
    } 
    else { 
      genZero(); 
    }
  }
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany)
{
  while(Wire.available()) // loop through all but the last
  {
    unsigned char c = Wire.receive(); // receive byte as a character
    
    switch(c) {
        case 0:
            Serial.println("\t0");
            break;
        case 1:
            Serial.println("\t1");
            break;
        case 2:
            Serial.println("\t-");
            break;
        case 0xFF:
            Serial.println("\nMAIN LOOP VALID FRAME FLAG");
            break;
        case 0xFD:
            Serial.println("\nMAIN LOOP FRAME RECIEVED FLAG");
            break;
        case 0xFC:
            Serial.println("\nPOSITION 59");
            break;
        default:
            Serial.print(c-3);
    }
   
  }
}

void genMark() {                      //generate 800ms Mark
  digitalWrite(clockOutPin, LOW);
  delay(80);
  digitalWrite(clockOutPin, HIGH);
  delay(20);
}

void genZero() {                      // generate 200ms Zero
  digitalWrite(clockOutPin, LOW);
  delay(20);
  digitalWrite(clockOutPin, HIGH);
  delay(80);
}

void genOne() {                       // generate 500ms One
  digitalWrite(clockOutPin, LOW);
  delay(50);
  digitalWrite(clockOutPin, HIGH);
  delay(50);
}
