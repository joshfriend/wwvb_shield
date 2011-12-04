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
 * Date: Dec 8, 2011
 -----------------------------------------------------------------------------*/

#include "htc.h"
#include "I2C.h"
#include "RTC.h"
#include "types.h"

uint8_t decimal_to_bcd(uint8_t dec) {
    return (dec/10 * 16) + (dec % 10);
}

uint8_t bcd_to_decimal(uint8_t bcd) {
    return ((bcd/16 * 10) + (bcd % 16));
}

void time_to_bcd(time_t *time) {
    //Convert to BCD for RTC
    time->seconds = 0x80;
    time->minutes = decimal_to_bcd(time->minutes);
    time->hours = decimal_to_bcd(time->hours);
    time->day_of_week |= (1<<5);
    time->month = decimal_to_bcd(time->month);
    time->date = decimal_to_bcd(time->date);
    time->year = decimal_to_bcd(time->year);
    time->leap = decimal_to_bcd(time->leap);
}

void start_rtc(void) {
    //Start transmission
    i2c_start();
    
    //Begin transmission in write mode
    i2c_tx_byte(RTC_WRITE_ADDR);
    //Send register location for data
    i2c_tx_byte(0x00);
    
    //Send data to RTC
    i2c_tx_byte(0x80);
    
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
    i2c_tx_byte(0x00);
    
    //Stop transmission
    i2c_halt();
}

void set_rtc_time(time_t time) {
    //Start transmission
    i2c_start();
    
    //Begin transmission in write mode
    i2c_tx_byte(RTC_WRITE_ADDR);
    //Send register location for data
    i2c_tx_byte(0x00);
    
    //Send data to RTC
    i2c_tx_byte(decimal_to_bcd(time.seconds));
    i2c_tx_byte(decimal_to_bcd(time.minutes));
    i2c_tx_byte(decimal_to_bcd(time.hours));
    i2c_tx_byte(decimal_to_bcd(time.day_of_week));
    i2c_tx_byte(decimal_to_bcd(time.date));
    i2c_tx_byte(decimal_to_bcd(time.month));
    i2c_tx_byte(decimal_to_bcd(time.year));
    
    //Stop transmission
    i2c_halt();
}

time_t get_rtc_time(void) {
    time_t rtc_time;
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
    rtc_time.seconds = bcd_to_decimal(i2c_recieve());
    i2c_ack();
    rtc_time.minutes = bcd_to_decimal(i2c_recieve());
    i2c_ack();
    rtc_time.hours = bcd_to_decimal(i2c_recieve());
    i2c_ack();
    rtc_time.day_of_week = bcd_to_decimal(i2c_recieve());
    i2c_ack();
    rtc_time.date = bcd_to_decimal(i2c_recieve());
    i2c_ack();
    rtc_time.month = bcd_to_decimal(i2c_recieve());
    i2c_ack();
    rtc_time.year = bcd_to_decimal(i2c_recieve());
    //End repeated start by non-ack
    i2c_noack();
    
    //Stop transmission
    i2c_halt();
    
    return rtc_time;
}

void rtc_output_ctrl(uint8_t data) {
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