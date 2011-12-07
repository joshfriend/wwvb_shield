// Code by JeeLabs http://news.jeelabs.org/code/
// Released to the public domain! Enjoy!

//Modified for use with the MCP79410 by Eric Born and Josh Friend

#include <Wire.h>
#include <avr/pgmspace.h>
#include "RTClib.h"

#define RTC_ADDRESS 0x6F
#define SECONDS_PER_DAY 86400L

#define SECONDS_FROM_1970_TO_2000 946684800

#if ARDUINO > 22
#include <arduino.h>
#endif

#if ARDUINO <= 22
#include <WProgram.h>
#endif

////////////////////////////////////////////////////////////////////////////////
// utility code, some of this could be exposed in the DateTime API if needed

static uint8_t daysInMonth [] PROGMEM = { 31,28,31,30,31,30,31,31,30,31,30,31 };

// number of days since 2000/01/01, valid for 2001..2099
static uint16_t date2days(uint16_t y, uint8_t m, uint8_t d) {
    if (y >= 2000)
        y -= 2000;
    uint16_t days = d;
    for (uint8_t i = 1; i < m; ++i)
        days += pgm_read_byte(daysInMonth + i - 1);
    if (m > 2 && y % 4 == 0)
        ++days;
    return days + 365 * y + (y + 3) / 4 - 1;
}

static long time2long(uint16_t days, uint8_t h, uint8_t m, uint8_t s) {
    return ((days * 24L + h) * 60 + m) * 60 + s;
}

////////////////////////////////////////////////////////////////////////////////
// DateTime implementation - ignores time zones and DST changes
// NOTE: also ignores leap seconds, see http://en.wikipedia.org/wiki/Leap_second

DateTime::DateTime (uint32_t t) {
  t -= SECONDS_FROM_1970_TO_2000;    // bring to 2000 timestamp from 1970

    ss = t % 60;
    t /= 60;
    mm = t % 60;
    t /= 60;
    hh = t % 24;
    uint16_t days = t / 24;
    uint8_t leap;
    for (yOff = 0; ; ++yOff) {
        leap = yOff % 4 == 0;
        if (days < 365 + leap)
            break;
        days -= 365 + leap;
    }
    for (m = 1; ; ++m) {
        uint8_t daysPerMonth = pgm_read_byte(daysInMonth + m - 1);
        if (leap && m == 2)
            ++daysPerMonth;
        if (days < daysPerMonth)
            break;
        days -= daysPerMonth;
    }
    d = days + 1;
}

DateTime::DateTime (uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t min, uint8_t sec) {
    if (year >= 2000)
        year -= 2000;
    yOff = year;
    m = month;
    d = day;
    hh = hour;
    mm = min;
    ss = sec;
}

static uint8_t conv2d(const char* p) {
    uint8_t v = 0;
    if ('0' <= *p && *p <= '9')
        v = *p - '0';
    return 10 * v + *++p - '0';
}

// A convenient constructor for using "the compiler's time":
//   DateTime now (__DATE__, __TIME__);
// NOTE: using PSTR would further reduce the RAM footprint
DateTime::DateTime (const char* date, const char* time) {
    // sample input: date = "Dec 26 2009", time = "12:34:56"
    yOff = conv2d(date + 9);
    // Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec 
    switch (date[0]) {
        case 'J': m = date[1] == 'a' ? 1 : m = date[2] == 'n' ? 6 : 7; break;
        case 'F': m = 2; break;
        case 'A': m = date[2] == 'r' ? 4 : 8; break;
        case 'M': m = date[2] == 'r' ? 3 : 5; break;
        case 'S': m = 9; break;
        case 'O': m = 10; break;
        case 'N': m = 11; break;
        case 'D': m = 12; break;
    }
    d = conv2d(date + 4);
    hh = conv2d(time);
    mm = conv2d(time + 3);
    ss = conv2d(time + 6);
}

uint8_t DateTime::dayOfWeek() const {    
    uint16_t day = date2days(yOff, m, d);
    return (day + 6) % 7; // Jan 1, 2000 is a Saturday, i.e. returns 6
}

uint32_t DateTime::unixtime(void) const {
  uint32_t t;
  uint16_t days = date2days(yOff, m, d);
  t = time2long(days, hh, mm, ss);
  t += SECONDS_FROM_1970_TO_2000;  // seconds from 1970 to 2000

  return t;
}

////////////////////////////////////////////////////////////////////////////////
// RTC implementation

static uint8_t bcd2bin (uint8_t val) { return val - 6 * (val >> 4); }
static uint8_t bin2bcd (uint8_t val) { return val + 6 * (val / 10); }

