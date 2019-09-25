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
uint8_t control(uint16_t, uint16_t);
void setKp(double);
void setKi(double);
void setKd(double);
void setDt(int);

#endif /* PID_H_ */