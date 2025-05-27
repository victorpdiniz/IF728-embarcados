#include <REG51F.H>

// Constants
sbit bit_zero = P2^0;
sbit bit_one = P2^1;
const long int timeout = 7750;
unsigned char aux = 0x00;

// Read bit_one from P2
void read_one(){ 
		static long int timer = 0;
		static char state = '2';
	
	  switch(state){
			case '0': 
				if(bit_one){
					aux = (aux & 0x0f) | (P0 & 0xf0);
					P1 = aux;
					state = '1';
				}
				break;
			case '1': 
				timer++;
				if (timer > timeout) {
					timer = 0;
					aux = (aux & 0x0f);
					P1 = aux;
					state = '2';
				}
				break;
			case '2':
				if(~bit_one){
					state = '0';
				}
				break;
			default:
				break;
		}
}

// Read bit_zero from P2
void read_zero() {
		static long int timer = 0;
		static char state = '2';
		
		switch(state) {
			case '0': 
				if (bit_zero) {
					aux = (aux & 0xf0) | (P0 & 0x0f);
					P1 = aux;
					state = '1';
				}
				break;
			case '1': 
				timer++;
				if (timer > timeout) {
					timer = 0;
					aux = (aux & 0xf0);
					P1 = aux;
					state = '2';
				}
				break;
			case '2': 
				if (~bit_zero) {
					state = '0';
				}
				break;
			default:
				break;
		}
}
	
void main() {
		P1 = aux;

		while (1) {
			read_zero();
			read_one();
		}
}