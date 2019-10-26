/*
 * config.h
 *
 *  Tim Breuer september 2019
 */ 

#ifndef CONFIG_H_
#define CONFIG_H_

//adress
#define ADRESS 0x04

//prescaler
#define TIMER_PRESCALER 4096
#define PWM_PRESCALER 1

//pwm
#define PWM_FREQUENCY 10000

//fan specific
#define COUNTS_PER_ROTATION 2
#define MAX_SPEED 2000

//calculation speed in sec
#define REFRESH_SPEED 1

//pid values
#define KP 0.9
#define KI 0.7
#define KD 0.05

//do not change
#define F_CPU 1000000

#endif /* CONFIG_H_ */