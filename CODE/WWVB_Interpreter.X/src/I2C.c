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
 * Date: Nov 6, 2011
 -----------------------------------------------------------------------------*/

void i2c_setup(void) {
    //Disable slew rate control
    SSP1STAT |= 0x80;
    
    //Enable I2C in master mode, clock = FOSC/(4 * (SSPADD + 1))
    SSP1CON1 = 0b00101000;
    
    //For a 100kHz CLock at FOSC = 1MHz, SCL pin clock period = ((ADD<7:0> + 1) *4)/FOSC
    SSP1ADD = 0x0A;
}

void i2c_start(void) {
    //Enable start condition
    SEN = 1;
    
    //Wait for start condition to finish
    while(SEN);
}

void i2c_repstart(void) {
    //Enable repeated start condition
    RSEN = 1;
    
    //Wait for restart condition to finish
    while(PEN);
}

void i2c_halt(void) {
    //Enable stop condition
    PEN = 1;
    
    //Wait for stop condition to finish
    while(PEN);
}

void i2c_ack(void) {
    //Set acknowledge data bit, value 0 is acknowlege
    ACKDT = 0;
    
    //Initiate acknowledge sequence
    ACKEN = 1;
    
    //Wait for acknowledge sequence to finish
    while(ACKEN);
}

void i2c_noack(void) {
    //Set acknowledge data bit, value 1 is non-acknowlege
    ACKDT = 1;
    
    //Initiate acknowledge sequence
    ACKEN = 1;
    
    //Wait for acknowledge sequence to finish
    while(ACKEN);
}

void i2c_wait(void) {
    //Wait for any transfers to finish
    while (( SSPCON2 & 0x1F) || (SSPSTAT & 0x04));
}

void i2c_transmit(unsigned char data) {
    //Dump data to be sent into buffer
    SSP1BUF = data;
    
    //Wait for data to send
    while(BF);
    
    i2c_wait();
}

unsigned char i2c_recieve(void) {
    unsigned char data;

    //Set recieve enable bit
    RCEN = 1;
    
    //Wait for data buffer to fill
    while(!BF);
    
    //Store data recieved
    data = SSP1BUF;
    
    i2c_wait();
    
    //Return recieved data
    return data;
}