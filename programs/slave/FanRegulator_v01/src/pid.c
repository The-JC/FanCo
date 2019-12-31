/*
 * pid.c
 *
 *  Tim Breuer september 2019
 */ 

#include "pid.h"

//local variables
float Kp, Kd, Ki, dt;
int integral, previousError;
uint16_t max_speed;

//public functions
void pidInit(float p, float i, float d, uint16_t mp) {
	
	//default
	integral = 0;
	previousError = 0;
	Kp = p;
	Ki = i;
	Kd = d;
	max_speed = mp;
}

uint8_t control(uint16_t x, uint16_t w) {

	//init variables
	int derivative, out, e;	//overflow bug derivative (int to long)
	
	//control error
	e = w-x;
	
	//factor
	int factor = max_speed/100;
	
	//integral
	integral += e*dt/factor;
	if(integral>100) integral=100;
	if(integral<-100) integral=-100;
	
	//derivative
	derivative = (e - previousError) / (dt*factor);
	previousError = e;
	
	//sum up control values
	out = Kp*e/factor + integral*Ki + derivative * Kd;
	
	//scale
	if(out>100) out=100;
	if(out<0) out=0;
	
	return out;
}

void setKp(float kp) {
	
	//set p value
	Kp = kp;
}

void setKi(float ki) {

	//set i value
	Ki = ki;
}

void setKd(float kd) {
	
	//set d value
	Kd = kd;
}

void setDt(float t) {
	
	//set refresh speed
	dt = t;
}
