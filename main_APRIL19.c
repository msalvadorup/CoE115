#include "xc.h"
#define FCY 2000000
#include <libpic30.h>
#include <p24FJ64GA002.h>

//Struct
typedef struct time {
    int hour;
    int min;
    int sec;
    int month;
    int date;
    int year;
} time;

//Config GB002
//_CONFIG1 (FWDTEN_OFF & JTAGEN_OFF)
//_CONFIG2 (POSCMOD_NONE & OSCIOFNC_ON & FCKSM_CSDCMD & FNOSC_FRCPLL & PLL96MHZ_OFF & PLLDIV_NODIV)
//_CONFIG3 (SOSCSEL_IO)
/*
#pragma config FWDTEN=OFF, JTAGEN=OFF
#pragma config POSCMOD=NONE, OSCIOFNC=ON, FCKSM=CSDCMD, FNOSC=FRCPLL 
#pragma config PLL96MHZ=OFF, PLLDIV=NODIV
#pragma config SOSCSEL=SOSC
#pragma config I2C1SEL=PRI
*/
//Config GA002
#pragma config FWDTEN=OFF, JTAGEN=OFF
#pragma config POSCMOD=NONE, OSCIOFNC=ON, FCKSM=CSDCMD, FNOSC=FRCPLL 
#pragma config I2C1SEL=PRI  

/* else if PIC24FJ64GA202
#pragma config FWDTEN=OFF, JTAGEN=OFF
#pragma config POSCMD=NONE, OSCIOFCN=ON, FCKSM=CSDCMD, FNOSC=FRC 
#pragma config PLLDIV=DISABLED
#pragma config SOSCSEL=ON
#pragma config I2C1SEL=DISABLE
*/

//Pin Assignments
#define SCL LATBbits.LATB8
#define SDA LATBbits.LATB9
#define LED1 LATBbits.LATB5
#define LCD_RS LATBbits.LATB13
#define LCD_EN LATBbits.LATB14
#define LCD_0 LATBbits.LATB7
#define LCD_1 LATBbits.LATB5
#define LCD_2 LATBbits.LATB4
#define LCD_3 LATBbits.LATB3
//#define LCD_TRIS 
#define KP_R1 LATAbits.LATA0
#define KP_R2 LATAbits.LATA1
#define KP_R3 LATAbits.LATA2
#define KP_R4 LATAbits.LATA3
#define KP_C1 LATBbits.LATB0
#define KP_C2 LATBbits.LATB1
#define KP_C3 LATBbits.LATB2
#define FIRST_PTR 0x1000
#define LAST_PTR 0x1003
#define ROM_ADDR 0x50
#define RTC_ADDR 0x68
#define _RTC 1
#define _ROM 0


//Constants
#define DEB_MAX 10
#define SAMPLE
#define BUFMAX 32

//Global Variables
int mode = 0;
/* MODES
1 = Get HHmm
2 = Get Frequency
3 = Data Logging Mode
4 = Data Display Mode
5 = Confirmation
*/
float adcBuf = 0;
float temperature = 0;
int adcValue = 0;
int freq = 0;
int lcdRow = 0;
int lcdCol = 0;
int last_addr = 0;
time currentTime;


//I2C VARIABLES
char indata[] = "tenten";
char outdata[BUFMAX]; 
int idle = 1;
int acked = 0;
int rx_complete = 0;
int tx_complete = 0;
int temp=0;
int ms;
int c, d;
int cbyte_write= 0b10100000; 
int cbyte_read = 0b10100001;
int i = 0; //tx
int j =0; //rx
char test[8];


//LCD
void lcd_init(); //OK
void send4toLCD(int a); //OK
void send8ToLCD(int a, int b); //OK
void write8ToLCD(int a, int b); //OK
void print(char* str); //OK
//void print_char(char a);
void clear_LCD(); //OK
void LCD_clearLine(int a); //OK
void LCD_moveCursor(int a); //OK
void set_cursor(int a); //OK
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

char* int_to_string(int a);


