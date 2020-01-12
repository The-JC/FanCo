/*
 * pwm.c
 *
 * Tim Breuer september 2019
 */ 

#include "pwm.h"
#include "config.h"

//local variables
uint8_t power, range;
uint32_t frequency;

//public functions
void pwmInit() {
	
	//enable fast pwm
	TCCR0A = 0x23;
	TCCR0B = 0x09;
	//pinout
	DDRB |= 0x01;
}

void setFrequency(uint32_t f) {
	
	//frequency
	frequency = f;
	//range
	range = F_CPU/(PWM_PRESCALER*frequency);
	//top value
	OCR0A = range;
}

uint32_t getFrequency() {
	
	return frequency;
}

void setDuCy(uint8_t p) {
	
	//set duty-cicle
	power = (uint32_t)(range*p)/100;
	OCR0B = power;
}

uint8_t getDuCy(void) {
	
	return (uint32_t)(100*power)/range;
}
