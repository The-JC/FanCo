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

//prototypes
void pinChangeInit(uint8_t);
void ovfInit(float);
float getOvf(void);
void (*onChange)(void);
void (*onCheck)(void);

#endif /* INTERRUPTS_H_ */