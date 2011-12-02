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
 * --- Types Header ---
 * Authors: Eric Born and Josh Friend
 * Course: EGR326-901
 * Instructor: Dr. Andrew Sterian
 * Date: Nov 25, 2011
 -----------------------------------------------------------------------------*/
 
#ifndef TYPES_h
#define TYPES_h

typedef unsigned char uint8_t;
typedef unsigned uint16_t;
 
//Time elements structure
typedef struct {
    //Organized in same order as in RTC memmory map
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;
    uint8_t day_of_week;
    uint8_t date;
    uint8_t month;
    uint8_t year;
    
    //Used only by WWVB interpreter functions
    uint16_t day_of_year;
    uint8_t leap;
    uint8_t leap_second;
    uint8_t dst;
} time_t;

#endif