/* ************ KEYPAD **************** */

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