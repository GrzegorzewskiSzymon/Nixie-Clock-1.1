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
void DisplayTime_Normal(int8_t hours, int8_t minutes, int8_t seconds)
{
		if(millis-millis_DisplayTime>10)//Its time to display tens
		{
			NIXIE_TENS_OFF;
			NIXIE_UNITS_OFF;
			if(millis-millis_DisplayTime>11)//shift one ms to cancel ghost numbers in tubes
			{
				NIXIE_TENS_ON;

				//displaying  tens of hours
				PORTB &=~ 0b00001111;
				PORTB |= hours/10;

				//displaying  tens of minutes
				PORTB &=~ 0b11110000;
				PORTB |= ((minutes/10)<<4);

				//displaying  tens of seconds
				PORTC &=~ 0b00001111;
				PORTC |= seconds/10;

				millis_DisplayTime = millis;//set actual time after full cicle
			}
			return;
		}

		if(millis-millis_DisplayTime>5)//Its time to display units
		{
			NIXIE_TENS_OFF;
			NIXIE_UNITS_OFF;
			if(millis-millis_DisplayTime>6)//shift one ms to cancel ghost numbers in tubes
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


void DisplayTime_Slow(int8_t hours, int8_t minutes, int8_t seconds)
{

	if(millis-millis_DisplayTime>20*SPEED_OF_SLOW_MULTIPLEXING)//Its time to display seconds
	{
		//NOT displaying tens/units of hours
		PORTB |= 0b00001111;

		//NOT displaying tens/units of minutes
		PORTB |= 0b11110000;

		if(millis-millis_DisplayTime>25*SPEED_OF_SLOW_MULTIPLEXING)//Its time to display units
		{
			NIXIE_TENS_OFF;
			NIXIE_UNITS_ON;
			//displaying units of seconds
			PORTC &=~ 0b00001111;
			PORTC |= seconds%10;
			if(millis-millis_DisplayTime>30*SPEED_OF_SLOW_MULTIPLEXING)//Its time to display units
			{
				millis_DisplayTime = millis;//set actual time after full cicle
			}
		}
		else//Its still time to display tens
		{
			NIXIE_TENS_ON;
			NIXIE_UNITS_OFF;
			//displaying tens of seconds
			PORTC &=~ 0b00001111;
			PORTC |= seconds/10;
		}
		return;
	}

	if(millis-millis_DisplayTime>10*SPEED_OF_SLOW_MULTIPLEXING)//Its time to display minutes
	{
		//NOT displaying tens/units of hours
		PORTB |= 0b00001111;
		//NOT displaying tens/units of seconds
		PORTC |= 0b00001111;

		if(millis-millis_DisplayTime>15*SPEED_OF_SLOW_MULTIPLEXING)//Its time to display units
		{
			NIXIE_TENS_OFF;
			NIXIE_UNITS_ON;
			//displaying units of minutes
			PORTB &=~ 0b11110000;
			PORTB |= ((minutes%10)<<4);

		}
		else//Its still time to display tens
		{
			NIXIE_TENS_ON;
			NIXIE_UNITS_OFF;
			//displaying tens of minutes
			PORTB &=~ 0b11110000;
			PORTB |= ((minutes/10)<<4);
		}
		return;
	}

	if(millis-millis_DisplayTime>0*SPEED_OF_SLOW_MULTIPLEXING)//Its time to display tens of hours
	{
		//NOT displaying tens/units of minutes
		PORTB |= 0b11110000;

		//NOT displaying tens/units of seconds
		PORTC |= 0b00001111;

		if(millis-millis_DisplayTime>5*SPEED_OF_SLOW_MULTIPLEXING)//Its time to display units of hours
		{
			NIXIE_TENS_OFF;
			NIXIE_UNITS_ON;
			//displaying units of hours
			PORTB &=~ 0b00001111;
			PORTB |= hours%10;

		}
		else//Its still time to display tens
		{
			NIXIE_TENS_ON;
			NIXIE_UNITS_OFF;
			//displaying tens of hours
			PORTB &=~ 0b00001111;
			PORTB |= hours/10;
		}
	}
}

uint64_t millis_DisplayTime_Diodes = 0;
uint64_t millis_SlowMultiplexing = 0;
void DisplayTime(int8_t hours, int8_t minutes, int8_t seconds)
{

	if(millis-millis_SlowMultiplexing > (1000ULL*60ULL*TIME_BETWEEN_SLOW_MULTIPLEXING) )//when it's time to "repair" balance in nixie tubes
	{
		DisplayTime_Slow(hours, minutes, seconds);
		if(millis-millis_SlowMultiplexing > (1000ULL*60ULL*(TIME_BETWEEN_SLOW_MULTIPLEXING)+TIME_OF_SLOW_MULTIPLEXING) )//when it's time to stop "repairing" balance in nixie tubes
		{
			millis_SlowMultiplexing = millis;
		}
	}
	else
	{
		DisplayTime_Normal(hours, minutes, seconds);
	}

	if(millis-millis_DisplayTime_Diodes > DIODES_BLINK_TIME)
	{
		NIXIEDIODES_TOG;
		millis_DisplayTime_Diodes = millis;
	}


}

