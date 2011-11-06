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
 * --- I2C Communications Header ---
 * Authors: Eric Born and Josh Friend
 * Course: EGR326-901
 * Instructor: Dr. Andrew Sterian
 * Date: Nov 6, 2011
 -----------------------------------------------------------------------------*/
 
//RTC Control address definitions
#ifdef RTC_DS1307
#define RTC_WRITE_ADDR 0xD0
#define RTC_READ_ADDR 0xD1
#endif
#ifdef RTC_MCP79400N
#define RTC_WRITE_ADDR 0xDE
#define RTC_WRITE_ADDR 0xDF
#endif

//Function Prototypes
void i2c_setup(void);

void i2c_start(void);

void i2c_repstart(void);

void i2c_halt(void);

void i2c_ack(void);

void i2c_noack(void);

void i2c_wait(void);

void i2c_transmit(unsigned char data);

unsigned char i2c_recieve(void);