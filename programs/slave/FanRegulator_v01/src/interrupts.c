/*
 * interrupts.c
 *
 * Tim Breuer september 2019
 */ 

#include "interrupts.h"

//public functions
void pinChangeInit(uint8_t pin) {
	
	//enable pin change interrupt
	GIMSK |= (1<<PCIE);
	//interrupt pin
	if(pin<=5)
		PCMSK = (1<<pin);
}

void ovfInit() {
	
	//1 seconds; D for 0.5s
	TCCR1 = 0x8E;
	OCR1C = 0x79;
	//enable compare interrupt
	TIMSK |= (1<<OCIE1A);
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

	