/*
 * pwm.c
 *
 * Tim Breuer september 2019
 */ 

#include "pwm.h"

//local variables
uint8_t power;
uint32_t frequenz;

//public functions
void pwmInit() {
	
	//enable fast pwm
	TCCR0A |= (1<<WGM02)|(1<<WGM01)|(1<<WGM00)|(1<<COM0B1);

	//frequenz 10kHz
	TCCR0B = 0;
	TCCR0B |= (1<<WGM02)|(1 << CS00);

	//top value
	OCR0A = 100;
}

void setFrequency(uint32_t f) {
	
	frequenz = f;
}

uint32_t getFrequency() {
	
	return frequenz;
}

void setDuCy(uint8_t p) {
	
	//duty-cicle
	power = p;
	OCR0B = power;
}

uint8_t getDuCy() {
	
	return power;
}
