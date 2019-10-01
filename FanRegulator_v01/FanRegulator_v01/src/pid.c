/*
 * pid.c
 *
 *  Tim Breuer september 2019
 */ 

#include "pid.h"

//local variables
float Kp, Kd, Ki, dt;
int integral, previousError;

//public functions
void pidInit(float p, float i, float d) {
	
	//set
	integral = 0;
	previousError = 0;
	
	Kp = p;
	Ki = i;
	Kd = d;
}

uint8_t control(uint16_t x, uint16_t w) {

	//init variables
	int derivative, out;
	
	//control error
	int e = (w-(int32_t)x)/18;
	
	//integral
	integral += e*dt;
	
	//derivative
	derivative = (e - previousError) / dt;
	previousError = e;
	
	//sum up control values
	out = Kp*e + integral*Ki + derivative * Kd;
	
	//scale
	if(out>100) out=100;
	if(out<0) out=0;
	
	return out;
}

void setKp(float kp) {
	
	Kp = kp;
}

void setKi(float ki) {
	
	Ki = ki;
}

void setKd(float kd) {
	
	Kd = kd;
}

void setDt(float t) {
	
	dt = t;
}
