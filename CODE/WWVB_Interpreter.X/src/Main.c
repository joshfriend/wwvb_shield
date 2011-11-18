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
 * WWVB Interpreter Main Source
 * Authors: Eric Born and Josh Friend
 * Course: EGR326-901
 * Instructor: Dr. Andrew Sterian
 * Date: Nov 2, 2011
 -----------------------------------------------------------------------------*/

#include "htc.h"
#include "timer_hardware.h"
#include "ct_hardware.h"
//#include "wwvb.h"
#include "I2C.h"
#include "RTC.h"

__CONFIG(FOSC_INTOSC & WDTE_OFF & PWRTE_OFF & MCLRE_ON & CP_OFF & CPD_OFF & BOREN_OFF & CLKOUTEN_OFF & IESO_OFF & FCMEN_OFF);
__CONFIG(WRT_OFF & PLLEN_OFF & STVREN_OFF & LVP_OFF);

void setup(void);

volatile unsigned tick = 0;
volatile unsigned char buffer_position = 0;
volatile unsigned pulse_length_ms=0;

char data[10] = {0x00,0x80,0x01,0x02,0x04,0x17,0x11,0x88};

void main(void) {

    setup();
    io_setup();
    timer1_init();
    timer2_init();

    i2c_setup();

    rtc_output_ctrl(0b00010011);
    
    while(1) {
        //RC3 = T1GVAL;
    }
}

void interrupt isr (void) {
    //MSSP interrupt event
    if(SSP1IE && SSP1IF) {
        i2c_send_next();
        //MSSP has finished sending data (not working)
        if(!SSP1STATbits.BF && !SSP1CON2bits.RCEN) {
            //Begin next byte transmission
            //i2c_send_next();
        }
        //MSSP has finished recieving data
        else if(SSP1STATbits.BF && SSP1CON2bits.RCEN) {

        }

        //Clear interrupt flag
        SSP1IF = 0;
    }

    //Timer1 Gate event
    if(TMR1GIE && TMR1GIF) {
        //Used to analyze individual bits
        TMR1GIF = 0;

        TMR1 = 0;
        RC3 = !RC3;
        //i2c_tx(RTC_WRITE_ADDR,data,8);
        //First rising edge, reset system tick
        tick = 0;
    }

    //Timer2 compare match event
    if(TMR2IE && TMR2IF) {
        //Used to keep track of frame postition
        TMR2IF = 0;

        if(tick == 1000) {
            i2c_tx(RTC_WRITE_ADDR,data,8);
            //rtc_output_ctrl(0b00010011);
            tick = 0;
        }
        else {
            tick++;
        }
    }
}

void setup(void) {
    //Disable watchdog timer
    SWDTEN = 0;

    //Set primary clock frequency to 1MHz
    //OSCCON = IRCF3 + IRCF1 + IRCF0 + SCS1;
    OSCCON = 0b01011010;

    //Enable global interrupts
    GIE = 1;

    //Enable peripheral interrupts
    PEIE = 1;
}