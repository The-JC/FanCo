/*
 * pid.c
 *
 *  Tim Breuer september 2019
 */ 

#include "pid.h"

//local variables
double Kp;
double Ki;
double Kd;

int integral;
int previousError;
int dt;

//public functions
uint8_t control(uint16_t x, uint16_t w) {

	//init variables
	int derivative, out;
	
	//control error
	int e = (w-(int32_t)x)/18;
	
	//integral
	integral += e*dt;
	
	//derivative
	derivative = (e - previousError) / dt;
	
	//sum up control values
	out = Kp*e + integral*Ki + derivative * Kd;
	
	//scale
	if(out>100) out=100;
	if(out<0) out=0;
	
	return out;
}

void setKp(double kp) {
	
	Kp = kp;
}

void setKi(double ki) {
	
	Ki = ki;
}

void setKd(double kd) {
	
	Kd = kd;
}

void setDt(int t) {
	
	dt = t;
}
