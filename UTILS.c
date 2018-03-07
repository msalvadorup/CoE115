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
	char str[digits+1];
	str[digits] = 0; //NULL
	int ten_power;
	for(ten_power = 10; digits+1; digits--) {
		str[digits-1] = a%ten_power;
	}
	return str;
}

/*
void delay (long millisecs) {
    int delay = 0;
    delay = millisecs*400;
    while(delay) {
        delay--;
    }
    delay = millisecs*400;
}
*/