#include <REG51F.H>

void main(){
	unsigned char auxP1 = 0x00;
	P1 = auxP1;
	while (1){
		// Analise do bit_0 de P2
		if (P2&0x01) {
			auxP1 = (auxP1&0xf0)|(P0&0x0f);
			P1 = auxP1;
		}
		else {
			auxP1 = (auxP1&0xf0);
			P1 = auxP1;
		}
		
		// Analise do bit_1 de P2
		if (P2&0x02) {
			auxP1 = (auxP1&0x0f)|(P0&0xf0);
			P1 = auxP1;
		}
		else {
			auxP1 = (auxP1&0x0f);
			P1 = auxP1;
		}
	}
}