uint8_t vbaten = 1;

uint8_t RTC::begin(void) {
    vbaten = 1;
    return 1;
}


#if ARDUINO > 22


uint8_t RTC::isrunning(void) {
    Wire.beginTransmission(RTC_ADDRESS);
    Wire.write(0x03);	
    Wire.endTransmission();
  
    Wire.requestFrom(RTC_ADDRESS, 1);
    uint8_t ss = Wire.read() & (1<<5);
    return !(ss>>5);
}

void RTC::set_vbaten(uint8_t state) {
    if(state == 1 || state == 0) {
        vbaten = state;
        
        Wire.beginTransmission(RTC_ADDRESS);
        Wire.write(0x03);
        Wire.write((vbaten<<3) | (1<<4));
        Wire.endTransmission();
    }
}

void RTC::adjust(const DateTime& dt) {
    Wire.beginTransmission(RTC_ADDRESS);
    Wire.write((int)0);
    Wire.write(bin2bcd(dt.second()) | 0x80);
    Wire.write(bin2bcd(dt.minute()));
    Wire.write(bin2bcd(dt.hour()));
    Wire.write((vbaten<<3) | (1<<4));
    Wire.write(bin2bcd(dt.day()));
    Wire.write(bin2bcd(dt.month()));
    Wire.write(bin2bcd(dt.year() - 2000));
    Wire.write((int)0);
    Wire.endTransmission();
}

DateTime RTC::now() {
    Wire.beginTransmission(RTC_ADDRESS);
    Wire.write((int)0);	
    Wire.endTransmission();
    
    Wire.requestFrom(RTC_ADDRESS, 7);
    uint8_t ss = bcd2bin(Wire.read() & 0x7F);
    uint8_t mm = bcd2bin(Wire.read());
    uint8_t hh = bcd2bin(Wire.read());
    Wire.read();
    uint8_t d = bcd2bin(Wire.read());
    uint8_t m = bcd2bin(Wire.read());
    uint16_t y = bcd2bin(Wire.read()) + 2000;
    
    return DateTime (y, m, d, hh, mm, ss);
}

#endif

#if ARDUINO <= 22

uint8_t RTC::isrunning(void) {
    Wire.beginTransmission(RTC_ADDRESS);
    //OSCON bit is in register 0x03
    Wire.send(0x03);	
    Wire.endTransmission();
  
    Wire.requestFrom(RTC_ADDRESS, 1);
    //Mask off OSCON bit
    uint8_t oscon = Wire.receive() & (1<<5);
    return (oscon>>5);
}

void RTC::set_vbaten(uint8_t state) {
    //Check for invlaid values
    if(state == 1 || state == 0) {
        vbaten = state;
        
        Wire.beginTransmission(RTC_ADDRESS);
        //Bit to enable battery backup is in register 0x03
        Wire.send(0x03);
        //Set the VBATEN bit to desired value and dont clear VBAT flag
        Wire.send((vbaten<<3) | (1<<4));
        Wire.endTransmission();
    }
}

void RTC::adjust(const DateTime& dt) {
    Wire.beginTransmission(RTC_ADDRESS);
    //Wire library requires int for 0 address
    Wire.send((int)0);
    Wire.send(bin2bcd(dt.second()) | 0x80);
    Wire.send(bin2bcd(dt.minute()));
    Wire.send(bin2bcd(dt.hour()));
    //Set the VBATEN bit to desired value and dont clear VBAT flag
    Wire.send((vbaten<<3) | (1<<4));
    Wire.send(bin2bcd(dt.day()));
    Wire.send(bin2bcd(dt.month()));
    Wire.send(bin2bcd(dt.year() - 2000));
    //Disable ALL options in control register
    Wire.send((int)0);
    Wire.endTransmission();
}

DateTime RTC::now() {
    Wire.beginTransmission(RTC_ADDRESS);
    Wire.send((int)0);	
    Wire.endTransmission();
    
    Wire.requestFrom(RTC_ADDRESS, 7);
    //Mask on seconds removes ST bit from number of seconds
    uint8_t ss = bcd2bin(Wire.receive() & 0x7F);
    uint8_t mm = bcd2bin(Wire.receive());
    uint8_t hh = bcd2bin(Wire.receive());
    //Ignore day of week
    Wire.receive();
    uint8_t d = bcd2bin(Wire.receive());
    uint8_t m = bcd2bin(Wire.receive());
    uint16_t y = bcd2bin(Wire.receive()) + 2000;
    
    return DateTime (y, m, d, hh, mm, ss);
}

#endif
