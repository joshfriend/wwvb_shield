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
 * Date: Dec 8, 2011
 -----------------------------------------------------------------------------*/

#include "htc.h"
#include "wwvb.h"
#include "calendar.h"
#include "types.h"
#include "I2C.h"
#include "RTC.h"

uint8_t frame_recieved_flag = 0;
uint8_t frame_position = 0;

//Variable to store WWVB time data as it is processed
static time_t wwvb;

/*------------------------------------------------------------------------------
 * Description:
 * Processes a bit recieved from the WWVB radio. A finite state machine
 * implementation is used to keep track of frame position and evaluate the 
 * meaning of each bit recieved.
 *
 * Parameters:
 * pulse_length - the duration of the WWVB pulse in milliseconds
 *
 * Returns:
 * The value of the bit decoded (0-3 for ZERO, ONE, FRAME, or ERROR)
 -----------------------------------------------------------------------------*/
uint8_t process_bit(uint16_t pulse_length) {
    static uint8_t prev_bit_value = 0;
    uint8_t bit_value = get_bit_value(pulse_length);

    //Roll-over frame position
    if(frame_position > 59) {
        clear_data(&wwvb);
        frame_position = 0;
    }

#ifdef DEBUG
    //Send debug data (bit value)
    i2c_start();
    i2c_tx_byte(8);
    i2c_tx_byte(bit_value);
    i2c_halt();
#endif

    //Check if error can be ignored
    if(bit_value == ERROR && !dont_care_bit[frame_position]) {
        clear_data(&wwvb);
        frame_position = 0;
    }
    else {
        if(bit_value == FRAME && prev_bit_value == FRAME) {
            //Bitstream minute mark detected, set frame position to 0
            clear_data(&wwvb);
            frame_position = 0;

#ifdef DEBUG
            //Send debug data (frame mark detected)
            i2c_start();
            i2c_tx_byte(8);
            i2c_tx_byte(0xFC);
            i2c_halt();
#endif
        }

        switch(frame_position) {
            //Frame markers...
            case 0:
            case 9:
            case 19:
            case 29:
            case 39:
            case 49:
            case 59:
                if(bit_value != FRAME) {
                    //Syncs frame position counter
                    //Increment operation at end will set this to 0:
                    frame_position = -1;
                    clear_data(&wwvb);
                    
#ifdef DEBUG
                    //Send debug data (frame error)
                    i2c_start();
                    i2c_tx_byte(8);
                    i2c_tx_byte(0xFF);
                    i2c_halt();
#endif
                }
                else if(frame_position == 59) {
                    frame_recieved_flag = 1;
                }
                break;
    
            //Seconds bits...
            case 1:
                if(bit_value == FRAME) {
                    //Double mark detected in bitstream, reset position to 0
                    frame_position = 0;
                    clear_data(&wwvb);
                }
                else {
                    wwvb.minutes += 40 * bit_value;
                }
                break;
            case 2:
                wwvb.minutes += 20 * bit_value;
                break;
            case 3:
                wwvb.minutes += 10 * bit_value;
                break;
            case 5:
                wwvb.minutes += 8 * bit_value;
                break;
            case 6:
                wwvb.minutes += 4 * bit_value;
                break;
            case 7:
                wwvb.minutes += 2 * bit_value;
                break;
            case 8:
                wwvb.minutes += 1 * bit_value;
                break;
    
            //Hours bits...
            case 12:
                wwvb.hours += 20 * bit_value;
                break;
            case 13:
                wwvb.hours += 10 * bit_value;
                break;
            case 15:
                wwvb.hours += 8 * bit_value;
                break;
            case 16:
                wwvb.hours += 4 * bit_value;
                break;
            case 17:
                wwvb.hours += 2 * bit_value;
                break;
            case 18:
                wwvb.hours += 1 * bit_value;
                break;
    
            //Day of year bits...
            case 22:
                wwvb.day_of_year += 200 * bit_value;
                break;
            case 23:
                wwvb.day_of_year += 100 * bit_value;
                break;
            case 25:
                wwvb.day_of_year += 80 * bit_value;
                break;
            case 26:
                wwvb.day_of_year += 40 * bit_value;
                break;
            case 27:
                wwvb.day_of_year += 20 * bit_value;
                break;
            case 28:
                wwvb.day_of_year += 10 * bit_value;
                break;
            case 30:
                wwvb.day_of_year += 8 * bit_value;
                break;
            case 31:
                wwvb.day_of_year += 4 * bit_value;
                break;
            case 32:
                wwvb.day_of_year += 2 * bit_value;
                break;
            case 33:
                wwvb.day_of_year += 1 * bit_value;
                break;
    
            //DUT correction bits...
            case 36:
            case 37:
            case 38:
            case 40:
            case 41:
            case 42:
            case 43:
                //DUT1 Correction not implemented yet...
                //max skew from actual time +/- 0.7
                break;
    
            //Year bits...
            case 45:
                wwvb.year += 80 * bit_value;
                break;
            case 46:
                wwvb.year += 40 * bit_value;
                break;
            case 47:
                wwvb.year += 20 * bit_value;
                break;
            case 48:
                wwvb.year += 10 * bit_value;
                break;
            case 50:
                wwvb.year += 8 * bit_value;
                break;
            case 51:
                wwvb.year += 4 * bit_value;
                break;
            case 52:
                wwvb.year += 2 * bit_value;
                break;
            case 53:
                wwvb.year += 1 * bit_value;
                break;
    
            //Leap year bit
            case 55:
                //Year is a leap year?
                wwvb.leap = bit_value;
                break;
    
            //Leap second bit
            case 56:
                //Leap second warning
                wwvb.leap_second = bit_value;
                break;
    
            //Daylight Savings indicator bits...
            // 0 = DST not in effect
            // 1 = DST ends today
            // 2 = DST begins today
            // 3 = DST is in effect
            case 57:
                wwvb.dst += 2 * bit_value;
                break;
            case 58:
                wwvb.dst += 1 * bit_value;
                break;
    
            default:
                //Don't care bit recieved
                break;
        }
    
        frame_position++;
    }

    //Save previous bit value
    prev_bit_value = bit_value;

    return bit_value;
}

