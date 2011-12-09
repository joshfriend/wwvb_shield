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
 * --- WWVB Interpreter Main Source ---
 * Authors: Eric Born and Josh Friend
 * Course: EGR326-901
 * Instructor: Dr. Andrew Sterian
 * Date: Dec 8, 2011
 -----------------------------------------------------------------------------*/

#include "htc.h"
#include "timer_hardware.h"
#include "ct_hardware.h"
#include "wwvb.h"
#include "I2C.h"
#include "RTC.h"
#include "types.h"
#include "calendar.h"

//Set config bits
__CONFIG(FOSC_INTOSC & WDTE_OFF & PWRTE_OFF & MCLRE_OFF &
         CP_OFF & CPD_OFF & BOREN_OFF & CLKOUTEN_OFF & IESO_OFF & FCMEN_OFF);
__CONFIG(WRT_OFF & PLLEN_OFF & STVREN_OFF & LVP_OFF);

//Function prototypes
void setup(void);

//WWVB processing variables
volatile uint16_t tick = 0;
volatile uint16_t pulse_length = 0;
volatile uint16_t timer1_val = 0;
volatile uint8_t edge_count = 0;
volatile uint8_t bit_recieved_flag = 0;

//Buffer for I2C data
uint8_t i2c_buffer[10] = {0};


/*------------------------------------------------------------------------------
 * Description:
 * MAIN CODE LOOP
 * Initializes harware peripherals
 * Waits for WWVB processing functions and interrupts to set flags. Main then 
 * calls the appropriate function to deal with these flags.
 *
 * Parameters:
 * none
 *
 * Returns:
 * none
 -----------------------------------------------------------------------------*/
void main(void) {
    //Setup main clock
    setup();
    //Initialize IO
    io_setup();

    //Initialize timer hardware
    timer1_init();
    timer2_init();

    //Initialize pin change interrupts
    pcint_setup();

    //Initialize I2C hardware
    i2c_setup();

    while(1) {
        if(bit_recieved_flag == 1) {
            //Convert timer count to milliseconds (divide by 32)
            pulse_length >>= 5;

            uint8_t wwvb_bit = process_bit(pulse_length);

            //Clear flag
            bit_recieved_flag = 0;
        }
        if(frame_recieved_flag == 1) {
#ifdef DEBUG
            //Send debug data (frame recieved)
            i2c_start();
            i2c_tx_byte(RTC_WRITE_ADDR);
            i2c_tx_byte(0xFE);
            i2c_halt();
#endif

            //Get time data
            time_t time;
            process_frame(&time);
            
            if(validate(time)) {
#ifdef DEBUG
                //Send debug data (passed validation)
                i2c_start();
                i2c_tx_byte(RTC_WRITE_ADDR);
                i2c_tx_byte(0xFB);
                i2c_halt();
#endif


                //Format to BCD for RTC
                time_to_bcd(&time);

                i2c_start();
                //Slave address:
                i2c_tx_byte(RTC_WRITE_ADDR);
                //Register address:
                i2c_tx_byte(0x00);
                i2c_tx_byte(time.seconds);
                i2c_tx_byte(time.minutes);
                i2c_tx_byte(time.hours);
                i2c_tx_byte(time.day_of_week);
                i2c_tx_byte(time.date);
                i2c_tx_byte(time.month);
                i2c_tx_byte(time.year);
#ifdef DEBUG
                //Debug code for new line
                i2c_tx_byte(0xFF);
#endif
                i2c_halt();

                
            }
            else {
#ifdef DEBUG
                //Send debug data (failed validation)
                i2c_start();
                i2c_tx_byte(RTC_WRITE_ADDR);
                i2c_tx_byte(0xFA);
                i2c_halt();
#endif
            }

            //Clear flag
            frame_recieved_flag = 0;
        }
    }
}

/*------------------------------------------------------------------------------
 * Description:
 * Interrupt service routine for the PIC16F1824.
 * Responsible for handling I2C, gated timer and Timer2 interrupt events.
 * This ISR handles the timing of the WWVB radio input.
 *
 * Parameters:
 * none
 *
 * Returns:
 * none
 -----------------------------------------------------------------------------*/
void interrupt isr (void) {
    //MSSP interrupt event
    if(SSP1IE && SSP1IF) {
        //Transmit next byte of I2C data in buffer
        i2c_send_next();
        
        //Clear interrupt flag
        SSP1IF = 0;
    }

    //RA4 Pin change interrupt event
    if(IOCAF4 && IOCAN4) {
        //Beginning of bit
        if(edge_count == 0) {
            TMR1 = 0;
            tick = 0;
            edge_count++;
        }
        else {
            //Check to see that transition occured inside width of one bit
            if(tick <= MIN_BIT_WIDTH_MS) {
                //Increment edge count
                edge_count++;
            }
            //Edge outside bit width, reset
            else if(tick >= BIT_TIMEOUT_MS) {
                edge_count = 0;
            }
            //End of bit/beginning of new bit
            else {
                //set flag recieved bit
                bit_recieved_flag = 1;
                
                //Save length of bit
                if(edge_count > MAX_EDGES) {
                    pulse_length = 0;
                }
                else {
                    pulse_length = TMR1;
                }

                //Reset timers
                TMR1 = 0;
                tick = 0;

                //Reset edge count
                edge_count = 1;
            }
        }
        
        //Clear interrupt flag
        IOCAF4 = 0;
    }

    //Timer2 compare match event
    if(TMR2IE && TMR2IF) {
        //Clear interrupt flag
        TMR2IF = 0;

        //Increment tick
        tick++;
    }
}

/*------------------------------------------------------------------------------
 * Description:
 * Initializes the main oscilator hardware and interrupts
 * Main clock frequency FOSC is set to 1 MHz
 *
 * Parameters:
 * none
 *
 * Returns:
 * none
 -----------------------------------------------------------------------------*/
void setup(void) {
    //Disable watchdog timer
    SWDTEN = 0;

    //Set primary clock frequency to 1MHz
    //OSCCON = IRCF3 + IRCF1 + IRCF0 + SCS1;
    OSCCON = 0b01011010;

    //Enable global interrupts
    GIE = 1;

    //Enable peripheral interrupts
    PEIE = 1;
}