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
 * --- Shield Hardware Source ---
 * Authors: Eric Born and Josh Friend
 * Course: EGR326-901
 * Instructor: Dr. Andrew Sterian
 * Date: Dec 8, 2011
 -----------------------------------------------------------------------------*/

#include "htc.h"
#include "ct_hardware.h"

/*------------------------------------------------------------------------------
 * Description:
 * Properly initializes the PIC IO hardware for use with the Chrono-tomic
 * Arduino Shield.
 *
 * Parameters:
 * none
 *
 * Returns:
 * none
 -----------------------------------------------------------------------------*/
void io_setup(void) {
    //Turn ADC Functionality OFF
    ANSELA = ANSELC = 0;

    //Set all pins as outputs
    TRISA = TRISC = 0;

    //Make T1G an input
    TRISA |= 0x10;

    //Set I2C pins as inputs
    TRISC |= 0x03;

    //Enable weak pull-ups
    nWPUEN = 0;

    //Enable weak pull-up on RA4
    WPUA |= 0x10;
}

/*------------------------------------------------------------------------------
 * Description:
 * Initializes falling edge pin-change interrupts on gated timer input (RA4).
 *
 * Parameters:
 * none
 *
 * Returns:
 * none
 -----------------------------------------------------------------------------*/
void pcint_setup(void) {
    //Enable negative edge pin change interrupt on RA4
    IOCAN4 = 1;

    //Enable Global interrupts
    GIE = 1;

    //Enable peripheral interrupts
    PEIE = 1;
}