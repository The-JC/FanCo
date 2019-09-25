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
	switch (master_data[0]) {
		case 0x00:
			//rpm
			w = master_data[1];
			w = (w << 8) + master_data[2];
			break;
		case 0x01:
			//p
			break;
		case 0x02:
			//i
			break;
		case 0x03:
			//d
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
	OCR0B = control(x, w);
}

int main(void) {
	
	//set
	w = 1000;
	x = 0;
	counter = 0;
	
	//correction
	factor = 2*COUNTS_PER_ROTATION*(F_CPU/(float)PRESCALER/256)*1000;
	
	//i2c begin
    usiTwiSlaveInit(ADRESS);
	
	//i2c callback
	usi_onReceiverPtr = *receiveEvent;
	usi_onRequestPtr = *requestEvent;
	
	//interrupt callback
	onChange = *changeEvent;
	onCheck = *checkEvent;
	
	//pid
	setKp(0.7);
	setKi(0.3);
	setKp(0.05);
	setDt(REFRESH_SPEED);
	
	//disable interrupts
	cli();
	
	//pwm
	pwmInit();
	setFrequency(10000);

	//pin change
	pinChangeInit(3);
	
	//interrupt
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

