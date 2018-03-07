/* ************ ADC **************** */

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
    adcvalue = ADC1BUF0;
    IEC0bits.AD1IE = 1;//Enable interrupt
    IFS0bits.AD1IF = 0;//Clear flag
}