/*------------------------------------------------------------------------------
 * Description:
 * Gets the value of the WWVB bit.
 *
 * Parameters:
 * pulse_length - the duration of the WWVB pulse in milliseconds
 *
 * Returns:
 * The value of the bit decoded (0-3 for ZERO, ONE, FRAME, or ERROR)
 -----------------------------------------------------------------------------*/
uint8_t get_bit_value(uint16_t pulse_length) {
    uint8_t bit_value = 0;

    //Use windowing approach
    if((pulse_length > (BIT_LEN_ZERO_MS - MARGIN_MS))
        && (pulse_length < (BIT_LEN_ZERO_MS + MARGIN_MS))) {
        bit_value = ZERO;
    }
    else if((pulse_length > (BIT_LEN_ONE_MS - MARGIN_MS))
             && (pulse_length < (BIT_LEN_ONE_MS + MARGIN_MS))) {
        bit_value = ONE;
    }
    else if((pulse_length > (BIT_LEN_FRAME_MS - MARGIN_MS))
             && (pulse_length < (BIT_LEN_FRAME_MS + MARGIN_MS))) {
        bit_value = FRAME;
    }
    else {
        bit_value = ERROR;
    }

    return bit_value;
}

/*------------------------------------------------------------------------------
 * Description:
 * Processes the data recieved and copies it to a new location.
 *
 * Parameters:
 * *frame - Pointer reference to the location where the data will be copied to.
 *
 * Returns:
 * none
 -----------------------------------------------------------------------------*/
void process_frame(time_t *frame) {
    //Process WWVB Date data
    wwvb.month = get_month(wwvb);
    wwvb.date = get_day_of_month(wwvb);
    wwvb.day_of_week = get_day_of_week(wwvb);

    //Copy data for RTC
    frame->seconds = 0x80;
    frame->minutes = wwvb.minutes + 1;  //Frame is for previous minute
    frame->hours = wwvb.hours;
    frame->day_of_week = wwvb.day_of_week;
    frame->month = wwvb.month;
    frame->date = wwvb.date;
    frame->day_of_year = wwvb.day_of_year;
    frame->year = wwvb.year;
    frame->leap = wwvb.leap;

    clear_data(&wwvb);
}

/*------------------------------------------------------------------------------
 * Description:
 * Sets all values in a time_t structure back to 0
 *
 * Parameters:
 * *frame - Pointer reference to time_t structure to be cleared
 *
 * Returns:
 * none
 -----------------------------------------------------------------------------*/
void clear_data(time_t *frame) {
    frame->seconds = 0;
    frame->minutes = 0;
    frame->hours = 0;
    frame->day_of_week = 0;
    frame->date = 0;
    frame->month = 0;
    frame->year = 0;
    frame->day_of_year = 0;
    frame->leap = 0;
    frame->leap_second = 0;
    frame->dst = 0;
}