/*
 * utils.c
 *
 *  Created on: 17 cze 2022
 *      Author: Grzegorzewski Szymon
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "utils.h"


void Led_Init()
{
	LED_INIT;
}

void BlinkLed()
{
	LED_ON;
	_delay_ms(250);
	LED_OFF;
}


/********************Interrupt********************/
uint64_t millis;
void Timer0_Init()
{
	sei();// allow interrupts

	TCCR0A |= (1<<WGM01); //CTC mode
	TIMSK0 |= (1<<OCIE0A); //Compare match
	TCCR0B |= (1<<CS00) | (1<<CS01); //Prescaler = 64     8000000\64 = 125000
	OCR0A = 124;                     //                   125000\(124+1) = 1000
}

ISR(TIMER0_COMPA_vect)
{
	millis++;
}
/*end*of***************Interrupt********************/
