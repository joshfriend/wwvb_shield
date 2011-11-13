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

void main(void) {
    unsigned char en = 0;

    setup();
    io_setup();
    timer1_init();
    timer2_init();

    i2c_setup();

    while(1) {
        
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