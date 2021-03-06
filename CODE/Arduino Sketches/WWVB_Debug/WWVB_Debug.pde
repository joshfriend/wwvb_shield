#include <Wire.h>
#include "RTClib.h"

RTC Clock;

DateTime now;

void setup() {
    Serial.begin(9600);
    Serial3.begin(9600);
    
    Clock.begin();
    Clock.set_vbaten(0x01);
    
    //Clock.adjust(DateTime(__DATE__, __TIME__));
    
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
    now = Clock.now();
    delay(5000);
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany)
{
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
        else if(c == 0xFB) {
            Serial.print("PASSED VALIDATION @ ");
            Serial3.print("PASSED VALIDATION @ ");
    
            Serial.print(now.year(), DEC);
            Serial.print('/');
            Serial.print(now.month(), DEC);
            Serial.print('/');
            Serial.print(now.day(), DEC);
            Serial.print(' ');
            Serial.print(now.hour(), DEC);
            Serial.print(':');
            Serial.print(now.minute(), DEC);
            Serial.println();

            Serial3.print(now.year(), DEC);
            Serial3.print('/');
            Serial3.print(now.month(), DEC);
            Serial3.print('/');
            Serial3.print(now.day(), DEC);
            Serial3.print(' ');
            Serial3.print(now.hour(), DEC);
            Serial3.print(':');
            Serial3.print(now.minute(), DEC);
            Serial3.println();
            
            digitalWrite(7,HIGH);
            digitalWrite(8,HIGH);
            delay(200);
            digitalWrite(7,LOW);
            digitalWrite(8,LOW);
        }
        else if(c == 0xFA) {
            Serial.print("FAILED VALIDATION @ ");
            Serial3.print("FAILED VALIDATION @ ");
            
                
            Serial.print(now.year(), DEC);
            Serial.print('/');
            Serial.print(now.month(), DEC);
            Serial.print('/');
            Serial.print(now.day(), DEC);
            Serial.print(' ');
            Serial.print(now.hour(), DEC);
            Serial.print(':');
            Serial.print(now.minute(), DEC);
            Serial.println();

            Serial3.print(now.year(), DEC);
            Serial3.print('/');
            Serial3.print(now.month(), DEC);
            Serial3.print('/');
            Serial3.print(now.day(), DEC);
            Serial3.print(' ');
            Serial3.print(now.hour(), DEC);
            Serial3.print(':');
            Serial3.print(now.minute(), DEC);
            Serial3.println();
            
            
            digitalWrite(9,HIGH);
            digitalWrite(10,HIGH);
            delay(200);
            digitalWrite(9,LOW);
            digitalWrite(10,LOW);
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
