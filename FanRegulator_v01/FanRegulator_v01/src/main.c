/*
 * FanRegulator_v01.c
 *
 * Tim Breuer september 2019
 */ 

//includes
#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include "config.h"
#include "usiTwiSlave.h"
#include "pwm.h"
#include "interrupts.h"
#include "pid.h"

//init
uint16_t x, w;
uint8_t counter;
float factor;

//communication
uint8_t i2cRegister[2];
uint8_t master_data[3];
uint8_t master_bytes;

void receiveEvent(uint8_t num_bytes) {
	
	//receive data
	master_bytes = num_bytes;
	for (uint8_t i = 0; i < master_bytes; i++) {
		master_data[i] = usiTwiReceiveByte();
	}

	//mode
	uint16_t buffer;
	switch (master_data[0]) {
		case 0x00:
			//rpm
			buffer = master_data[1];
			buffer = (buffer << 8) + master_data[2];
			w = buffer;
			break;
		case 0x01:
			//p
			buffer = master_data[1];
			buffer = (buffer << 8) + master_data[2];
			float kp = buffer/(double)1000;
			setKp(kp);
			break;
		case 0x02:
			//i
			buffer = master_data[1];
			buffer = (buffer << 8) + master_data[2];
			float ki = buffer/(double)1000;
			setKi(ki);
			break;
		case 0x03:
			//d
			buffer = master_data[1];
			buffer = (buffer << 8) + master_data[2];
			float kd = buffer/(double)1000;
			setKd(kd);
			break;
	}
}

void requestEvent() {

	//send to master
	for(uint8_t i=0; i<2; i++) {
		usiTwiTransmitByte(i2cRegister[i]);
	}
}

void changeEvent() {
	
	//count
	counter++;
}

void checkEvent() {
	
	//calculate
	x = (counter/factor)*60000;
	counter = 0;
	
	//pid correction
	setDuCy(control(x, w));
}

int main(void) {
	
	//default
	w = 0;
	x = 0;
	counter = 0;
	
	//correction factor
	factor = 2*COUNTS_PER_ROTATION*(F_CPU/(float)TIMER_PRESCALER/256)*1000;
	
	//i2c init
    usiTwiSlaveInit(ADRESS);
	
	//i2c callback
	usi_onReceiverPtr = *receiveEvent;
	usi_onRequestPtr = *requestEvent;
	
	//interrupt callback
	onChange = *changeEvent;
	onCheck = *checkEvent;
	
	//pid
	pidInit(KP, KI, KD);
	setDt(REFRESH_SPEED);
	
	//disable interrupts
	cli();
	
	//pwm
	pwmInit();
	setFrequency(PWM_FREQUENCY);

	//pin change
	pinChangeInit(3);
	
	//timer interrupt
	ovfInit(REFRESH_SPEED);
	
	//enable interrupts
	sei();
	
	//pinout
	DDRB |= (1<<DDB1);
	
    while (1) {
		
		//update i2c
		TinyWireS_stop_check();
		
		//split bytes for communication
		i2cRegister[0] = (uint8_t) (x >> 8);
		i2cRegister[1] = (uint8_t) x;
    }
}

