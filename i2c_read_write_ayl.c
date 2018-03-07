
void i2c_write (unsigned int h_wr_addr, unsigned int l_wr_addr, char wr_data[]){
    /* Write to EEPROM
    Store "CoE_115" in address 0x1000 of the EEPROM 
    */     
    
    i2c_start();// send I2C start sequence 
    //writeToLCD("1");
    i2c_tx(cbyte_write);// send EEPROM CTRL byte with Write enabled
    //writeToLCD("2");
    i2c_tx(h_wr_addr); // send EEPROM high address
    //writeToLCD("3");
    i2c_tx(l_wr_addr); // send EEPROM low address     
    //writeToLCD("4");
    
    // send actual data; 
    for (i =0; wr_data[i] != '\0'; i++){
        i2c_tx((int)wr_data[i]);
    }
    
    i2c_tx('\0');
    //writeToLCD("5");
    i2c_stop(); // send I2C stop sequence
    //writeToLCD("6");
    delay(30); //30ms delay
    return;
}

void i2c_read (unsigned int h_re_addr, unsigned int l_re_addr, char re_data[]){
    
    /* Read from EEPROM
    * Get data written previously in the EEPROM
    */   
    i2c_start();// send I2C start sequence
    i2c_tx(cbyte_write);// send EEPROM CTRL byte with Write enabled
    //writeToLCD("7");
    i2c_tx(h_re_addr);// send EEPROM high address
    //writeToLCD("8");
    i2c_tx(l_re_addr);// send EEPROM low address
    //writeToLCD("9");
    I2C1CONbits.RSEN=1; 
    //writeToLCD("a");
    i2c_tx(cbyte_read);// send EEPROM CTRL byte with Read enabled
    // receive data
    // send ACK, but not on last byte
    //writeToLCD("b");
    for(j=0;j<BUFMAX;j++){
        if (j<BUFMAX-1){
            re_data[j]=i2c_rx();
            ACK();
        }
        if (j==BUFMAX-1){
            re_data[j]=i2c_rx();
            NACK();
            break;
        }
     
    }     
    //writeToLCD("c");
    i2c_stop();// send I2C stop sequence
    //writeToLCD(re_data);
    return;
}
