#include "xc.h"
#define FCY 2000000
#include <libpic30.h>

#pragma config FWDTEN=OFF, JTAGEN=OFF
#pragma config POSCMOD=NONE, OSCIOFNC=ON, FCKSM=CSDCMD, FNOSC=FRCPLL 
#pragma config I2C1SEL=PRI  

#define LCD_RS LATBbits.LATB4
#define LCD_EN LATBbits.LATB5
#define LCD_0 LATBbits.LATB3
#define LCD_1 LATBbits.LATB2
#define LCD_2 LATBbits.LATB1
#define LCD_3 LATBbits.LATB0



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

int main() {
    TRISB = 0;
    lcd_init();
    set_cursor(0);
    print("PRINT");
    set_cursor(1);
    print("LATB");
    while(1);
    return;
}