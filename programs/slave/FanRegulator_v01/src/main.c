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

//rpm
uint16_t x, w, x_old;

//counter interrupt
volatile uint16_t counter, c;

//communication
volatile uint8_t master_data[3], master_bytes;

void receiveEvent(uint8_t num_bytes) {
	
	//receive data
	master_bytes = num_bytes;
	for (uint8_t i = 0; i < master_bytes; i++) {
		master_data[i] = usiTwiReceiveByte();
	}

	//mode
	uint16_t buffer =  master_data[1];
	buffer = (buffer << 8) + master_data[2];
	switch (master_data[0]) {
		case 0x00:
			//rpm
			w = buffer;
			break;
		case 0x01:
			//p
			setKp(buffer/1000);
			break;
		case 0x02:
			//i
			setKi(buffer/1000);
			break;
		case 0x03:
			//d
			setKd(buffer/1000);
			break;
	}
}

void requestEvent() {

	//split bytes for communication
	usiTwiTransmitByte(x>>8);
	usiTwiTransmitByte(x);
	usiTwiTransmitByte(getDuCy());
}

void changeEvent() {
	
	//count
	counter++;
}

void checkEvent() {
	
	//calculate rpm
	c = counter;
	counter = 0;
}

int main() {
	
	//default
	c = 0;
	w = 0;
	x = 0;
	counter = 0;
	
	//i2c init
    usiTwiSlaveInit(ADRESS);
	
	//i2c callback
	usi_onReceiverPtr = *receiveEvent;
	usi_onRequestPtr = *requestEvent;
	
	//interrupt callback
	onChange = *changeEvent;
	onCheck = *checkEvent;
	
	//pid
	pidInit(KP, KI, KD, MAX_SPEED);
	setDt(REFRESH_SPEED);
	
	//disable interrupts
	cli();
	
	//pwm
	pwmInit();
	setFrequency(PWM_FREQUENCY);

	//pin change
	pinChangeInit(TACHO_PIN);
	
	//timer interrupt
	ovfInit();
	
	//enable interrupts
	sei();
	
    while(1) {
		
		//calculation
		x_old = x;
		x = (c*60*REFRESH_SPEED)/(2*COUNTS_PER_ROTATION);
		
		//calculate correction error by deviation
		if(((x-x_old)>DEVIATION)||((x_old-x)>DEVIATION))
			setDuCy(control(x, w));
		
		//update i2c
		TinyWireS_stop_check();
    }
}

