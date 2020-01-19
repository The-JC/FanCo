/*
 * pid.h
 *
 * Tim Breuer september 2019
 */ 


#ifndef PID_H_
#define PID_H_

//includes
#include <avr/io.h>

//initialisation of pid regulation, set pid values and set maximal fan speed in rpm
void pidInit(float kp, float ki, float kd, uint16_t speed);
//calculate control error using x (actual value) and w (setpoint) also returns new PWM power in percent
uint8_t control(uint16_t x, uint16_t w);
//set new proportional value
void setKp(float kp);
//set new integral value
void setKi(float ki);
//set new differential value
void setKd(float kd);
//set new calculation time
void setDt(float time);

#endif /* PID_H_ */