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
 * --- Calendar Source ---
 * Authors: Eric Born and Josh Friend
 * Course: EGR326-901
 * Instructor: Dr. Andrew Sterian
 * Date: Nov 25, 2011
 -----------------------------------------------------------------------------*/

#include "calendar.h"
#include "types.h"

uint8_t get_month(time_t time) {
    if(time.day_of_year < 31)
        return 1;
    else if(time.day_of_year < (59+time.leap))
        return 2;
    else if(time.day_of_year < (90+time.leap))
        return 3;
    else if(time.day_of_year < (120+time.leap))
        return 4;
    else if(time.day_of_year < (151+time.leap))
        return 5;
    else if(time.day_of_year < (181+time.leap))
        return 6;
    else if(time.day_of_year < (212+time.leap))
        return 7;
    else if(time.day_of_year < (243+time.leap))
        return 8;
    else if(time.day_of_year < (273+time.leap))
        return 9;
    else if(time.day_of_year < (304+time.leap))
        return 10;
    else if(time.day_of_year < (334+time.leap))
        return 11;
    else
        return 12;
}

uint8_t get_day_of_week(time_t time) {

    uint8_t d = get_day_of_month(time);
    uint8_t m = get_month(time);
    uint8_t y = time.year;

    /*This is an alternate method which I used in my 350 project.
    http://en.wikipedia.org/wiki/Weekday_determination#Sakamoto.27s_Method
    */
    static uint8_t t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
    y -= m < 3;
    return (uint8_t)((y + y/4 - y/100 + y/400 + t[m-1] + d) % 7);

}

uint8_t get_day_of_month(time_t time) {
    uint8_t m = get_month(time);
    uint8_t d = time.day_of_year;

    for(uint8_t i=1;i<m;i++){
        if(d > days_in_month(i, time.leap)){
            d -= days_in_month(i, time.leap);
        }
    }

    return d;
}

uint8_t days_in_month(uint8_t month, uint8_t leap){
    if(month == 2)
        return 28 + leap;
    else if(month==4 || month==6 || month==10 || month==11)
        return 30;
    else
        return 31;
}

uint8_t validate(time_t time) {
    
    if(time.minutes > 59 || time.minutes < 0)
        return 0;
    if(time.hours > 23 || time.hours < 0)
        return 0;
    if(time.day_of_year > (365 + time.leap) || time.day_of_year < 0)
        return 0;
    if(time.year > 99 || time.year < 0)
        return 0;
    if(time.month > 12 || time.month < 1)
        return 0;
    
    return 1;
}