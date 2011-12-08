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
 * Date: Dec 8, 2011
 -----------------------------------------------------------------------------*/

#include "calendar.h"
#include "types.h"

/*------------------------------------------------------------------------------
 * Description:
 * Finds the current month using day-of-year and leapyear info
 *
 * Parameters:
 * time - time_t structure containing time data
 *
 * Returns:
 * Month number (1-12)
 -----------------------------------------------------------------------------*/
uint8_t get_month(time_t time) {
    if(time.day_of_year <= 31)
        return 1;
    else if(time.day_of_year <= (59+time.leap))
        return 2;
    else if(time.day_of_year <= (90+time.leap))
        return 3;
    else if(time.day_of_year <= (120+time.leap))
        return 4;
    else if(time.day_of_year <= (151+time.leap))
        return 5;
    else if(time.day_of_year <= (181+time.leap))
        return 6;
    else if(time.day_of_year <= (212+time.leap))
        return 7;
    else if(time.day_of_year <= (243+time.leap))
        return 8;
    else if(time.day_of_year <= (273+time.leap))
        return 9;
    else if(time.day_of_year <= (304+time.leap))
        return 10;
    else if(time.day_of_year <= (334+time.leap))
        return 11;
    else
        return 12;
}

/*------------------------------------------------------------------------------
 * Description:
 * Finds the day of week from day-of-year
 *
 * Parameters:
 * time - time_t structure containing time data
 *
 * Returns:
 * Day of week in integer form (1-7) where 1 = Sunday, 7 = Saturday, etc.
 -----------------------------------------------------------------------------*/
uint8_t get_day_of_week(time_t time) {

    uint8_t d = get_day_of_month(time);
    uint8_t m = get_month(time);
    uint8_t y = time.year;
	
	//Sakamoto's method:
    // http://en.wikipedia.org/wiki/Weekday_determination#Sakamoto.27s_Method
    
    static uint8_t t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
    y -= m < 3;
    return (uint8_t)((y + y/4 - y/100 + y/400 + t[m-1] + d) % 7) + 1;

}

/*------------------------------------------------------------------------------
 * Description:
 * Finds the day of month (date) given day-of-year
 *
 * Parameters:
 * time - time_t structure containing time data
 *
 * Returns:
 * Day of month in integer form (0-31)
 -----------------------------------------------------------------------------*/
uint8_t get_day_of_month(time_t time) {
    uint8_t m = get_month(time);
    uint16_t d = time.day_of_year;

    for(uint8_t i=1;i<m;i++){
        if(d > days_in_month(i, time.leap)){
            d -= days_in_month(i, time.leap);
        }
    }

    return d;
}

/*------------------------------------------------------------------------------
 * Description:
 * Gets the number of days in the given month
 *
 * Parameters:
 * month - The current month (1-12)
 * leap - 1 if it is a leap year, 0 if not.
 *
 * Returns:
 * The number of days in the given month (28-31 epending on month and leapyear)
 -----------------------------------------------------------------------------*/
uint8_t days_in_month(uint8_t month, uint8_t leap){
    if(month == 2)
        return 28 + leap;
    else if(month==4 || month==6 || month==10 || month==11)
        return 30;
    else
        return 31;
}

/*------------------------------------------------------------------------------
 * Description:
 * Checks if date info inside a time_t is valid
 *
 * Parameters:
 * time - The time_t structure to be checked
 *
 * Returns:
 * 1 if date is valid, 0 if not valid.
 -----------------------------------------------------------------------------*/
uint8_t validate(time_t time) {
    
    if(time.minutes > 59)
        return 0;
    if(time.hours > 23)
        return 0;
    if(time.day_of_year > (365 + time.leap))
        return 0;
    if(time.year > 99)
        return 0;
    if(time.month > 12 || time.month < 1)
        return 0;
    
    return 1;
}