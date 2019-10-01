/*
 * pid.h
 *
 * Tim Breuer september 2019
 */ 


#ifndef PID_H_
#define PID_H_

//includes
#include <avr/io.h>

//prototypes
void pidInit(float, float, float);
uint8_t control(uint16_t, uint16_t);
void setKp(float);
void setKi(float);
void setKd(float);
void setDt(float);

#endif /* PID_H_ */