#include <Wire.h>
#include "RTClib.h"

RTC Clock;

void setup() {
    Serial.begin(9600);
    Serial3.begin(9600);
    
    Clock.begin();
    Clock.set_vbaten(0x01);
    
    pinMode(7, OUTPUT);
    pinMode(8, OUTPUT);
    pinMode(9, OUTPUT);
    pinMode(10, OUTPUT);
    
    Serial.println("\nRESET");
    Serial3.println("\nRESET");
    
    Wire.begin(4);                // join i2c bus with address #4
    Wire.onReceive(receiveEvent); // register event
}


void  loop(){
    delay(100);
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany)
{
  //Serial.println("Recieved from PIC:");
    while(Wire.available()) // loop through all but the last
    {
        unsigned char c = Wire.receive(); // receive byte as a character
        if(c == 0xFF) {
            Serial.println();
            Serial3.println();
        }
        else if(c == 0xFE) {
            Serial.println("FRAME: ");
            Serial3.println("FRAME: ");
        }
        else if(c == 0xFC) {
            Serial.println("DOUBLE SYNC MARKER");
            Serial3.println("DOUBLE SYNC MARKER");
        }
        else {
            Serial.print(c,HEX);
            Serial.print(" ");
            Serial3.print(c,HEX);
            Serial3.print(" ");
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