int main() {
//Initialization
    adc_init();
    lcd_init();
    keypad_init();
    i2c_init();

    //If clock isn't initialized
    if(1) { //NO CHECKING IF INITIALIZED
        mode = 1;
        //set_cursor(0, 1);
        //print("HH:mm DD/MM/YYYY");
        //set_cursor(0, 0);
        //input_time(); //THIS IS LOOPED
        set_cursor(1);
        print("HH:mm MM/DD/YYYY");
        set_cursor(0);
        print("  :     /  /");
        set_cursor(0);
        int cursor = 0;
        char time[16];
        time[2] = 0;
        time[5] = 0;
        time[8] = 0;
        time[11] = 0;
        while(mode == 1) {
            if(keyPressed) {
                if((key == 11) && (cursor!=0)) {
                    move_control(0); //MOVE LEFT
                    cursor--;
                } else if((key == 12) && (cursor!=15)) {
                    move_control(1); //MOVE RIGHT
                    cursor++
                } else if(((key!=12) || (key!=11)) && ((cursor!=2) || (cursor!=8) || (cursor!=11))) {
                    print(int_to_string(key));
                }

                } else if((cursor==15) && (key==12)) {
                    mode=2;
                    currentTime.hour = string_to_int(time);
                    currentTime.min = string_to_int(time[3]);
                    currentTime.sec = 0;
                    currentTime.month = string_to_int(time[6]);
                    currentTime.date = string_to_int(time[9]);
                    currentTime.year = string_to_int(time[12]);
                }
            } else continue;
        }
    }
    //Else
    //If frequency is not set
    if(1) { //NO CHECKING
        //mode = 2;
        //set_cursor(1, 0);
        //printdate();
        //set_cursor(0, 0);
        //input_freq(); //THIS IS LOOPED
        set_cursor(0);
        print_time();
        while(mode == 2) {
            if(keyPressed) {
                if((key==1) || (key==2) || (key==3)) {
                    freq_data = keyPressed;
                    int a[3] = {5, 30 ,60};
                    freq = a[freq_data-1];
                }
            }
        }
        i2c_write(_ROM, 0x1FFF, 1, freqData);
    }
    //Else
    mode = 3;
    while(1) {
        if(mode == 3) {
            //Data Logging
            
        }
        else if(mode == 4) {
            //Data Display

        }
        else print("Unknown Mode");
    }
    return;
}
/* ************ LCD **************** */
void lcd_init() {
    __delay_ms(15); //delay();
    send4ToLCD(0x0003);
    __delay_ms(5);
    send4ToLCD(0x0003);
    __delay_us(100);
    send4ToLCD(0x0003);
    send4ToLCD(0x0002);
    send8ToLCD(0x0002,0x0008);
    send8ToLCD(0x0000,0x0008);
    send8ToLCD(0x0000,0x0001);
    __delay_ms(15);
    send8ToLCD(0x0000,0x0006);
    send8ToLCD(0x0000,0x000F);
    return;
}

void send4ToLCD(int a) {
    LCD_RS = 0; //LATB = 0x0000;
    LCD_EN = 0;
    LCD_0 = (a & 0x8) >> 3;
    LCD_1 = (a & 0x4) >> 2;
    LCD_2 = (a & 0x2) >> 1;
    LCD_3 = (a & 0x1);  //LATB = W0;
    LCD_EN = 1;
    LCD_EN = 0;
    __delay_ms(5); //delay();
    return;
}

void send8ToLCD(int a, int b) {
    LCD_RS = 0; //LATB = 0x0000;
    LCD_EN = 0;
    LCD_0 = (a & 0x8) >> 3;
    LCD_1 = (a & 0x4) >> 2;
    LCD_2 = (a & 0x2) >> 1;
    LCD_3 = (a & 0x1);  //LATB = W0;
    LCD_EN = 1;
    LCD_EN = 0;
    LCD_RS = 0; //LATB = 0x0000;
    LCD_EN = 0;
    LCD_0 = (b & 0x8) >> 3;
    LCD_1 = (b & 0x4) >> 2;
    LCD_2 = (b & 0x2) >> 1;
    LCD_3 = (b & 0x1);  //LATB = W0;
    LCD_EN = 1;
    LCD_EN = 0;
    __delay_ms(5); //delay();
    return;
}

void write8ToLCD(int a, int b) {
    LCD_RS = 0; //LATB = 0x0000;
    LCD_EN = 0;
    LCD_0 = (a & 0x8) >> 3;
    LCD_1 = (a & 0x4) >> 2;
    LCD_2 = (a & 0x2) >> 1;
    LCD_3 = (a & 0x1);  //LATB = W0;
    LCD_RS = 1;
    LCD_EN = 1;
    LCD_EN = 0;
    LCD_RS = 0;
    LCD_0 = (b & 0x8) >> 3;
    LCD_1 = (b & 0x4) >> 2;
    LCD_2 = (b & 0x2) >> 1;
    LCD_3 = (b & 0x1);  //LATB = W0;
    LCD_RS = 1;
    LCD_EN = 1;
    LCD_EN = 0;
    LCD_RS = 0; 
    __delay_ms(20); //delay();
    return;
}

