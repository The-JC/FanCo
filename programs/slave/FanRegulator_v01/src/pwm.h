/*
 * pwm.h
 *
 * Tim Breuer september 2019
 */ 

/*
	[]	change pwm frequenz
*/


#ifndef PWM_H_
#define PWM_H_

//includes
#include <avr/io.h>
#include <avr/interrupt.h>

//prototypes
void pwmInit(void);
void setFrequency(uint32_t);
uint32_t getFrequency(void);
void setDuCy(uint8_t);
uint8_t getDuCy(void);

#endif /* PWM_H_ */