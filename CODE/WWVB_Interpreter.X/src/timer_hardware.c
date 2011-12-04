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
 * --- Timer Hardware Source ---
 * Authors: Eric Born and Josh Friend
 * Course: EGR326-901
 * Instructor: Dr. Andrew Sterian
 * Date: Dec 8, 2011
 -----------------------------------------------------------------------------*/

#include "htc.h"
#include "timer_hardware.h"

//CPU Frequency
#define FOSC 1000000UL

void timer1_init(void) {
    //Timer ON, 1/8 Prescaler, Source = FOSC/4, 2 second range
    TMR1ON = 1;
    T1CKPS1 = 1;
    T1CKPS0 = 1;

    //Enable gate on T1G pin, active LOW.
    TMR1GE = 1;
    T1GPOL = 0;

    //Enable Global interrupts
    GIE = 1;

    //Enable peripheral interrupts
    PEIE = 1;
}

void timer2_init(void) {
    //Timer2 ON, 1/1 prescaler = 4us per tick
    TMR2ON = 1;

    //Timer2 compare match set to create 1ms interrupt tick
    PR2 = ((FOSC / 4) / 1) / 1000 - 1;

    //Enable Timer2 Interrupts
    TMR2IE = 1;

    //Clear interrupt flag
    TMR2IF = 0;

    //Enable Global interrupts
    GIE = 1;

    //Enable peripheral interrupts
    PEIE = 1;
}