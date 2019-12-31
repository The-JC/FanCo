/*
 * config.h
 *
 *  Tim Breuer september 2019
 */ 

#ifndef CONFIG_H_
#define CONFIG_H_

//adress
#define ADRESS 0x04

//pwm
#define PWM_FREQUENCY 10000

//fan specific
#define COUNTS_PER_ROTATION 3
#define MAX_SPEED 12000

//calculation speed in sec
#define REFRESH_SPEED 0.5

//pid values
#define KP 0.7
#define KI 0.5
#define KD 0.0

//do not change
#define F_CPU 1000000
#define PWM_PRESCALER 1
#define PWM_PIN 3;

#endif /* CONFIG_H_ */