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
 * --- WWVB Interpreter Source ---
 * Authors: Eric Born and Josh Friend
 * Course: EGR326-901
 * Instructor: Dr. Andrew Sterian
 * Date: Nov 2, 2011
 -----------------------------------------------------------------------------*/

#include "htc.h"
#include "wwvb.h"

unsigned char bit_value(unsigned pulse_length) {
    unsigned char bit_val = 0;

    if(pulse_length < MAX_ERROR) {
	//Pulse not recognized
	bit_val = 3;
    }

    else if(pulse_length < MAX_FRAME) {
	//Recieved frame marker pulse
	bit_val = 2;
    }
    else if(pulse_length < MAX_ONE) {
	//Recieved binary one pulse
	bit_val = 1;
    }
    else if(pulse_length < MAX_ZERO){
	//Recieved binary zero pulse
	bit_val = 0;
    }

    return bit_val;
}