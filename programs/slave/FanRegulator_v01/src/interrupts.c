/*
 * interrupts.c
 *
 * Tim Breuer september 2019
 */ 

#include "interrupts.h"

//local variables
float seconds;

//public functions
void pinChangeInit(uint8_t pin) {
	
	//enable pin change interrupt
	GIMSK |= (1<<PCIE);

	//interrupt pin
	switch(pin) {
		case 0:
			PCMSK |= (1<<PCINT0);
			break;
		case 1:
			PCMSK |= (1<<PCINT1);
			break;
		case 2:
			PCMSK |= (1<<PCINT2);
			break;
		case 3:
			PCMSK |= (1<<PCINT3);
			break;
		case 4:
			PCMSK |= (1<<PCINT4);
			break;
		case 5:
			PCMSK |= (1<<PCINT5);
			break;
	}
}

void ovfInit(float sec) {
	
	seconds = sec;
	
	//0.5 seconds
	TCCR1 = 0x8D;
	//1.0 seconds
	//TCCR1 = 0x8E;
	
	//compare
	OCR1A = 0x97;
	
	//enable compare interrupt
	TIMSK |= (1<<OCIE1A);
}

float getOvf(void) {
	
	return seconds;
}

ISR(TIMER1_COMPA_vect) {
	
	//lower priority
	sei();
	
	//callback
	onCheck();
}

ISR(PCINT0_vect) {
	
	//lower priority
	sei();
	
	//callback
	onChange();
}

	