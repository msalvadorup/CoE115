#include "xc.h"
#define FCY 2000000
#include <libpic30.h>
#include <p24FJ64GB002.h>

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
#define LCD_RS LATBbits.LATB3
#define LCD_EN LATBbits.LATB4
#define LCD_0 LATBbits.LATB10
#define LCD_1 LATBbits.LATB11
#define LCD_2 LATBbits.LATB12
#define LCD_3 LATBbits.LATB13
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


//Function Prototypes
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
//Keypad
void keypad_init();
//ADC
void adc_init(); //OK
float lm35temp(int a);	//OK
//I2C
time get_time();
void print_time(); //OK
//int bcd_to_dec();
void store_datapoint(); //OK
void print_datapoint(); //OK
void get_lastaddr(); //OK
void store_lastaddr(); //OK
void i2c_beginTransmission(int dev_address); //OK
void i2c_endTransmission(); //OK

//Modes
void input_time();
void confirmation();
void input_freq();
//Utilities
char int_to_char(int a); //OK
char* int_to_string(int a); //OK
//void delayus();
//void delayms();
//void delay();



//Keypad Interrupt



//I2C Interrupt



//Main
int main(void) {
	//Initialization
	adc_init();
	lcd_init();
	keypad_init();
	i2c_init();

	//If clock isn't initialized
	if() {
		mode = 1;
		//set_cursor(0, 1);
		//print("HH:mm DD/MM/YYYY");
		//set_cursor(0, 0);
		input_time(); //THIS IS LOOPED
	}
	//Else
	//If frequency is not set
	if() {
		mode = 2;
		//set_cursor(1, 0);
		//printdate();
		//set_cursor(0, 0);
		input_freq(); //THIS IS LOOPED
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
	return 0;
}