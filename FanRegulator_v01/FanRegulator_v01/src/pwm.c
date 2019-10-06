/*
 * pwm.c
 *
 * Tim Breuer september 2019
 */ 

#include "pwm.h"
#include "config.h"

//local variables
uint8_t power, range;
uint32_t frequenz;

//public functions
void pwmInit(void) {
	
	//enable fast pwm
	TCCR0A |= (1<<WGM02)|(1<<WGM01)|(1<<WGM00)|(1<<COM0B1);

	//prescaler 1
	TCCR0B = 0;
	TCCR0B |= (1<<WGM02)|(1 << CS00);
}

void setFrequency(uint32_t f) {
	
	//frequency
	frequenz = f;
	
	//range
	range = F_CPU/(PWM_PRESCALER*frequenz);
	
	//top value
	OCR0A = range;
}

uint32_t getFrequency() {
	
	return frequenz;
}

void setDuCy(uint8_t p) {
	
	//duty-cicle
	power = (uint32_t)(range*p)/100;
	OCR0B = power;
}

uint8_t getDuCy(void) {
	
	return (uint32_t)(100*power)/range;
}
