#include <stdio.h>

typedef struct {
	int hour;
	int min;
	int sec;
	int month;
	int date;
	int year;
} time_t ;

time_t currentTime = {12, 42, 12, 4, 30, 2017};
/*
currentTime.hour = 12;
currentTime.min = 42;
currentTime.sec = 12;
currentTime.month = 4;
currentTime.date = 30;
currentTime.year = 2017;
*/

float temperature = 26.1233;

char* store_datapoint() {
	char data[9];
	float temp = temperature;
	time_t t = currentTime;
	int i;

	for(i = 0; i <= 3; i++) {
	data[i] = *(((char*)(&temp))+i);
	}

	data[4] = (char)((t.hour << 3) + ((t.min & 0x3F) >> 3));
	data[5] = (char)(((t.min & 0x3) << 5) + (t.month << 1) + ((t.date & 0x10) >> 4));
	data[6] = (char)(((t.date & 0xF) << 4) + ((t.year & 0x0F00) >> 8));
	data[7] = (char)(t.year & 0xFF);
	data[8] = 0;

	//i2c_write(data);
	return data;
}



void print_datapoint() {
	char* raw;
	float temp;
	//raw = i2c_read(addr, 8);
	raw = store_datapoint();
	time_t t;

	temp = *((float*)raw);
	t.hour = (int)(((*(raw+4)) & 0xF8) >> 3);
	t.min = (int)(((*(raw + 4) & 0x7) << 3) + ((*(raw + 5) & 0xE0) >> 5));
	t.month = (int)((*(raw + 5) & 0x1E) >> 1);
	t.date = (int)(((*(raw + 5) & 0x1) << 4) + ((*(raw + 6) & 0xF0) >> 4));
	t.year = (int)(((*(raw + 6) & 0xF) << 8) + *(raw + 7));

	printf("%f", temp);
	//set_cursor(0, 1);
	print_time(t);
	return;
}


void print_time(time_t t) {
	set_cursor(0, 1);
	printf("%d",t.hour);
	printf(":");
	printf("%d",t.min);
	printf(" ");
	printf("%d",t.month);
	printf("/");
	printf("%d",t.date);
	printf("/");
	printf("%d",t.year);
	return;
}

int main() {
	print_datapoint();
	/*
	int i;
	for(i=0; i<8;i++){
	printf("%2x", *((char*)(store_datapoint()+i)));
	
	}*/
		
}

//ffffff85fffffffcffffffd0416549ffffffe7ffffffe1

