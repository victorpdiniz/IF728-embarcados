#include <REG51F.H>

// Defines
#define TH1_L 204

void interface_serial_inicializa(){
	SM0 = 0;
	SM1 = 1;
	SM2 = 0;
	REN = 1;
	ES = 1;
}


void timer1_inicializa(){
	TR1 = 0;
	PCON |= 0X80;
	TMOD = (TMOD & 0x0F)|0x20;
	TH1 = TH1_L;
	TR1 = 1;
}

void interruptSerial(void) interrupt 4 using 2{
	unsigned char chr;
	
	if(TI){
		TI = 0;
	}
	if(RI){
		RI = 0;
		chr = SBUF;
		SBUF = chr + 1;
	}
}
void main(){
	timer1_inicializa();
	interface_serial_inicializa();
	EA = 1;
	while(1){};
}