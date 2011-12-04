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
 * --- RTC Header ---
 * Authors: Eric Born and Josh Friend
 * Course: EGR326-901
 * Instructor: Dr. Andrew Sterian
 * Date: Dec 8, 2011
 -----------------------------------------------------------------------------*/

#ifndef RTC_h
#define RTC_h

#include "types.h"

//RTC Control address definitions
#ifdef RTC_DS1307
#define RTC_WRITE_ADDR 0xD0
#define RTC_READ_ADDR 0xD1
#endif
#ifdef RTC_MCP79400N
#define RTC_WRITE_ADDR 0xDE
#define RTC_READ_ADDR 0xDF
#endif
#ifdef RTC_MCP79410N
#define RTC_WRITE_ADDR 0xDE
#define RTC_READ_ADDR 0xDF
#define EEPROM_WRITE_ADDR 0xFE
#define EEPROM_READ_ADDR 0xFF
#endif

//Function Prototypes
uint8_t decimal_to_bcd(uint8_t dec);
uint8_t bcd_to_decimal(uint8_t bcd);
void time_to_bcd(time_t *time);
void start_rtc(void);
void stop_rtc(void);
void set_rtc_time(time_t);
time_t get_rtc_time(void);
void rtc_output_ctrl(uint8_t data);

#endif