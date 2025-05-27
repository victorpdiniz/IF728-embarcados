#include <REG51F.H>

// Defines
#define FrClk 12000000
#define FreqTimer0_emHz 100
#define CORRECAO 9
#define TH0_Inicial ((65536-(FrClk/(12*FreqTimer0_emHz))+CORRECAO)>>8)
#define TL0_Inicial ((65536-(FrClk/(12*FreqTimer0_emHz))+CORRECAO)&0xFF)

// Constantes
sbit bit_zero = P2^0;
sbit bit_one = P2^1;
const char timeout = 100;
unsigned char aux = 0x00;

// Temporizadores
int timer_one = 0;
int timer_zero = 0;

void timer0_inicializa(){
	TR0 = 0; // Desliga Timer0
	TMOD = (TMOD & 0xF0)|0x01; // Timer 0 programado no Modo 1 (16 bits)
	TH0 = TH0_Inicial; // Programa valor de contagem do Timer0
	TL0 = TL0_Inicial;
	ET0 = 1; // Habilita interrupcao do timer 0
	TR0 = 1; // Habilita contagem do timer 0
}

void timer0_int(void) interrupt 1 using 2{
	TR0 = 0; // Desliga Timer0
	TL0 += TL0_Inicial; // Faz recarga da contagem do Timer0
	TH0 += TH0_Inicial + (unsigned char) CY;
	TR0 = 1; // Habilita contagem do timer 0
	timer_one++;
	timer_zero++;
}

// Read bit_one from P2
void read_one(){
	static char state = '2';
	switch(state){
		case '0': 
			if(bit_one){
				aux = (aux & 0x0f) | (P0 & 0xf0);
				P1 = aux;
				state = '1';
				timer_one = 0;
			}
			break;
		case '1': 
			if(timer_one > timeout){
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
		static char state = '2';
		
		switch(state) {
			case '0': 
				if (bit_zero) {
					aux = (aux & 0xf0) | (P0 & 0x0f);
					P1 = aux;
					state = '1';
					timer_zero = 0;
				}
				break;
			case '1': 
				if (timer_zero > timeout) {
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
		timer0_inicializa();
		EA=1; //Habilita o tratamento de interrupções
		while (1) {
			read_zero();
			read_one();
		}
}
