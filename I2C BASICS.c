/******** I2C BASICS **********/

void i2c_init(void); 
void i2c_start(void); 
void i2c_stop(void); 
void i2c_tx(int data);
int i2c_rx(void); 
//void i2c_write(unsigned int h_wr_addr, unsigned int l_wr_addr, char wr_data[]);
//void i2c_read(unsigned int h_re_addr, unsigned int l_re_addr, char re_data[]);
void i2c_write(int type, int addr, int size, char* data);
void i2c_read(int type, int addr, int size, char* dest);
void ACK();
void NACK();
void wait_idle(void); 
//void delay (long millisecs); 
void reset_flags (void);
//unsigned long hash(char *str); 
void __attribute__((interrupt, auto_psv)) _MI2C1Interrupt(void);


/******** I2C BASICS **********/
void i2c_write(int type, int addr, int size, char* data) {
	int i;
	i2c_start();
	if(type == _RTC) {
		i2c_tx(RTC_ADDR << 1);
		i2c_tx((addr & 0xF0) >> 2);
		i2c_tx(addr & 0xF);
	}
	if(type == _ROM) {
		i2c_tx(ROM_ADDR << 1);
		i2c_tx(addr);
	}
	for(i = 0; i < size; i++) {
		i2c_tx(*(data+i));
	}
	i2c_stop();
	__delay_ms(30);
	return;
}

void i2c_read(int type, int addr, int size, char* dest) {
	int i;
	i2c_start();
	if(type == _RTC) {
		i2c_tx(RTC_ADDR << 1);
		i2c_tx((addr & 0xF0) >> 2);
		i2c_tx(addr & 0xF);
	}
	if(type == _ROM) {
		i2c_tx(ROM_ADDR << 1);
		i2c_tx(addr);
	}
	//wait_idle();
	I2C1CONbits.RSEN = 1;
	//i2c_tx((ROM_));
	if(type == _RTC) i2c_tx((RTC_ADDR << 1) + 1);
	if(type == _ROM) i2c_tx((ROM_ADDR << 1) + 1);
	for(i = 0; i < size-1; i++) {
		*(dest+i) = i2c_rx();
		ACK();
	}
	*(dest+i) = i2c_rx();
	NACK();
	i2c_stop();
	__delay_ms(30);
	return;
}

void __attribute__((interrupt, auto_psv)) _MI2C1Interrupt(void){
   
    if (!I2C1STATbits.ACKSTAT){  
        acked = 1;
    }    
    if (I2C1STATbits.RBF){      
        rx_complete = 1;
    } 
    
    if (!I2C1STATbits.TBF){     
        tx_complete = 1;
    }
    if (!I2C1STATbits.IWCOL || !I2C1STATbits.BCL){    
        idle = 1;
    }
    // Clear I2C interrupt flag
    IFS1bits.MI2C1IF=0;
    //IFS1bits.SI2C1IF=0;
    return;
}

void i2c_init(void){
    // Configure GPIOs
    //TRISB = 0x8300;
    // ##### Interrupt-driven I2C configuration ########
    IEC1bits.MI2C1IE=1;// enable I2C interrupts
    IFS1bits.MI2C1IF=0;// clear interrupt flag for I2C
  //IEC1bits.SI2C1IE=1;// enable I2C interrupts
    IFS1bits.SI2C1IF=0;// clear interrupt flag for I2C
    
    // ##### I2C configuration #########################
    I2C1CONbits.I2CSIDL =1;
    I2C1BRG= 18;
    I2C1CONbits.ACKDT= 0;
    I2C1CONbits.ACKEN= 1;
    I2C1CONbits.I2CEN =1; // enable I2C module
    return;
}

void reset_flags(){
    idle = 1;
    tx_complete = 0;
    rx_complete = 0;
    acked = 0;
    return;
}

void wait_idle(){
    while (!idle){
    // do nothing, just wait
    }
    reset_flags();
    return;
}
/*
void delay (long millisecs) {
    // implement delay
    ms=millisecs*180;
    while (ms !=0){
        ms--;
    }
    return;
}
*/
void i2c_start(){
    wait_idle();//wait until free
    I2C1CONbits.SEN = 1;//initiate start condition on SDA and SCL pins
}

void i2c_stop(){
    wait_idle(); //wait until free
    I2C1CONbits.PEN = 1; //initiate stop condition on SDA and SCL pins
}

void i2c_tx(int data){
   
    wait_idle(); // wait until free
    idle=0;// set flag to declare you're not free/idle    
    I2C1TRN = data; // load data for transmission over the I2C bus
    while (!tx_complete);  // wait until TX is complete  
    while (!acked); // wait until ACK is received
    reset_flags();// reset flags   
}

int i2c_rx(){
    int temp = 0;
    // This ensures the master logic is inactive.
    I2C1CON = 0xFFE0;// The lower 5 bits of I2CxCON must be '0' before attempting to set the RCEN bit. 
    I2C1CONbits.RCEN = 1; // enable RX mode     
    wait_idle();// wait until free
    idle=0;// set flag to declare you're not free/idle
    while (!rx_complete);
    temp = I2C1RCV;// get our data, store in temp
    reset_flags();// reset flags    
    return temp;
}

void ACK() {
    I2C1CONbits.ACKDT=0;
    I2C1CONbits.ACKEN=1;
    return;
}

void NACK() {
    I2C1CONbits.ACKDT=1;//nack
    I2C1CONbits.ACKEN=0;
    return;
}