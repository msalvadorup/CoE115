
void i2c_write(int type, int addr, int size, char data[]) {
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
	//print("3");
	print(int_to_string((ROM_ADDR << 1) + 1));
	for(i = 0; i < size-1; i++) {
		dest[i] = (char)i2c_rx();
		//dest[i+1] = 0;
		//print(dest);
		print("P");
		//print(int_to_string(3));
		wait_idle();
		__delay_ms(50);
		ACK();
	}
	print("O");
	dest[i] = (char)i2c_rx();
	print("R");
	NACK();
	i2c_stop();
	__delay_ms(30);
	return;
}
