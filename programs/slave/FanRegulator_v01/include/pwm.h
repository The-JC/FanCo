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

//initialisation of PWM mode
void pwmInit(void);
//set new PWM frequency
void setFrequency(uint32_t frequency);
//get PWM frequency
uint32_t getFrequency(void);
//set new power of PWM in percent
void setDuCy(uint8_t power);
//get power of PWM in percent
uint8_t getDuCy(void);

#endif /* PWM_H_ */