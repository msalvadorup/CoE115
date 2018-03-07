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
#define col1 LATBbits.LATB0
#define col2 LATBbits.LATB1
#define col3 LATBbits.LATB2
#define FIRST_PTR 0x1000
#define LAST_PTR 0x1003
#define ROM_ADDR 0x50
#define RTC_ADDR 0x68
#define _RTC 1
#define _ROM 0


//Constants
#define DEB_MAX 1000
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
int keyPressed = 0;
int key = 13;

int zero = 0;
int one = 0;
int two = 0;
int three = 0;
int four = 0;
int five = 0;
int six = 0;
int seven = 0;
int eight = 0;
int nine = 0;
int ast = 0;
int hash = 0;

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
int freq_data = 0;
//char test[8];


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
void adc_int();
void keypad_init();
int freq_data;
char freqData[3];

int main() {
    /*
    //TRISA = 0xFF;
    TRISB = 0x8300;
    lcd_init();
    //print("2");
    i2c_init();
    //print("2");
    keypad_init();
    //print("2");
    //set_cursor(1);
    //char* test;
    //print("HEY");
    char a[] = "A";
        //print("2");
    
    char* b;
        //print("2");
    //i2c_write(0x10, 0x00, a);
    //i2c_read(0x10, 0x00, b);
    i2c_write(_ROM, 0x1000, 8, a);
        //print("2");
    //print("w");

    print(int_to_string((int)lm35temp(adcValue))
    char test[8];
    //clear_LCD();
    i2c_read(_ROM, 0x1000, 8, test);
    //print("mark");
    //print(int_to_string(9));
    //print(int_to_string(9));
    //print("ROM");
    while(1){
        if(keyPress == 1) {
            print("P");
            print(int_to_string(keyf));
            keyPress = 0;
        }
    }



    //I2C1BRG = 19;
    //I2C1CONbits.DISSLW = 1;
    //i2c_write(_RTC, 0x00, 2, a);
    //print("WR");
    //i2c_read(_RTC, 0x00, 2, test);
    print(test);    
    //print("d");
    //print(b);
    print(test);
    while(1);
        return;
        */
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
        int freq_data;
        char freqData[2];
        time[2] = 0;
        time[5] = 0;
        time[8] = 0;
        time[11] = 0;
        while(mode == 1) {
            if(keyPressed) {
                if((key == 11) && (cursor!=0)) {
                    LCD_moveCursor(0); //MOVE LEFT
                    cursor--;
                } else if((key == 12) && (cursor!=15)) {
                    LCD_moveCursor(1); //MOVE RIGHT
                    cursor++;
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
        }            } else; //continue;

    
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
    //wait_idle();
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
    //print(int_to_string((ROM_ADDR << 1) + 1));
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
    char str_int[digits+1];
    str_int[digits] = 0; //NULL
    for(; a; digits--) {
        str_int[digits-1] = int_to_char(a%10);  
        a = a/10;
    }
    return str_int;
}


void keypad_init() {
    CNPU1 = CNPU1 | 0x000C;
    CNPU2 = CNPU2 | 0x6000;
    
    /* Enable interrupts and clear IRQ flag 
    */

    CNEN1 = CNEN1 | 0x000C;
    CNEN2 = CNEN2 | 0x6000;
        
    IEC1bits.CNIE = 1; 
    IFS1bits.CNIF = 0;
    
    LATB = LATB | 0x0;
}

void __attribute__((interrupt)) _CNInterrupt(void) {
    IEC1bits.CNIE = 0; 
    int deb_ctr = 0; //debounce counter
    
    if (!PORTAbits.RA0) {
        // Software debounce 
        for (deb_ctr=0;(!PORTAbits.RA0) && (deb_ctr < DEB_MAX);){
            deb_ctr++;
        }
        if (deb_ctr == DEB_MAX) {
            LATBbits.LATB0 = 0;
            LATBbits.LATB1 = 1;
            LATBbits.LATB2 = 1;
            for (deb_ctr=0;(!PORTAbits.RA0) && (deb_ctr < DEB_MAX);){
                    deb_ctr++;
            }
            if (deb_ctr == DEB_MAX) output(1, 1);
            else {
                LATBbits.LATB0 = 1;
                LATBbits.LATB1 = 0;
                LATBbits.LATB2 = 1;
                for (deb_ctr=0;(!PORTAbits.RA0) && (deb_ctr < DEB_MAX);){
                    deb_ctr++;
                }
                if (deb_ctr == DEB_MAX) output(2, 1);
                else {
                    LATBbits.LATB0 = 1;
                    LATBbits.LATB1 = 1;
                    LATBbits.LATB2 = 0;
                    for (deb_ctr=0;(!PORTAbits.RA0) && (deb_ctr < DEB_MAX);){
                        deb_ctr++;
                    }
                    if (deb_ctr == DEB_MAX) output(3, 1);
                }
            }
        }
    } else if (!PORTAbits.RA1){
        for (deb_ctr=0;(!PORTAbits.RA1) && (deb_ctr < DEB_MAX);){
            deb_ctr++;
        }
        if (deb_ctr == DEB_MAX) {
            LATBbits.LATB0 = 0;
            LATBbits.LATB1 = 1;
            LATBbits.LATB2 = 1;
            for (deb_ctr=0;(!PORTAbits.RA1) && (deb_ctr < DEB_MAX);){
                deb_ctr++;
            }
            if (deb_ctr == DEB_MAX) output(4, 1);
            else {
                LATBbits.LATB0 = 1;
                LATBbits.LATB1 = 0;
                LATBbits.LATB2 = 1;
                for (deb_ctr=0;(!PORTAbits.RA1) && (deb_ctr < DEB_MAX);){
                    deb_ctr++;
                }
                if (deb_ctr == DEB_MAX) output(5, 1);
                else {
                     LATBbits.LATB0 = 1;
                    LATBbits.LATB1 = 1;
                    LATBbits.LATB2 = 0;
                    for (deb_ctr=0;(!PORTAbits.RA1) && (deb_ctr < DEB_MAX);){
                        deb_ctr++;
                    }
                    if (deb_ctr == DEB_MAX) output(6, 1);
                }
            }
        }
        /*
        if (deb_ctr == DEB_MAX)
            row2_press = 1; //set flag
        else
            row2_press = 0;
        */
    } else if(!PORTAbits.RA2) {
            for (deb_ctr=0;(!PORTAbits.RA2) && (deb_ctr < DEB_MAX);){
                deb_ctr++;
        }
        if (deb_ctr == DEB_MAX) {
            LATBbits.LATB0 = 0;
            LATBbits.LATB1 = 1;
            LATBbits.LATB2 = 1;
            for (deb_ctr=0;(!PORTAbits.RA2) && (deb_ctr < DEB_MAX);){
            deb_ctr++;
            }
            if (deb_ctr == DEB_MAX) output(7, 1);
            else {
                LATBbits.LATB0 = 1;
                LATBbits.LATB1 = 0;
                LATBbits.LATB2 = 1;
                for (deb_ctr=0;(!PORTAbits.RA2) && (deb_ctr < DEB_MAX);){
                    deb_ctr++;
                }
                if (deb_ctr == DEB_MAX) output(8, 1);
                else {
                    LATBbits.LATB0 = 1;
                    LATBbits.LATB1 = 1;
                    LATBbits.LATB2 = 0;
                    for (deb_ctr=0;(!PORTAbits.RA2) && (deb_ctr < DEB_MAX);){
                        deb_ctr++;
                    }
                    if (deb_ctr == DEB_MAX) output(9, 1);
                }
                }
            }
    
    } else if(!PORTAbits.RA3) {
            for (deb_ctr=0;(!PORTAbits.RA3) && (deb_ctr < DEB_MAX);){
                deb_ctr++;
        }
        if (deb_ctr == DEB_MAX) {
            LATBbits.LATB0 = 0;
            LATBbits.LATB1 = 1;
            LATBbits.LATB2 = 1;
            for (deb_ctr=0;(!PORTAbits.RA3) && (deb_ctr < DEB_MAX);){
            deb_ctr++;
            }
            if (deb_ctr == DEB_MAX) output(11);
            else {
                LATBbits.LATB0 = 1;
                LATBbits.LATB1 = 0;
                LATBbits.LATB2 = 1;
                for (deb_ctr=0;(!PORTAbits.RA3) && (deb_ctr < DEB_MAX);){
                    deb_ctr++;
                }
                if (deb_ctr == DEB_MAX) output(0);
                else {
                   LATBbits.LATB0 = 1;
                   LATBbits.LATB1 = 1;
                   LATBbits.LATB2 = 0;
                   for (deb_ctr=0;(!PORTAbits.RA3) && (deb_ctr < DEB_MAX);){
                       deb_ctr++;
                   }
                   if (deb_ctr == DEB_MAX) output(12);
                }
            }
        }
    }
    //print("ch");
    //while(!PORTAbits.RA3 | !PORTAbits.RA2 | !PORTAbits.RA1 | !PORTAbits.RA0) {
    //}
    //print("ok");
    LATBbits.LATB0 = 0;
    LATBbits.LATB1 = 0;
    LATBbits.LATB2 = 0;
 /* Clear IRQ flag */
    IEC1bits.CNIE = 1; 
    IFS1bits.CNIF = 0;
    return;
}

void output(int keyp, int keyflag) {
    //print("K");
    //print(int_to_string(keyp)); 
    key = keyp;
    keyPressed = keyflag;
    return;
}


float lm35temp(int a) {
    float temp = (float)a;
    temp = (temp*500)/1023;
    return temp;
}

//FROM ME1
void adc_init() {
    AD1PCFG = 0xfdff; // 1111 1101 1111 1111
    //AD1
    //ADSIDL = 1 to discontinue module op pag idle mode
    //form <1:0> = 00 for integer output
    //SSRC <2:0> = 111 for trigger n auto converter        
    ///AD1CON1bits.FORM = 0x0;
    ///AD1CON1bits.SSRC = 0x7;
    ///AD1CON1bits.ADSIDL = 1;
    AD1CON1 = 0x20E6; //*old is 0x00e0
    //AD1CON2
    //CSCNA = 0 so di sya magscan inputs
    //SMPI = 0000 for interrupts at the end of each sample/convert sequence
    //ALTS = 0 for always use MUXA input multiplexer settings
    //BUFM = 0 for buffer is configured as one 16-word buffer
    
    ///AD1CON2bits.CSCNA = 0;
    ///AD1CON2bits.SMPI = 0x0;
    ///AD1CON2bits.ALTS = 0;
    ///AD1CON2bits.BUFM = 0;
    
    AD1CON2 = 0x0000;
    
    //AD1CON3
    //ADRC = 0 to use system clock
    //AD1CON3 <12:8> set to 2Tad ->> SAMC<4:0> = 00010
    //AD1CON3 <7:0> set to 2Tcy ->> ADCS<7:0> = 0x01
    
    ///AD1CON3bits.ADRC = 0;
    ///AD1CON3bits.SAMC = 0x02;
    ///AD1CON3bits.ADCS = 0x01;
    AD1CON3 = 0x0FFF;
    //AD1CHS
    //CH0SA = 0x09 Channel 0 positive input is AN9 for MUXA
    //all other bits 0
    //0000 0000 0000 1001
    AD1CHS = 0x0009;
    
    //AD1CSSL
    //set to 0
    AD1CSSL = 0x0;
    IEC0bits.AD1IE = 1; //enable interrupt
    IFS0bits.AD1IF = 0; //clear interrupt flag
    AD1CON1bits.ADON = 1; //turn on ADC
}

//FROM ME 1
void __attribute__ ((interrupt, no_auto_psv)) _ADC1Interrupt(void){
    IEC0bits.AD1IE = 0; //Disable interrupt
    IFS0bits.AD1IF = 0;//Clear flag
    adcValue = ADC1BUF0;
    IEC0bits.AD1IE = 1;//Enable interrupt
    IFS0bits.AD1IF = 0;//Clear flag
}