/*
 * TransmitterIR.c
 *
 *  Created on: 4 lip 2022
 *      Author: Grzegorzewski Szymon
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "TransmitterIR.h"
#include "../Utils/utils.h"

void InitTransmitterIr()
{
	INIT_IR_DIODE;

	//Init timer2 for generating IR 36kHz pulses
	TCCR2A |= (1<<WGM21); //CTC
	TCCR2B |= (1<<CS20); //Pre = 1
	OCR2A = 143;

	//piece of initialization timer1 for precise delayed
	TCCR1B |= (1<<WGM12); //CTC

}

static void wait889us()
{
	OCR1A = 889;
	TIFR1 |= (1<<OCF1A);
	TCCR1B |= (1<<CS11);
	while( !(TIFR1&(1<<OCF1A)) ) {}
	TCCR1B &= ~(1<<CS11);
}


static void SendRc5One(){
	wait889us();
	TIMSK2 |= (1<<OCIE2A);
	wait889us();
	TIMSK2 &=~ (1<<OCIE2A);
}


static void SendRc5Zero(){
	TIMSK2 |= (1<<OCIE2A);
	wait889us();
	TIMSK2 &=~ (1<<OCIE2A);
	wait889us();
}

void SendRc5(uint8_t adr, uint8_t tog, uint8_t cmd){
	uint16_t data = 0;
	int8_t i = 15;
	data |= ( (1<<15)|(1<<14)|(tog<<13)|(adr<<8)|(cmd<<2) ); //sstaaaaaccccccxx

	do{
		if( !(data & (1<<i)) ) SendRc5Zero();
		else SendRc5One();
	}while(--i>1);
}

ISR(TIMER2_COMPA_vect)
{
	IR_DIODE_TOG;
}
