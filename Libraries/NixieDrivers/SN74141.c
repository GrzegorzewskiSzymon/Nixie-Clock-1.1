/*
 * SN74141.c
 *
 *  Created on: 23 cze 2022
 *      Author: Grzegorzewski Szymon
 */

#include <avr/io.h>
#include <util/delay.h>
#include "SN74141.h"
#include "AnodesMultiplexer.h"
#include "../Utils/utils.h"
#include "NixieDiodes.h"

void BCD_Init()
{
	BCD_INIT;
}

uint64_t millis_DisplayTime = 0;
uint64_t millis_DisplayTime_Diodes = 0;
uint64_t millis_SlowMultiplexing = 0;
void DisplayTime(int8_t hours, int8_t minutes, int8_t seconds)
{

	uint16_t slower = 1;
	if(millis-millis_SlowMultiplexing > (1000ULL*60ULL*TIME_BETWEEN_SLOW_MULTIPLEXING))//when it's time to "repair" balance in nixie tubes
	{
		slower = 1000;
	}
	if(millis-millis_SlowMultiplexing > ((1000ULL*60ULL*TIME_BETWEEN_SLOW_MULTIPLEXING)+(1000ULL*60ULL*TIME_OF_SLOW_MULTIPLEXING)) )
	{
		slower = 1;
		millis_SlowMultiplexing = millis;

	}

	if(millis-millis_DisplayTime_Diodes > DIODES_BLINK_TIME)
	{
		NIXIEDIODES_TOG;
		millis_DisplayTime_Diodes = millis;
	}


	if(millis-millis_DisplayTime>10*slower)//Its time to display tens
	{
		NIXIE_TENS_OFF;
		NIXIE_UNITS_OFF;
		if(millis-millis_DisplayTime>11*slower)//shift one ms to cancel ghost numbers in tubes
		{
			NIXIE_TENS_ON;

			//displaying  tens of hours
			PORTB &=~ 0b00001111;
			PORTB |= hours/10;

			//displaying  tens of minutes
			PORTB &=~ 0b11110000;
			PORTB |= ((minutes/10)<<4);

			//displaying  tens of minutes
			PORTC &=~ 0b00001111;
			PORTC |= seconds/10;

			millis_DisplayTime = millis;//set actual time after full cicle
			return;
		}
	}

	if(millis-millis_DisplayTime>5*slower)//Its time to display units
	{
		NIXIE_TENS_OFF;
		NIXIE_UNITS_OFF;
		if(millis-millis_DisplayTime>6*slower)//shift one ms to cancel ghost numbers in tubes
		{
			NIXIE_UNITS_ON;

			//displaying units of hours
			PORTB &=~ 0b00001111;
			PORTB |= hours%10;

			//displaying units of minutes
			PORTB &=~ 0b11110000;
			PORTB |= ((minutes%10)<<4);

			//displaying units of seconds
			PORTC &=~ 0b00001111;
			PORTC |= seconds%10;
		}
	}
}

