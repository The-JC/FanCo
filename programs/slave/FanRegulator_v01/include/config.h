/*
 * config.h
 *
 *  Tim Breuer september 2019
 */ 

#ifndef CONFIG_H_
#define CONFIG_H_

//adress
#define ADDRESS 0x04

//pwm
#define PWM_FREQUENCY 10000

//fan specific
#define COUNTS_PER_ROTATION 3
#define MAX_SPEED 12000

//calculation speed in sec
#define REFRESH_SPEED 1

//pid values
#define KP 0.7
#define KI 0.5
#define KD 0.0
#define DEVIATION 100

//do not change
//#define F_CPU 1000000
#define PWM_PRESCALER 1
#define TACHO_PIN 3

#endif /* CONFIG_H_ */