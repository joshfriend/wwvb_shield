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
 * --- I2C Communications Source ---
 * Authors: Eric Born and Josh Friend
 * Course: EGR326-901
 * Instructor: Dr. Andrew Sterian
 * Date: Nov 25, 2011
 -----------------------------------------------------------------------------*/

#include "htc.h"
#include "types.h"
#include <stdlib.h>

#define MAX_TX_BUFFER_SIZE 30

static uint8_t i2c_tx_buf[MAX_TX_BUFFER_SIZE] = {0};
//static uint8_t i2c_rx_buf[MAX_TX_BUFFER_SIZE] = {0};
static uint8_t tx_buffer_size = 0, rx_buffer_size = 0, buffer_pos = 0;

void i2c_setup(void) {
    //Disable slew rate control
    SSP1STAT |= 0x80;
    
    //Enable I2C in master mode, clock = FOSC/(4 * (SSPADD + 1))
    SSP1CON1 = 0b00101000;
    
    //For a 100kHz CLock at FOSC = 1MHz, SCL pin clock period = ((ADD<7:0> + 1) *4)/FOSC
    SSP1ADD = 0x01;

    //Enable Global interrupts
    GIE = 1;

    //Enable peripheral interrupts
    PEIE = 1;
}

void i2c_start(void) {
    //Enable start condition
    SSP1CON2bits.SEN = 1;
    
    //Wait for start condition to finish
    while(SSP1CON2bits.SEN);
}

void i2c_repstart(void) {
    //Enable repeated start condition
    SSP1CON2bits.RSEN = 1;
    
    //Wait for restart condition to finish
    while(SSP1CON2bits.PEN);
}

void i2c_halt(void) {
    //Enable stop condition
    SSP1CON2bits.PEN = 1;
    
    //Wait for stop condition to finish
    while(SSP1CON2bits.PEN);
}

void i2c_ack(void) {
    //Set acknowledge data bit, value 0 is acknowlege
    SSP1CON2bits.ACKDT = 0;
    
    //Initiate acknowledge sequence
    SSP1CON2bits.ACKEN = 1;
    
    //Wait for acknowledge sequence to finish
    while(SSP1CON2bits.ACKEN);
}

void i2c_noack(void) {
    //Set acknowledge data bit, value 1 is non-acknowlege
    SSP1CON2bits.ACKDT = 1;
    
    //Initiate acknowledge sequence
    SSP1CON2bits.ACKEN = 1;
    
    //Wait for acknowledge sequence to finish
    while(SSP1CON2bits.ACKEN);
}

void i2c_wait(void) {
    //Wait for any transfers to finish
    while (( SSPCON2 & 0x1F) || (SSPSTAT & 0x04));
}

void i2c_tx_byte(uint8_t data) {
    //Dump data to be sent into buffer
    SSP1BUF = data;
    
    //Wait for data to send
    while(SSP1STATbits.BF);
    
    i2c_wait();
}

uint8_t i2c_send_next(void) {

    //Check if slave acknowledged previous transmission
    if(!ACKSTAT) {
        //Check to see that buffer position is still in range
        if(buffer_pos < tx_buffer_size){
            //Put next byte in buffer
            SSP1BUF = i2c_tx_buf[buffer_pos++];
        }
        else {
            //Buffer has been sent, send stop condition
            i2c_halt();

            //Disable MSSP interrupts
            SSP1IE = 0;
        }
    }
    else {
        //Slave did not acknowledge, send stop condition
        i2c_halt();

        //Disable MSSP interrupts
        SSP1IE = 0;
    }

    //Return 1 if slave acknowledged, 0 if no-ack
    return !ACKSTAT;
}

uint8_t i2c_tx(uint8_t address, uint8_t *data, uint8_t count) {
    //Initialize bus
    i2c_start();

    //Enable MSSP interrupts
    SSP1IE = 1;
 
    buffer_pos = 0;

    //Store transmission size
    tx_buffer_size = count+1;

    //Put address and data in transmit buffer
    i2c_tx_buf[0] = address;
    for(int i = 1;  i <= count; i++)
        i2c_tx_buf[i] = data[i-1];

    //Start sending first byte of data
    SSP1BUF = i2c_tx_buf[buffer_pos++];
}

uint8_t i2c_recieve(void) {
    uint8_t data;

    //Set recieve enable bit
    SSP1CON2bits.RCEN = 1;
    
    //Wait for data buffer to fill
    while(!SSP1STATbits.BF);
    
    //Store data recieved
    data = SSP1BUF;
    
    i2c_wait();
    
    //Return recieved data
    return data;
}