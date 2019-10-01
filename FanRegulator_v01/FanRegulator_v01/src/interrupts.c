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
	
	//prescaler 4096
	TCCR1 = 0;
	TCCR1 |= (1<<CS10);
	TCCR1 |= (1<<CS12);
	TCCR1 |= (1<<CS13);

	//enable counter interrupt
	TIMSK |= (1<<TOIE1);
}

void setOvf(float sec) {
	
	seconds = sec;
}

float getOvf(void) {
	
	return seconds;
}

ISR(TIMER1_OVF_vect) {
	
	//callback
	onCheck();
}

ISR(PCINT0_vect) {
	
	//callback
	onChange();
}

	