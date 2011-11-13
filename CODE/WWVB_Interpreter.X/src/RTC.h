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
 * Date: Nov 7, 2011
 -----------------------------------------------------------------------------*/

#ifndef RTC_h
#define RTC_h

//RTC Control address definitions
#ifdef RTC_DS1307
#define RTC_WRITE_ADDR 0xD0
#define RTC_READ_ADDR 0xD1
#endif
#ifdef RTC_MCP79400N
#define RTC_WRITE_ADDR 0xDE
#define RTC_READ_ADDR 0xDF
#endif
#ifdef PCF8574
#define RTC_WRITE_ADDR 0x40
#define RTC_READ_ADDR 0x41
#endif

//Function Prototypes
unsigned char decimal_to_bcd(unsigned char dec);

unsigned char bcd_to_decimal(unsigned char bcd);

void start_rtc();

void stop_rtc();

void set_rtc_time();

void get_rtc_time();

void rtc_output_en(unsigned char en);

#endif