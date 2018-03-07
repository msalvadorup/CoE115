#include "xc.h"
#define FCY 2000000
#include <libpic30.h>

#pragma config FWDTEN=OFF, JTAGEN=OFF
#pragma config POSCMOD=NONE, OSCIOFNC=ON, FCKSM=CSDCMD, FNOSC=FRCPLL 
#pragma config I2C1SEL=PRI

#define LCD_RS LATBbits.LATB13
#define LCD_EN LATBbits.LATB14
#define LCD_0 LATBbits.LATB7
#define LCD_1 LATBbits.LATB5
#define LCD_2 LATBbits.LATB4
#define LCD_3 LATBbits.LATB3

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
    //LCD_RS = 0; //LATB = 0x0000;
    //LCD_EN = 0;
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
    //LCD_EN = 1;
    //LCD_EN = 0;
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
//LCD Character Mapping
//Column and row numbers correspond to high address and low address of the character, respectively (in decimal form)
//In the data sheet, binary format was used.
	/*
                             // 0   1   2   3   4   5   6   7    
    char LCD_array[16][9] = { {' ',' ',' ','0','@','P',' ','p'}, //0
                              {' ',' ','!','1','A','Q','a','q'}, //1
                              {' ',' ','"','2','B','R','b','r'}, //2
                              {' ',' ','#','3','C','S','c','s'}, //3
                              {' ',' ','$','4','D','T','d','t'}, //4
                              {' ',' ','%','5','E','U','e','u'}, //5
                              {' ',' ','&','6','F','V','f','v'}, //6
                              {' ',' ',' ','7','G','W','g','w'}, //7
                              {' ',' ','(','8','H','X','h','x'}, //8
                              {' ',' ',')','9','I','Y','i','y'}, //9
                              {' ',' ','*',':','J','Z','j','z'}, //10
                              {' ',' ','+',';','K','[','k','{'}, //11
                              {' ',' ',',','<','L',' ','l','|'}, //12
                              {' ',' ','-','=','M',']','m','}'}, //13
                              {' ',' ','.','>','N','^','n',' '}, //14
                              {' ',' ','/','?','O','_','o',' '}};  //15
                           

int a = 0; //row
int b = 0; //column
int i = 0; //iterates

//hiniwalay lang yung pag-identify sa numbers kasi kailangan mabilis ung paghanap sa numerical characters.
//mabilis kasi pagbago ng ADC input, parang si *ano* lang, mabilis magbago...... =))
    for (i = 0; outdata[i]!='\0'; i++){

        if(outdata[i] == ' '){
            W0 = 2;
            W1= 0;
        }
        
        else {
            for(a = 0; a < 16 ; a++){     
                for(b = 0; b < 8 ; b++){
                    if( LCD_array[a][b] == outdata[i] ){
                        W0 = b;
                        W1 = a;
                        goto write;
                    }
                }
            }
        }
        
        write:
        write8ToLCD(W0,W1);
       
    } // end for    
    */
    int i = 0;
    int W0 = 0;
    int W1 = 0;
    char a = *(str+i);
    for(i = 0; a != 0; i++) {
    	W0 = (int)((*(str+i) & 0xF0) >> 4);
    	W1 = (int)(*(str+i) & 0xF);
    	a = *(str+i);
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
    //LCD_setCursor(0); // places the cursor on the 1st line of LCD
/*
   // char cur_ctr[5];
    if (a == 0){
        if (mov_ctr == 0) {
            mov_ctr = 0;
        }
        else {
            send8ToLCD(0x0001, 0x0000); // moves cursor to the left
            mov_ctr--;         
        }
        //LATBbits.LATB10=1; // LED is ON
        //__delay_ms(500);
        //LATBbits.LATB11=0; // LED is OFF
        //__delay_ms(500);
    }
    
    else if(a == 1){
        if (mov_ctr == 15) {
            //LCD_write("conf_page");
            //mov_ctr == 15;
            LCD_clearLine(1);
            LCD_setCursor(0);
            LCD_write("Conf page:");
            LCD_setCursor(1);
            LCD_write("Yes = #, No = *");
            
        }
        else {
            send8ToLCD(0x0001, 0x0004); // moves cursor to the right
            mov_ctr++;
        }
        //LATBbits.LATB10=0; // LED is ON
        //__delay_ms(500);
        //LATBbits.LATB11=1; // LED is OFF
        //__delay_ms(500);
    }
*/

	if(a == 0) {
		send8ToLCD(0x0001, 0x0000); //MOVE LEFT
	} else if (a == 1) {
		send8ToLCD(0x0001, 0x0004); //MOVE RIGHT
	}
	return;
}


void main() {
	set_cursor(0);
	print("HH:mm MM/DD/YYYY");
	set_cursor(1);
	print("11:17");
	return;
}