void print(char* str) {
    int i = 0;
    int W0 = 0;
    int W1 = 0;
    char a = *(str+i);
    for(i = 0; a != 0; i++) {
        W0 = (int)((*(str+i) & 0xF0) >> 4);
        W1 = (int)(*(str+i) & 0xF);
        a = *(str+i+1);
        write8ToLCD(W0, W1);
    }
    return;
} // end function

void clear_LCD() {
    send8ToLCD(0x0000,0x0001);
    send8ToLCD(0x0000,0x0006);
    return;
}

void LCD_clearLine(int a) {
    if (a == 0){
        set_cursor(0);
        print("                ");
    } else if (a == 1){
        set_cursor(1);
        print("                ");
    }
    return;
}

void set_cursor(int a) {
    if (a == 0){
        send8ToLCD(0x0008, 0x0000); //1st line
    }
    
    else if(a == 1){
        send8ToLCD(0x000C, 0x0000); //2nd line
    }
    return;
}

void LCD_moveCursor(int a) {
    if(a == 0) {
        send8ToLCD(0x0001, 0x0000); //MOVE LEFT
    } else if (a == 1) {
        send8ToLCD(0x0001, 0x0004); //MOVE RIGHT
    }
    return;
}


/******** I2C BASICS **********/

void i2c_write(int type, int addr, int size, char data[]) {
    int i;
    i2c_start();
    //print("s");
    if(type == _RTC) {
        i2c_tx(RTC_ADDR << 1);
        i2c_tx(addr);
    }
    if(type == _ROM) {
        i2c_tx(ROM_ADDR << 1);
        //print("h");
        i2c_tx((addr & 0xF0) >> 4);
        i2c_tx(addr & 0xF);
    }
    for(i = 0; i < size; i++) {
        i2c_tx((int)data[i]);
    }
    i2c_stop();
    __delay_ms(30);
    return;
}

void i2c_read(int type, int addr, int size, char dest[]) {
    int i;
    i2c_start();
    if(type == _RTC) {
        i2c_tx(RTC_ADDR << 1);
        i2c_tx(addr);
    }
    if(type == _ROM) {
        i2c_tx(ROM_ADDR << 1);
        i2c_tx((addr & 0xF0) >> 4);
        i2c_tx(addr & 0xF);
    }
    //wait_idle();
    I2C1CONbits.RSEN = 1;
    __delay_ms(30);
    //i2c_tx((ROM_));
    if(type == _RTC) i2c_tx((RTC_ADDR << 1) + 1);
    if(type == _ROM) i2c_tx((ROM_ADDR << 1) + 1);
    //print("3");
    print(int_to_string((ROM_ADDR << 1) + 1));
    for(i = 0; i < size-1; i++) {
        dest[i] = (char)i2c_rx();
        //dest[i+1] = 0;
        //print(dest);
        //print("P");
        //print(int_to_string(3));
        wait_idle();
        __delay_ms(50);
        ACK();
    }
    //print("O");
    dest[i] = (char)i2c_rx();
    //print("R");
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
    //IFS1bits.SI2C1IF=0;// clear interrupt flag for I2C
    
    // ##### I2C configuration #########################
    I2C1CONbits.I2CSIDL =1;
    I2C1CONbits.DISSLW = 1;
    I2C1BRG= 4;
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
    
    //print("i");
    idle=0;// set flag to declare you're not free/idle    
    I2C1TRN = data; // load data for transmission over the I2C bus
    while (!tx_complete);  // wait until TX is complete  
    //print("t");
    while (!acked); // wait until ACK is received
    reset_flags();// reset flags   
    //print("k");
    __delay_ms(100);
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
    //__delay_ms(50);
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

/* ************ UTILITIES **************** */

char int_to_char(int a) {
    char c;
    c = a + '0';
    return c;
}

char* int_to_string(int a) {
    int digits;
    int backup = a;
    for(digits = 0; backup; digits++) {
        backup = backup/10;
    }
    char* str;
    str[digits] = 0; //NULL
    for(; a; digits--) {
        str[digits-1] = int_to_char(a%10);  
        a = a/10;
    }
    return str;
}