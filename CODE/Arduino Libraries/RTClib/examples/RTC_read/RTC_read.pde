// Date and time functions using a MCP79410N RTC connected via I2C and Wire lib

#include <Wire.h>
#include "RTClib.h"

RTC Clock;

void setup () {
    Serial.begin(9600);
    Wire.begin();
    Clock.begin();
    Clock.set_vbaten(0x01);
    //Clock.adjust(DateTime(__DATE__, __TIME__));
}

void loop () {
    
    DateTime now = Clock.now();
    
    Serial.println("CURRENT RTC TIME");
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
    
    //Example loop showing how various UTC offsets will be printned
    //UTC +/-13 are shown to indicate how invalid offsets will be handled
    for(int i = -13; i <= 13; i++){
        DateTime temp = now.tz_adjust(i, now.unixtime());
    
        Serial.print("Timezone adjusted UTC ");
        Serial.println(i);
        Serial.print(temp.year(), DEC);
        Serial.print('/');
        Serial.print(temp.month(), DEC);
        Serial.print('/');
        Serial.print(temp.day(), DEC);
        Serial.print(' ');
        Serial.print(temp.hour(), DEC);
        Serial.print(':');
        Serial.print(temp.minute(), DEC);
        Serial.print(':');
        Serial.print(temp.second(), DEC);
        Serial.println();
    }
    Serial.println("\n");
        
    delay(1000);
}
