/* ************ I2C **************** */

void i2c_beginTransmission(int dev_address) {
	i2c_start();
	int ctrl_byte = dev_address << 1;
	i2c_tx(ctrl_byte);
	return;
}

void i2c_endTransmission() {
	i2c_stop();
	return;
}

time get_time() {

}

void print_time(time t) {
	set_cursor(0, 1);
	print(int_to_string(t.hour));
	print(":");
	print(int_to_string(t.min));
	print(" ");
	print(int_to_string(t.month));
	print("/");
	print(int_to_string(t.date));
	print("/");
	print(int_to_string(t.year));
	return;
}

void store_datapoint() {
	char data[9];
	float temp = temperature;
	time t = currentTime;

	for(i = 0; i <= 3; i++) {
	data[i] = *(((char*)(&temp))+i);
	}

	data[4] = (char)((t.hour << 3) + ((t.min & 0x3F) >> 3));
	data[5] = (char)(((t.min & 0x3) << 5) + (t.month << 1) + ((t.date & 0x10) >> 4));
	data[6] = (char)(((t.date & 0xF) << 4) + ((t.year & 0x0F00) >> 8));
	data[7] = (char)(t.year & 0xFF);
	data[8] = 0;

	i2c_write(data);
	return;

/*
	i2c_beginTransmission(ROM_ADDR);
	for(i = 0; i <= 3; i++) {
		data = *(((char*)(&temp))+i);
		i2c_tx(data);
	}

	data  = (char)((t.hour << 3) + ((t.min & 0x3F) >> 3));
	i2c_tx(data);
	data = (char)(((t.min & 0x3) << 5) + (t.month << 1) + ((t.date & 0x10) >> 4));
	i2c_tx(data);
	data = (char)(((t.date & 0xF) << 4) + ((t.year & 0x0F00) >> 8));
	i2c_tx(data);
	data = (char)(t.year & 0xFF);
	i2c_tx(data);

	i2c_endTransmission();
*/
}

void print_datapoint(int addr) {
	char* raw;
	float temp;
	raw = i2c_read(addr, 8);
	time t;

	temp = &((float*)raw);
	t.hour = (int)((&(raw + 4) & 0xF8) >> 3);
	t.min = (int)(((&(raw + 4) & 0x7) << 3) + (&(raw + 5) & 0xE0));
	t.month = (int)((&(raw + 5) & 0x1E) >> 1);
	t.date = (int)(((&(raw + 5) & 0x1) << 4) + ((&(raw + 6) & 0xF0) >> 4));
	t.year = (int)(((&(raw + 6) & 0xF) << 8) + &(raw + 7));

	print(int_to_string((int)temp));
	set_cursor(0, 1);
	print_time(t);
	return;
}

void get_lastaddr() {
	last_addr = *((int*)(i2c_read(LAST_PTR, 2)));
	return;
}

void store_lastaddr() {
	char* data = (char*)last_addr;
	&(data+2) = 0;
	i2c_write(LAST_PTR, data);
	return;
} 


char* i2c_read(int addr, int size) {   //RETURNS STRING, ADDS 0 WITHIN THE FUNCTION

}

void i2c_write(int addr, char* data) { //ACCEPTS STRING, DOESN'T SEND NULL
	
}