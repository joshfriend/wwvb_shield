/*------------------------------------------------------------------------------
 *  ______     __  __     ______     ______     __   __     ______
 * /\  ___\   /\ \_\ \   /\  == \   /\  __ \   /\ "-.\ \   /\  __ \
 * \ \ \____  \ \  __ \  \ \  __<   \ \ \/\ \  \ \ \-.  \  \ \ \/\ \
 *  \ \_____\  \ \_\ \_\  \ \_\ \_\  \ \_____\  \ \_\\"\_\  \ \_____\
 *   \/_____/   \/_/\/_/   \/_/ /_/   \/_____/   \/_/ \/_/   \/_____/
 *          ______   ______     __    __     __     ______
 *         /\__  _\ /\  __ \   /\ "-./  \   /\ \   /\  ___\
 *         \/_/\ \/ \ \ \/\ \  \ \ \-./\ \  \ \ \  \ \ \____
 *            \ \_\  \ \_____\  \ \_\ \ \_\  \ \_\  \ \_____\
 *             \/_/   \/_____/   \/_/  \/_/   \/_/   \/_____/
 *
 * --- RTC Source ---
 * Authors: Eric Born and Josh Friend
 * Course: EGR326-901
 * Instructor: Dr. Andrew Sterian
 * Date: Nov 7, 2011
 -----------------------------------------------------------------------------*/

#include "htc.h"
#include "I2C.h"
#include "RTC.h"

unsigned char seconds, minutes, hours, weekday, day, month, year;

unsigned char decimal_to_bcd(unsigned char dec) {
    return (dec/10 * 16) + (dec % 10);
}

unsigned char bcd_to_decimal(unsigned char bcd) {
    return ((bcd/16 * 10) + (bcd % 16));
}

void start_rtc(void) {
    //Start transmission
    i2c_start();
    
    //Begin transmission in write mode
    i2c_tx_byte(RTC_WRITE_ADDR);
    //Send register location for data
    i2c_tx_byte(0x00);
    
    //Send data to RTC
    i2c_tx_byte(decimal_to_bcd(seconds) & 0x80);
    
    //Stop transmission
    i2c_halt();
}

void stop_rtc(void) {
    //Start transmission
    i2c_start();
    
    //Begin transmission in write mode
    i2c_tx_byte(RTC_WRITE_ADDR);
    //Send register location for data
    i2c_tx_byte(0x00);
    
    //Send data to RTC
    i2c_tx_byte(decimal_to_bcd(seconds) | 0x7F);
    
    //Stop transmission
    i2c_halt();
}

void set_rtc_time(void) {
    //Start transmission
    i2c_start();
    
    //Begin transmission in write mode
    i2c_tx_byte(RTC_WRITE_ADDR);
    //Send register location for data
    i2c_tx_byte(0x00);
    
    //Send data to RTC
    i2c_tx_byte(decimal_to_bcd(seconds));
    i2c_tx_byte(decimal_to_bcd(minutes));
    i2c_tx_byte(decimal_to_bcd(hours));
    i2c_tx_byte(decimal_to_bcd(weekday));
    i2c_tx_byte(decimal_to_bcd(day));
    i2c_tx_byte(decimal_to_bcd(month));
    i2c_tx_byte(decimal_to_bcd(year));
    
    //Stop transmission
    i2c_halt();
}

void get_rtc_time(void) {
    //Start transmission
    i2c_start();
    
    //Start with dummy write
    i2c_tx_byte(RTC_WRITE_ADDR);
    
    //Send register location for data
    i2c_send(0x00);
    
    //Enable repeated start condition
    i2c_repstart();
    
    //Send read address
    i2c_tx_byte(RTC_READ_ADDR);
    
    //Recieve data and acknowlege each byte recieved
    seconds = bcd_to_decimal(i2c_recieve());
    i2c_ack();
    minutes = bcd_to_decimal(i2c_recieve());
    i2c_ack();
    hours = bcd_to_decimal(i2c_recieve());
    i2c_ack();
    weekday = bcd_to_decimal(i2c_recieve());
    i2c_ack();
    day = bcd_to_decimal(i2c_recieve());
    i2c_ack();
    month = bcd_to_decimal(i2c_recieve());
    i2c_ack();
    year = bcd_to_decimal(i2c_recieve());
    //End repeated start by non-ack
    i2c_noack();
    
    //Stop transmission
    i2c_halt();
}

void rtc_output_ctrl(unsigned char data) {
    //Start transmission
    i2c_start();
    
    //Begin transmission in write mode
    i2c_tx_byte(RTC_WRITE_ADDR);

    //Send address of output control register
    i2c_tx_byte(0x07);

    //Set output enable bit to value of en
    i2c_tx_byte(data);

    //Stop transmission
    i2c_halt();
}