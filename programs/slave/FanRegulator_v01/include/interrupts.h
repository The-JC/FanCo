/*
 * interrupts.h
 *
 * Tim Breuer september 2019
 */ 


#ifndef INTERRUPTS_H_
#define INTERRUPTS_H_

//includes
#include <avr/io.h>
#include <avr/interrupt.h>

//initialisation and allocation of pin for Pin-Change-Interrupt
void pinChangeInit(uint8_t pin);
//initialisation of Timer-Interrupt for calculation
void ovfInit(void);
//Pin-Change-Interrupt Callback
void (*onChange)(void);
//Timer-Interrupt Callback for calculation
void (*onCheck)(void);

#endif /* INTERRUPTS_H_ */