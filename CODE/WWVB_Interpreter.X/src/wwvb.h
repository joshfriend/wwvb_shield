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
 * --- WWVB Interpreter Header ---
 * Authors: Eric Born and Josh Friend
 * Course: EGR326-901
 * Instructor: Dr. Andrew Sterian
 * Date: Nov 25, 2011
 -----------------------------------------------------------------------------*/

#ifndef WWVB_h
#define WWVB_h

#include "types.h"

//WWVB decoding algorithm parameters
#define MAX_EDGES 4             //Must be at least 1
#define MIN_BIT_WIDTH_MS 975    //Minimum bit width
#define BIT_TIMEOUT_MS 1100     //Bit width decode timout value

//Define WWVB Pulse widths
#define MAX_ERROR 100
#define MAX_FRAME 900
#define MAX_ONE 600
#define MAX_ZERO 300

//Definitions of values returned by bit_value()
#define FRAME 2
#define ONE 1
#define ZERO 0

//Global flag to mark when valid WWVB frame has been recieved
extern uint8_t frame_recieved_flag;

//Function prototypes
void process_bit(uint16_t pulse_length);
uint8_t get_bit_value(uint16_t pulse_length);
void process_frame(time_t *frame);

#endif