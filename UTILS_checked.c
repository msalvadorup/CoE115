/* ************ UTILITIES **************** */

char int_to_char(int a) {
	char c;
	c = a + '0';
	return c;
}

const char* int_to_string(int a) {
	int digits;
	int backup = a;
	for(digits = 0; backup; digits++) {
		backup = backup/10;
	}
	char* str;
	str[digits] = 0; //NULL
	for(; a; digits--) {
		str[digits-1] = int_to_char(a%10);	
		a = a/10;
	}
	return str;
}